#include "parse.hpp"

namespace tlc::parse {
    auto Parse::handleType(syntax::OpPrecedence const minP) -> ParseResult {
        auto lhsContext = enter(Context::Type);

        auto lhs = [this] {
            if (auto infer = handleTypeInfer(); infer) {
                return infer;
            }
            if (auto tuple = handleTypeTuple(); tuple) {
                return tuple;
            }
            return handleTypeIdentifier();
        }();
        if (!lhs) {
            return {};
        }

        auto rhsContext = enter(Context::Type);
        while (true) {
            if (auto array = handleArrayExpr(); array) {
                lhs = syntax::type::Array{
                    *lhs, std::move(*array), lhsContext.location()
                };
                continue;
            }
            if (auto genericArgs = handleGenericArguments(); genericArgs) {
                lhs = syntax::type::Generic{
                    *lhs, std::move(*genericArgs), lhsContext.location()
                };
                continue;
            }
            if (rhsContext.stream().match(lexeme::minusGreater)) {
                rhsContext.to(Context::FunctionType);

                auto fnResultType = handleType()
                    .value_or(syntax::RequiredButMissing{});
                rhsContext.emitIfNodeEmpty(fnResultType, Reason::MissingType);

                lhs = syntax::type::Function{
                    *lhs, std::move(fnResultType), lhsContext.location()
                };
                continue;
            }
            if (rhsContext.stream().match(syntax::isBinaryTypeOperator)) {
                rhsContext.to(Context::BinaryTypeExpr);

                auto const op = rhsContext.stream().current().lexeme();
                auto const p = syntax::opPrecedence(
                    op, syntax::EOperator::Binary
                );

                if (rhsContext.backtrackIf(p <= minP)) {
                    break;
                }

                auto rhs = handleType(
                    syntax::isLeftAssociative(op) ? p + 1 : p
                ).value_or(syntax::RequiredButMissing{});
                rhsContext.emitIfNodeEmpty(rhs, Reason::MissingType);

                lhs = syntax::type::Binary{
                    *lhs, op, std::move(rhs), lhsContext.location()
                };
                continue;
            }
            break;
        }

        return lhs;
    }

    auto Parse::handleTypeIdentifier() -> ParseResult {
        auto context = enter(Context::TypeIdentifier);

        auto const constant = !context.stream().match(lexeme::dollar);

        Vec<Str> fragments;
        while (context.stream().match(lexeme::identifier)) {
            fragments.push_back(context.stream().current().str());
            if (!context.stream().match(lexeme::dot)) {
                break;
            }
        }
        if (context.backtrackIf(!context.stream().match(
            lexeme::fundamentalType, lexeme::userDefinedType))) {
            return {};
        }

        fragments.push_back(context.stream().current().str());
        return syntax::type::Identifier{
            constant, std::move(fragments),
            context.stream().current().lexeme() == lexeme::fundamentalType,
            context.location()
        };
    }

    auto Parse::handleTypeTuple() -> ParseResult {
        auto context = enter(Context::TupleType);

        if (!context.stream().match(lexeme::leftParen)) {
            return {};
        }
        if (context.stream().match(lexeme::rightParen)) {
            return syntax::type::Tuple{{}, context.location()};
        }

        Vec<syntax::Node> types;
        do {
            auto type = handleType().value_or(syntax::RequiredButMissing{});
            context.emitIfNodeEmpty(type, Reason::MissingType);
            types.push_back(std::move(type));
        }
        while (context.stream().match(lexeme::comma));

        context.emitIfLexemeNotPresent(
            lexeme::rightParen, Reason::MissingEnclosingSymbol
        );
        return syntax::type::Tuple{std::move(types), context.location()};
    }

    auto Parse::handleTypeInfer() -> ParseResult {
        auto context = enter(Context::TypeInfer);

        if (context.backtrackIf(!context.stream().match(lexeme::leftBracket) ||
            !context.stream().match(lexeme::leftBracket))) {
            return {};
        }

        auto expr = handleExpr().value_or(syntax::RequiredButMissing{});
        context.emitIfNodeEmpty(expr, Reason::MissingExpr);

        context.emitIfLexemeNotPresent(
            lexeme::rightBracket, Reason::MissingEnclosingSymbol
        );
        context.emitIfLexemeNotPresent(
            lexeme::rightBracket, Reason::MissingEnclosingSymbol
        );
        return syntax::type::Infer{std::move(expr), context.location()};
    }

    auto Parse::handleGenericArguments() -> ParseResult {
        auto context = enter(Context::GenericTypeArguments);

        if (!context.stream().match(lexeme::less)) {
            return {};
        }

        Vec<syntax::Node> args;
        do {
            auto type = handleType().value_or(syntax::RequiredButMissing{});
            context.emitIfNodeEmpty(type, Reason::MissingType);
            args.push_back(std::move(type));
        }
        while (context.stream().match(lexeme::comma));

        context.emitIfLexemeNotPresent(
            lexeme::greater, Reason::MissingEnclosingSymbol
        );
        return syntax::type::GenericArguments{
            std::move(args), context.location()
        };
    }
}
