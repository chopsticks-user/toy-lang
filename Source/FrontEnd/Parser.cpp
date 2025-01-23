#include "Parser.hpp"

namespace tl::fe {
  // auto UnitParser::then(TThenFn fn) const -> UnitParser {
  //   return UnitParser{
  //     [&fn, this](CParsingDriver &driver) -> TReturn {
  //       auto [nodes, errors] = m_fn(driver);
  //       if (nodes.empty()) {
  //         return {{}, std::move(errors)};
  //       }
  //       return fn(std::move(nodes), std::move(errors));
  //     }
  //   };
  // }

  // template<typename U, typename V>
  // class UnitParserOr final : public UnitParserExpr {
  // public:
  //   static constexpr bool isLeaf = false;
  //
  //   UnitParserOr(U &&u, V &&v)
  //     : m_u(std::forward<U>(u)), m_v(std::forward<V>(v)) {
  //   }
  //
  // private:
  //   TOperand<U> m_u;
  //   TOperand<V> m_v;
  // };
  //
  // template<typename U, typename V>
  // class UnitParserAnd final : public UnitParserExpr {
  // public:
  //   static constexpr bool isLeaf = false;
  //
  //   UnitParserAnd(U &&u, V &&v)
  //     : m_u(std::forward<U>(u)), m_v(std::forward<V>(v)) {
  //   }
  //
  // private:
  //   TOperand<U> m_u;
  //   TOperand<V> m_v;
  // };
  //
  // template<typename U, typename V>
  // auto operator|(UnitParserExpr &&u, UnitParserExpr &&v) -> UnitParserOr<U, V> {
  //   return {std::forward<U>(u), std::forward<V>(v)};
  // }
  //
  // template<typename U, typename V>
  // auto operator&(UnitParserExpr &&u, UnitParserExpr &&v) -> UnitParserAnd<U, V> {
  //   return {std::forward<U>(u), std::forward<V>(v)};
  // }

  // auto operator|(UnitParser &&u, UnitParser &&v) -> UnitParser {
  //   return UnitParser{
  //     [&](ParsingDriver &driver) -> UnitParser::TReturn {
  //       driver.markRevertPoint();
  //
  //       if (auto [resultNodes, errors] = u(driver); !resultNodes.empty()) {
  //         return {std::move(resultNodes), std::move(errors)};
  //       }
  //
  //       if (auto [resultNodes, errors] = v(driver); !resultNodes.empty()) {
  //         return {std::move(resultNodes), std::move(errors)};
  //       }
  //
  //       // todo: error
  //       driver.toRevertPoint();
  //       return {{}, {}};
  //     }
  //   };
  // }
  //
  // auto operator&(UnitParser &&u, UnitParser &&v) -> UnitParser {
  //   return UnitParser{
  //     [&](ParsingDriver &driver) -> UnitParser::TReturn {
  //       driver.markRevertPoint();
  //
  //       Vec<syntax::ASTNode> nodes;
  //       Strings combindedErrors;
  //
  //       if (auto [resultNodes, errors] = u(driver); resultNodes.empty()) {
  //         // todo: error
  //         driver.toRevertPoint();
  //         return {{}, {}};
  //       } else {
  //         nodes.insert(nodes.end(), resultNodes.begin(), resultNodes.end());
  //         combindedErrors.insert(combindedErrors.end(), errors.begin(), errors.end());
  //       }
  //
  //       if (auto [resultNodes, errors] = v(driver); resultNodes.empty()) {
  //         // todo: error
  //         driver.toRevertPoint();
  //         return {{}, {}};
  //       } else {
  //         nodes.insert(nodes.end(), resultNodes.begin(), resultNodes.end());
  //         combindedErrors.insert(combindedErrors.end(), errors.begin(), errors.end());
  //       }
  //
  //       return {std::move(nodes), std::move(combindedErrors)};
  //     }
  //   };
  // }
  //
  // auto operator*(UnitParser &&parser) -> UnitParser {
  //   return UnitParser{
  //     [&parser](ParsingDriver &driver) -> UnitParser::TReturn {
  //       Vec<syntax::ASTNode> nodes;
  //       Strings combindedErrors;
  //       while (true) {
  //         auto [resultNodes, errors] = parser(driver);
  //         if (resultNodes.empty()) {
  //           break;
  //         }
  //         nodes.insert(nodes.end(), resultNodes.begin(), resultNodes.end());
  //         combindedErrors.insert(combindedErrors.end(), errors.begin(), errors.end());
  //       }
  //       return {std::move(nodes), std::move(combindedErrors)};
  //     }
  //   };
  // }
  //
  // auto operator+(UnitParser &&parser) -> UnitParser {
  //   return std::forward<UnitParser>(parser) | *std::forward<UnitParser>(parser);
  // }
  //
  // auto operator~(UnitParser &&parser) -> UnitParser {
  //   return UnitParser{
  //     [&parser](ParsingDriver &driver) -> UnitParser::TReturn {
  //       auto [resultNodes, errors] = parser(driver);
  //       Vec<syntax::ASTNode> nodes = std::move(resultNodes);
  //       if (nodes.empty()) {
  //         nodes.emplace_back();
  //       }
  //       return {std::move(nodes), std::move(errors)};
  //     }
  //   };
  // }

