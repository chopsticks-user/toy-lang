#include "CombParser.hpp"

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

  auto CParser::operator()(String filepath, Tokens tokens)
    -> std::tuple<syntax::TranslationUnit, Strings> {
    m_driver = {std::move(filepath), std::move(tokens)};

    auto [nodes, errors] = translationUnit()(m_driver);
    if (nodes.empty()) {
      throw InternalException(m_driver.filepath(), "missing module declaration");
    }
    return {astCast<syntax::TranslationUnit>(nodes.front()), errors};
  }

  auto match(std::same_as<EToken> auto... expected) -> UnitParser {
    return UnitParser{
      [expected...](CParsingDriver &driver) -> UnitParser::TReturn {
        if (driver.match(expected...)) {
          return {{syntax::TokenNode{driver.peekPrev()}}, {}};
        }
        return {{}, {}};
      }
    };
  }

  auto any(std::same_as<UnitParser> auto... parsers) -> UnitParser {
    return UnitParser{
      [parsers...](CParsingDriver &driver) -> UnitParser::TReturn {
        driver.markRevertPoint();
        for (const auto &parser: {parsers...}) {
          UnitParser::TReturn result = parser(driver);
          if (auto &[resultNodes, errors] = result;
            !resultNodes.empty()) {
            return result;
          }
        }
        driver.toRevertPoint();
        return {{}, {}};
      }
    };
  }

  auto sequence(std::same_as<UnitParser> auto... parsers) -> UnitParser {
    return UnitParser{
      [parsers...](CParsingDriver &driver) -> UnitParser::TReturn {
        driver.markRevertPoint();
        Vec<syntax::ASTNode> nodes;
        Strings combindedErrors;
        for (const auto &parser: {parsers...}) {
          UnitParser::TReturn result = parser(driver);
          auto &[resultNodes, errors] = result;
          if (resultNodes.empty()) {
            driver.toRevertPoint();
            return {{}, {}};
          }
          nodes.insert(nodes.end(), resultNodes.begin(), resultNodes.end());
          combindedErrors.insert(combindedErrors.end(), errors.begin(), errors.end());
        }
        return {std::move(nodes), std::move(combindedErrors)};
      }
    };
  }

  auto maybe(CRef<UnitParser> parser) -> UnitParser {
    return UnitParser{
      [parser](CParsingDriver &driver) -> UnitParser::TReturn {
        auto [resultNodes, errors] = parser(driver);
        Vec<syntax::ASTNode> nodes = std::move(resultNodes);
        if (nodes.empty()) {
          nodes.emplace_back();
        }
        return {std::move(nodes), std::move(errors)};
      }
    };
  }

  auto many0(CRef<UnitParser> parser) -> UnitParser {
    return UnitParser{
      [parser](CParsingDriver &driver) -> UnitParser::TReturn {
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
        return {std::move(nodes), std::move(combindedErrors)};
      }
    };
  }

  auto many1(CRef<UnitParser> parser) -> UnitParser {
    return sequence(parser, many0(parser));
  }

  auto list(CRef<UnitParser> parser, const EToken separator, const bool trailing) -> UnitParser {
    return maybe(
      trailing
        ? sequence(parser, many0(sequence(match(separator), parser)), maybe(match(separator)))
        : sequence(parser, many0(sequence(match(separator), parser)))
    );
  }

  auto integerExpr() -> UnitParser {
    return UnitParser{
      [](CParsingDriver &driver) -> UnitParser::TReturn {
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

  auto floatExpr() -> UnitParser {
    return UnitParser{
      [](CParsingDriver &driver) -> UnitParser::TReturn {
        auto [nodes, errors] = match(EToken::IntegerLiteral)(driver);

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

  auto booleanExpr() -> UnitParser {
    return UnitParser{
      [](CParsingDriver &driver) -> UnitParser::TReturn {
        auto [nodes, errors] = match(EToken::IntegerLiteral)(driver);

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

  auto stringExpr() -> UnitParser {
    return UnitParser{
      [](CParsingDriver &driver) -> UnitParser::TReturn {
        auto [nodes, errors] = match(EToken::IntegerLiteral)(driver);

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

  auto variableIdExpr() -> UnitParser {
    return UnitParser{
      [](CParsingDriver &driver) -> UnitParser::TReturn {
        auto [nodes, errors] = any(
          match(EToken::AnonymousIdentifier),
          sequence(
            many0(sequence(match(EToken::Identifier), match(EToken::Colon2))),
            match(EToken::Identifier, EToken::UserDefinedType, EToken::FundamentalType)
          )
        )(driver);

        if (nodes.size() == 1 && syntax::matchAstType<syntax::TokenNode>(nodes.front())) {
          if (syntax::astCast<syntax::TokenNode>(nodes.front()).token().type()
              == EToken::AnonymousIdentifier)
            return {{syntax::Identifier{}}, errors};
        }

        auto pathView =
            nodes | rv::filter([](CRef<syntax::ASTNode> node) {
              return syntax::astCast<syntax::TokenNode>(node).token().type() != EToken::Colon2;
            }) |
            rv::transform([](CRef<syntax::ASTNode> node) {
              return syntax::astCast<syntax::TokenNode>(node).token().string();
            });
        return {{syntax::Identifier{{pathView.begin(), pathView.end()}}}, errors};
      }
    };
  }

  auto typeIdExpr() -> UnitParser {
    return UnitParser{
      [](CParsingDriver &driver) -> UnitParser::TReturn {
        auto [nodes, errors] = sequence(
          many0(sequence(match(EToken::Identifier), match(EToken::Colon2))),
          match(EToken::UserDefinedType, EToken::FundamentalType)
        )(driver);

        if (nodes.size() == 1 && syntax::matchAstType<syntax::TokenNode>(nodes.front())) {
          if (syntax::astCast<syntax::TokenNode>(nodes.front()).token().type()
              == EToken::AnonymousIdentifier)
            return {{syntax::Identifier{}}, errors};
        }

        auto pathView =
            nodes | rv::filter([](CRef<syntax::ASTNode> node) {
              return syntax::astCast<syntax::TokenNode>(node).token().type() != EToken::Colon2;
            }) |
            rv::transform([](CRef<syntax::ASTNode> node) {
              return syntax::astCast<syntax::TokenNode>(node).token().string();
            });
        return {{syntax::Identifier{{pathView.begin(), pathView.end()}}}, errors};
      }
    };
  }

  auto typeExpr() -> UnitParser {
    return UnitParser{
      [](CParsingDriver &driver) -> UnitParser::TReturn {
        auto [nodes, errors] = sequence(
          list(variableIdExpr(), EToken::Bar, false)
        )(driver);

        if (!nodes.empty()) {
          auto view = nodes | rv::filter([](CRef<syntax::ASTNode> node) {
            return !syntax::matchAstType<syntax::TokenNode>(node);
          });

          nodes = {
            syntax::TypeExpr{{view.begin(), view.end()}}
          };
        }

        return {std::move(nodes), std::move(errors)};
      }
    };
  }

  auto arrayExpr() -> UnitParser {
    return UnitParser{
      [](CParsingDriver &driver) -> UnitParser::TReturn {
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

  auto tupleExpr() -> UnitParser {
    return UnitParser{
      [](CParsingDriver &driver) -> UnitParser::TReturn {
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

  auto lambdaExpr() -> UnitParser {
    return UnitParser{
      [](CParsingDriver &driver) -> UnitParser::TReturn {
        return {{}, {}};
      }
    };
  }

  auto fnCallExpr() -> UnitParser {
    return UnitParser{
      [](CParsingDriver &driver) -> UnitParser::TReturn {
        auto [nodes, errors] = sequence(
          primaryExpr(), tupleExpr()
        )(driver);

        return {std::move(nodes), std::move(errors)};
      }
    };
  }

  auto subscriptExpr() -> UnitParser {
    return UnitParser{
      [](CParsingDriver &driver) -> UnitParser::TReturn {
        auto [nodes, errors] = sequence(
          primaryExpr(), match(EToken::LeftBracket), expr(), match(EToken::RightBracket)
        )(driver);

        return {std::move(nodes), std::move(errors)};
      }
    };
  }

  auto accessExpr() -> UnitParser {
    return UnitParser{
      [](CParsingDriver &driver) -> UnitParser::TReturn {
        auto [nodes, errors] = sequence(
          primaryExpr(), match(EToken::Dot), variableIdExpr()
        )(driver);

        return {std::move(nodes), std::move(errors)};
      }
    };
  }

  auto primaryExpr() -> UnitParser {
    return UnitParser{
      [](CParsingDriver &driver) -> UnitParser::TReturn {
        auto [nodes, errors] = any(
          integerExpr(), floatExpr(), booleanExpr(), stringExpr(), variableIdExpr(),
          arrayExpr(), tupleExpr(), lambdaExpr()
        )(driver);

        return {std::move(nodes), std::move(errors)};
      }
    };
  }

  auto postfixExpr() -> UnitParser {
    return UnitParser{
      [](CParsingDriver &driver) -> UnitParser::TReturn {
        auto [nodes, errors] = any(
          fnCallExpr(), subscriptExpr(), accessExpr()
        )(driver);

        return {std::move(nodes), std::move(errors)};
      }
    };
  }

  auto prefixExpr() -> UnitParser {
    return UnitParser{
      [](CParsingDriver &driver) -> UnitParser::TReturn {
        auto [nodes, errors] = any(
          match(EToken::Exclaim, EToken::Tilde, EToken::Plus,
                EToken::Minus, EToken::Hash, EToken::Dot3),
          postfixExpr()
        )(driver);

        return {std::move(nodes), std::move(errors)};
      }
    };
  }

  auto exponentialExpr() -> UnitParser {
    return UnitParser{
      [](CParsingDriver &driver) -> UnitParser::TReturn {
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

  auto multiplicativeExpr() -> UnitParser {
    return UnitParser{
      [](CParsingDriver &driver) -> UnitParser::TReturn {
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

  auto additiveExpr() -> UnitParser {
    return UnitParser{
      [](CParsingDriver &driver) -> UnitParser::TReturn {
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

  auto shiftExpr() -> UnitParser {
    return UnitParser{
      [](CParsingDriver &driver) -> UnitParser::TReturn {
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

  auto relationalExpr() -> UnitParser {
    return UnitParser{
      [](CParsingDriver &driver) -> UnitParser::TReturn {
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

  auto equalityExpr() -> UnitParser {
    return UnitParser{
      [](CParsingDriver &driver) -> UnitParser::TReturn {
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

  auto andExpr() -> UnitParser {
    return UnitParser{
      [](CParsingDriver &driver) -> UnitParser::TReturn {
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

  auto exclusiveOrExpr() -> UnitParser {
    return UnitParser{
      [](CParsingDriver &driver) -> UnitParser::TReturn {
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

  auto inclusiveOrExpr() -> UnitParser {
    return UnitParser{
      [](CParsingDriver &driver) -> UnitParser::TReturn {
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

  auto logicalAndExpr() -> UnitParser {
    return UnitParser{
      [](CParsingDriver &driver) -> UnitParser::TReturn {
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

  auto logicalOrExpr() -> UnitParser {
    return UnitParser{
      [](CParsingDriver &driver) -> UnitParser::TReturn {
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

  auto nullCoalescingExpr() -> UnitParser {
    return UnitParser{
      [](CParsingDriver &driver) -> UnitParser::TReturn {
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

  auto sequenceExpr() -> UnitParser {
    return UnitParser{
      [](CParsingDriver &driver) -> UnitParser::TReturn {
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

  auto ternaryExpr() -> UnitParser {
    return UnitParser{
      [](CParsingDriver &driver) -> UnitParser::TReturn {
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

  auto expr() -> UnitParser {
    return UnitParser{
      [](CParsingDriver &driver) -> UnitParser::TReturn {
        return ternaryExpr()(driver);
      }
    };
  }

  auto identifierDecl() -> UnitParser {
    return UnitParser{
      [](CParsingDriver &driver) -> UnitParser::TReturn {
        auto [nodes, errors] = any(
          sequence(variableIdExpr(), maybe(sequence(match(EToken::Colon), typeExpr()))),
          typeExpr()
        )(driver);

        return {std::move(nodes), std::move(errors)};
      }
    };
  }

  auto tupleDecl() -> UnitParser {
    return UnitParser{
      [](CParsingDriver &driver) -> UnitParser::TReturn {
        auto [nodes, errors] = sequence(
          match(EToken::LeftParen), list(identifierDecl()), match(EToken::RightParen)
        )(driver);

        return {std::move(nodes), std::move(errors)};
      }
    };
  }

  auto stmt() -> UnitParser {
  }

  auto translationUnit() -> UnitParser {
  }
}