  auto Parser::operator()(String filepath, Tokens tokens)
    -> std::tuple<syntax::TranslationUnit, Strings> {
    m_driver = {std::move(filepath), std::move(tokens)};

    auto [nodes, errors] = translationUnit()(m_driver);
    if (nodes.empty()) {
      throw InternalException(m_driver.filepath(), "missing module declaration");
    }
    return {astCast<syntax::TranslationUnit>(nodes.front()), errors};
  }

  auto Parser::match(std::same_as<EToken> auto... expected) -> UnitParser {
    return UnitParser{
      [expected...](ParsingDriver &driver) -> UnitParser::TReturn {
        if (driver.match(expected...)) {
          return {{syntax::TokenNode{driver.peekPrev()}}, {}};
        }
        return {{}, {}};
      }
    };
  }

  auto Parser::any(std::same_as<UnitParser> auto... parsers) -> UnitParser {
    return UnitParser{
      [parsers...](ParsingDriver &driver) -> UnitParser::TReturn {
        for (const auto &parser: {parsers...}) {
          const auto revertPoint = driver.currentIndex();
          UnitParser::TReturn result = parser(driver);
          if (auto &[resultNodes, errors] = result;
            !resultNodes.empty()) {
            return result;
          }
          driver.to(revertPoint);
        }
        return {{}, {}};
      }
    };
  }

  auto Parser::sequence(std::same_as<UnitParser> auto... parsers) -> UnitParser {
    return UnitParser{
      [parsers...](ParsingDriver &driver) -> UnitParser::TReturn {
        const auto revertPoint = driver.currentIndex();
        Vec<syntax::ASTNode> nodes;
        Strings combindedErrors;
        for (const auto &parser: {parsers...}) {
          UnitParser::TReturn result = parser(driver);
          auto &[resultNodes, errors] = result;
          if (resultNodes.empty()) {
            driver.to(revertPoint);
            return {{}, {}};
          }
          nodes.insert(nodes.end(), resultNodes.begin(), resultNodes.end());
          combindedErrors.insert(combindedErrors.end(), errors.begin(), errors.end());
        }
        return {std::move(nodes), std::move(combindedErrors)};
      }
    };
  }

  auto Parser::maybe(CRef<UnitParser> parser) -> UnitParser {
    return UnitParser{
      [parser](ParsingDriver &driver) -> UnitParser::TReturn {
        const auto revertPoint = driver.currentIndex();
        auto [resultNodes, errors] = parser(driver);
        Vec<syntax::ASTNode> nodes = std::move(resultNodes);
        if (nodes.empty()) {
          driver.to(revertPoint);
          nodes.emplace_back();
        }
        return {std::move(nodes), std::move(errors)};
      }
    };
  }

  auto Parser::many0(CRef<UnitParser> parser) -> UnitParser {
    return UnitParser{
      [parser](ParsingDriver &driver) -> UnitParser::TReturn {
        Vec<syntax::ASTNode> nodes;
        Strings combindedErrors;
        while (true) {
          auto [resultNodes, errors] = parser(driver);
          if (resultNodes.empty()) {
            break;
          }
          nodes.insert(nodes.end(), resultNodes.begin(), resultNodes.end());
          combindedErrors.insert(combindedErrors.end(), errors.begin(), errors.end());
        }
        if (nodes.empty()) {
          nodes.emplace_back();
        }
        return {std::move(nodes), std::move(combindedErrors)};
      }
    };
  }

  auto Parser::many1(CRef<UnitParser> parser) -> UnitParser {
    return sequence(parser, many0(parser));
  }

  auto Parser::list(CRef<UnitParser> parser, const EToken separator, const bool trailing) -> UnitParser {
    return maybe(
      trailing
        ? sequence(parser, many0(sequence(match(separator), parser)), maybe(match(separator)))
        : sequence(parser, many0(sequence(match(separator), parser)))
    );
  }

  auto Parser::integerExpr() -> UnitParser {
    return UnitParser{
      [](ParsingDriver &driver) -> UnitParser::TReturn {
        auto [nodes, errors] = match(EToken::IntegerLiteral)(driver);

        if (!nodes.empty()) {
          nodes = {
            syntax::IntegerLiteral{
              std::stoll(syntax::astCast<syntax::TokenNode>(nodes.front()).token().string())
            }
          };
        }

        return {std::move(nodes), std::move(errors)};
      }
    };
  }

  auto Parser::floatExpr() -> UnitParser {
    return UnitParser{
      [](ParsingDriver &driver) -> UnitParser::TReturn {
        auto [nodes, errors] = match(EToken::FloatLiteral)(driver);

        if (!nodes.empty()) {
          nodes = {
            syntax::FloatLiteral{
              std::stod(syntax::astCast<syntax::TokenNode>(nodes.front()).token().string())
            }
          };
        }

        return {std::move(nodes), std::move(errors)};
      }
    };
  }

  auto Parser::booleanExpr() -> UnitParser {
    return UnitParser{
      [](ParsingDriver &driver) -> UnitParser::TReturn {
        auto [nodes, errors] = match(EToken::True, EToken::False)(driver);

        if (!nodes.empty()) {
          nodes = {
            syntax::BooleanLiteral{
              syntax::astCast<syntax::TokenNode>(nodes.front()).token().type() == EToken::True
            }
          };
        }

        return {std::move(nodes), std::move(errors)};
      }
    };
  }

  auto Parser::stringExpr() -> UnitParser {
    return UnitParser{
      [](ParsingDriver &driver) -> UnitParser::TReturn {
        auto [nodes, errors] = match(EToken::StringLiteral)(driver);

        if (!nodes.empty()) {
          nodes = {
            syntax::StringLiteral{
              syntax::astCast<syntax::TokenNode>(nodes.front()).token().string()
            }
          };
        }

        return {std::move(nodes), std::move(errors)};
      }
    };
  }

  /**
   * Grammar:
   * - AnonymousIdentifier
   * - (Identifier Colon2)* Identifier
   *
   * Results:
   * - "_" ↦ {"_"}
   * - "var" ↦ {{}, "var"}
   * - "foo::bar::var" ↦ {"foo", "::", "bar", "::", "var"}
   */
  auto Parser::varIdExpr() -> UnitParser {
    return UnitParser{
      [](ParsingDriver &driver) -> UnitParser::TReturn {
        auto [nodes, errors] = any(
          match(EToken::AnonymousIdentifier),
          sequence(
            many0(sequence(match(EToken::Identifier), match(EToken::Colon2))),
            match(EToken::Identifier)
          )
        )(driver);

        if (nodes.empty()) {
          return {{}, {}};
        }

        if (nodes.size() == 1 && syntax::matchAstType<syntax::TokenNode>(nodes.front())) {
          if (syntax::astCast<syntax::TokenNode>(nodes.front()).token().type()
              == EToken::AnonymousIdentifier)
            nodes = {syntax::VarId{}};
        } else if (nodes.size() == 2 && isEmptyAst(nodes.front())) {
          nodes = {syntax::VarId{{syntax::astCast<syntax::TokenNode>(nodes.back()).token().string()}}};
        } else {
          auto pathView =
              nodes | rv::filter([](CRef<syntax::ASTNode> node) {
                return syntax::astCast<syntax::TokenNode>(node).token().type() != EToken::Colon2;
              }) |
              rv::transform([](CRef<syntax::ASTNode> node) {
                return syntax::astCast<syntax::TokenNode>(node).token().string();
              });
          nodes = {syntax::VarId{{pathView.begin(), pathView.end()}}};
        }

        return {std::move(nodes), std::move(errors)};
      }
    };
  }

  /**
   * Grammar:
   * - (Identifier Colon2)* (UserDefinedType | FundamentalType)
   *
   * Results:
   * - "Type" ↦ {{}, "Type"}
   * - "foo::bar::Type" ↦ {"foo", "::", "bar", "::", "Type"}
   */
  auto Parser::typeIdExpr() -> UnitParser {
    return UnitParser{
      [](ParsingDriver &driver) -> UnitParser::TReturn {
        auto [nodes, errors] = sequence(
          many0(sequence(match(EToken::Identifier), match(EToken::Colon2))),
          match(EToken::UserDefinedType, EToken::FundamentalType)
        )(driver);

        if (nodes.empty()) {
          return {{}, {}};
        }

        if (nodes.size() == 2 && isEmptyAst(nodes.front())) {
          nodes = {syntax::TypeId{{syntax::astCast<syntax::TokenNode>(nodes.back()).token().string()}}};
        } else {
          auto pathView =
              nodes | rv::filter([](CRef<syntax::ASTNode> node) {
                return syntax::astCast<syntax::TokenNode>(node).token().type() != EToken::Colon2;
              }) |
              rv::transform([](CRef<syntax::ASTNode> node) {
                return syntax::astCast<syntax::TokenNode>(node).token().string();
              });
          nodes = {syntax::TypeId{{pathView.begin(), pathView.end()}}};
        }

        return {std::move(nodes), std::move(errors)};
      }
    };
  }

  auto Parser::opIdExpr() -> UnitParser {
    return UnitParser{
      [](ParsingDriver &driver) -> UnitParser::TReturn {
        auto [nodes, errors] = match(
          EToken::Plus, EToken::Minus, EToken::Star, EToken::FwdSlash, EToken::Equal2,
          EToken::ExclaimEqual
        )(driver);

        return {
          {syntax::OpId{syntax::astCast<syntax::TokenNode>(nodes.front()).token().string()}},
          errors
        };
      }
    };
  }

  auto Parser::arrayExpr() -> UnitParser {
    return UnitParser{
      [](ParsingDriver &driver) -> UnitParser::TReturn {
        auto [nodes, errors] = sequence(
          match(EToken::LeftBracket), list(expr()), match(EToken::RightBracket)
        )(driver);

        if (!nodes.empty()) {
          nodes = {
            syntax::ArrayExpr{{nodes.begin() + 1, nodes.end() - 1}}
          };
        }

        return {std::move(nodes), std::move(errors)};
      }
    };
  }

  auto Parser::tupleExpr() -> UnitParser {
    return UnitParser{
      [](ParsingDriver &driver) -> UnitParser::TReturn {
        auto [nodes, errors] = sequence(
          match(EToken::LeftParen), list(expr()), match(EToken::RightParen)
        )(driver);

        if (!nodes.empty()) {
          nodes = {
            syntax::TupleExpr{{nodes.begin() + 1, nodes.end() - 1}}
          };
        }

        return {std::move(nodes), std::move(errors)};
      }
    };
  }

  auto Parser::lambdaExpr() -> UnitParser {
    return UnitParser{
      [](ParsingDriver &driver) -> UnitParser::TReturn {
        return {{}, {}};
      }
    };
  }

  auto Parser::fnCallExpr() -> UnitParser {
    return UnitParser{
      [](ParsingDriver &driver) -> UnitParser::TReturn {
        auto [nodes, errors] = sequence(
          primaryExpr(), tupleExpr()
        )(driver);

        return {std::move(nodes), std::move(errors)};
      }
    };
  }

  auto Parser::subscriptExpr() -> UnitParser {
    return UnitParser{
      [](ParsingDriver &driver) -> UnitParser::TReturn {
        auto [nodes, errors] = sequence(
          primaryExpr(), match(EToken::LeftBracket), expr(), match(EToken::RightBracket)
        )(driver);

        return {std::move(nodes), std::move(errors)};
      }
    };
  }

  auto Parser::accessExpr() -> UnitParser {
    return UnitParser{
      [](ParsingDriver &driver) -> UnitParser::TReturn {
        auto [nodes, errors] = sequence(
          primaryExpr(), match(EToken::Dot), varIdExpr()
        )(driver);

        return {std::move(nodes), std::move(errors)};
      }
    };
  }

  auto Parser::primaryExpr() -> UnitParser {
    return UnitParser{
      [](ParsingDriver &driver) -> UnitParser::TReturn {
        auto [nodes, errors] = any(
          integerExpr(), floatExpr(), booleanExpr(), stringExpr(), varIdExpr(),
          arrayExpr(), tupleExpr(), lambdaExpr()
        )(driver);

        return {std::move(nodes), std::move(errors)};
      }
    };
  }

  auto Parser::postfixExpr() -> UnitParser {
    return UnitParser{
      [](ParsingDriver &driver) -> UnitParser::TReturn {
        auto [nodes, errors] = any(
          fnCallExpr(), subscriptExpr(), accessExpr()
        )(driver);

        return {std::move(nodes), std::move(errors)};
      }
    };
  }

  auto Parser::prefixExpr() -> UnitParser {
    return UnitParser{
      [](ParsingDriver &driver) -> UnitParser::TReturn {
        auto [nodes, errors] = any(
          match(EToken::Exclaim, EToken::Tilde, EToken::Plus,
                EToken::Minus, EToken::Hash, EToken::Dot3),
          postfixExpr()
        )(driver);

        return {std::move(nodes), std::move(errors)};
      }
    };
  }

  auto Parser::exponentialExpr() -> UnitParser {
    return UnitParser{
      [](ParsingDriver &driver) -> UnitParser::TReturn {
        auto [nodes, errors] = sequence(
          prefixExpr(), maybe(sequence(match(EToken::Star2), prefixExpr()))
        )(driver);

        if (isEmptyAst(nodes.back())) {
          nodes.pop_back();
        }

        return {std::move(nodes), std::move(errors)};
      }
    };
  }

  auto Parser::multiplicativeExpr() -> UnitParser {
    return UnitParser{
      [](ParsingDriver &driver) -> UnitParser::TReturn {
        auto [nodes, errors] = sequence(
          exponentialExpr(),
          maybe(sequence(match(EToken::Star, EToken::FwdSlash, EToken::Percent),
                         exponentialExpr()))
        )(driver);

        if (isEmptyAst(nodes.back())) {
          nodes.pop_back();
        }

        return {std::move(nodes), std::move(errors)};
      }
    };
  }

  auto Parser::additiveExpr() -> UnitParser {
    return UnitParser{
      [](ParsingDriver &driver) -> UnitParser::TReturn {
        auto [nodes, errors] = sequence(
          multiplicativeExpr(),
          maybe(sequence(match(EToken::Plus, EToken::Minus), multiplicativeExpr()))
        )(driver);

        if (isEmptyAst(nodes.back())) {
          nodes.pop_back();
        }

        return {std::move(nodes), std::move(errors)};
      }
    };
  }

  auto Parser::shiftExpr() -> UnitParser {
    return UnitParser{
      [](ParsingDriver &driver) -> UnitParser::TReturn {
        auto [nodes, errors] = sequence(
          additiveExpr(),
          maybe(sequence(match(EToken::Less2, EToken::Greater2), additiveExpr()))
        )(driver);

        if (isEmptyAst(nodes.back())) {
          nodes.pop_back();
        }

        return {std::move(nodes), std::move(errors)};
      }
    };
  }

  auto Parser::relationalExpr() -> UnitParser {
    return UnitParser{
      [](ParsingDriver &driver) -> UnitParser::TReturn {
        auto [nodes, errors] = sequence(
          shiftExpr(),
          maybe(
            sequence(
              match(EToken::Less, EToken::LessEqual, EToken::Greater, EToken::GreaterEqual),
              shiftExpr()
            )
          )
        )(driver);

        if (isEmptyAst(nodes.back())) {
          nodes.pop_back();
        }

        return {std::move(nodes), std::move(errors)};
      }
    };
  }

  auto Parser::equalityExpr() -> UnitParser {
    return UnitParser{
      [](ParsingDriver &driver) -> UnitParser::TReturn {
        auto [nodes, errors] = sequence(
          relationalExpr(),
          maybe(sequence(match(EToken::Equal2, EToken::ExclaimEqual), relationalExpr()))
        )(driver);

        if (isEmptyAst(nodes.back())) {
          nodes.pop_back();
        }

        return {std::move(nodes), std::move(errors)};
      }
    };
  }

  auto Parser::andExpr() -> UnitParser {
    return UnitParser{
      [](ParsingDriver &driver) -> UnitParser::TReturn {
        auto [nodes, errors] = sequence(
          equalityExpr(),
          maybe(sequence(match(EToken::Ampersand), equalityExpr()))
        )(driver);

        if (isEmptyAst(nodes.back())) {
          nodes.pop_back();
        }

        return {std::move(nodes), std::move(errors)};
      }
    };
  }

  auto Parser::exclusiveOrExpr() -> UnitParser {
    return UnitParser{
      [](ParsingDriver &driver) -> UnitParser::TReturn {
        auto [nodes, errors] = sequence(
          andExpr(),
          maybe(sequence(match(EToken::Hat), andExpr()))
        )(driver);

        if (isEmptyAst(nodes.back())) {
          nodes.pop_back();
        }

        return {std::move(nodes), std::move(errors)};
      }
    };
  }

  auto Parser::inclusiveOrExpr() -> UnitParser {
    return UnitParser{
      [](ParsingDriver &driver) -> UnitParser::TReturn {
        auto [nodes, errors] = sequence(
          exclusiveOrExpr(),
          maybe(sequence(match(EToken::Bar), exclusiveOrExpr()))
        )(driver);

        if (isEmptyAst(nodes.back())) {
          nodes.pop_back();
        }

        return {std::move(nodes), std::move(errors)};
      }
    };
  }

  auto Parser::logicalAndExpr() -> UnitParser {
    return UnitParser{
      [](ParsingDriver &driver) -> UnitParser::TReturn {
        auto [nodes, errors] = sequence(
          inclusiveOrExpr(),
          maybe(sequence(match(EToken::Ampersand2), inclusiveOrExpr()))
        )(driver);

        if (isEmptyAst(nodes.back())) {
          nodes.pop_back();
        }

        return {std::move(nodes), std::move(errors)};
      }
    };
  }

  auto Parser::logicalOrExpr() -> UnitParser {
    return UnitParser{
      [](ParsingDriver &driver) -> UnitParser::TReturn {
        auto [nodes, errors] = sequence(
          logicalAndExpr(),
          maybe(sequence(match(EToken::Bar2), logicalAndExpr()))
        )(driver);

        if (isEmptyAst(nodes.back())) {
          nodes.pop_back();
        }

        return {std::move(nodes), std::move(errors)};
      }
    };
  }

  auto Parser::nullCoalescingExpr() -> UnitParser {
    return UnitParser{
      [](ParsingDriver &driver) -> UnitParser::TReturn {
        auto [nodes, errors] = sequence(
          logicalOrExpr(),
          maybe(sequence(match(EToken::QMark2), logicalOrExpr()))
        )(driver);

        if (isEmptyAst(nodes.back())) {
          nodes.pop_back();
        }

        return {std::move(nodes), std::move(errors)};
      }
    };
  }

  auto Parser::sequenceExpr() -> UnitParser {
    return UnitParser{
      [](ParsingDriver &driver) -> UnitParser::TReturn {
        auto [nodes, errors] = sequence(
          nullCoalescingExpr(),
          maybe(
            sequence(
              match(EToken::Dot2), nullCoalescingExpr(),
              maybe(sequence(match(EToken::By, EToken::At), nullCoalescingExpr()))
            )
          )
        )(driver);

        if (isEmptyAst(nodes.back())) {
          nodes.pop_back();
        }

        return {std::move(nodes), std::move(errors)};
      }
    };
  }

  auto Parser::ternaryExpr() -> UnitParser {
    return UnitParser{
      [](ParsingDriver &driver) -> UnitParser::TReturn {
        auto [nodes, errors] = sequence(
          sequenceExpr(),
          maybe(
            sequence(
              match(EToken::QMark), sequenceExpr(),
              sequence(match(EToken::Colon), sequenceExpr())
            )
          )
        )(driver);

        if (isEmptyAst(nodes.back())) {
          nodes.pop_back();
        }

        return {std::move(nodes), std::move(errors)};
      }
    };
  }

  auto Parser::expr() -> UnitParser {
    return UnitParser{
      [](ParsingDriver &driver) -> UnitParser::TReturn {
        return ternaryExpr()(driver);
      }
    };
  }

  auto Parser::identifierDecl() -> UnitParser {
    return UnitParser{
      [](ParsingDriver &driver) -> UnitParser::TReturn {
        auto [nodes, errors] = any(
          sequence(
            maybe(match(EToken::Mut)),
            varIdExpr(),
            maybe(sequence(match(EToken::Colon), typeIdExpr()))
          ),
          typeIdExpr()
        )(driver);

        if (nodes.empty()) {
          return {{}, {}};
        }

        if (nodes.size() == 1) {
          nodes = {syntax::IdentifierDecl{false, {}, nodes[0]}};
        } else {
          const auto mut = !isEmptyAst(nodes[0]);
          const auto varId = nodes[1];
          const auto typeId = isEmptyAst(nodes[2]) ? syntax::ASTNode{} : nodes[3];
          nodes = {syntax::IdentifierDecl{mut, varId, typeId}};
        }

        return {std::move(nodes), std::move(errors)};
      }
    };
  }

  auto Parser::tupleDecl() -> UnitParser {
    return UnitParser{
      [](ParsingDriver &driver) -> UnitParser::TReturn {
        auto [nodes, errors] = sequence(
          match(EToken::LeftParen), list(identifierDecl()), match(EToken::RightParen)
        )(driver);

        if (!nodes.empty()) {
          auto view = nodes | rv::filter([](CRef<syntax::ASTNode> node) {
            return syntax::matchAstType<syntax::IdentifierDecl>(node);
          });
          nodes = {syntax::TupleDecl{{view.begin(), view.end()}}};
        }

        return {std::move(nodes), std::move(errors)};
      }
    };
  }

  auto Parser::stmt() -> UnitParser {
  }

  auto Parser::translationUnit() -> UnitParser {
  }
}
