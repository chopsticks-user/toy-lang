export module parse:handle_type;

import :unit_fwd;

namespace tlc::parse {
    auto handleType(Context context) -> Opt<syntax::Node> {
        auto lhs = [&context] {
            if (auto infer = handleTypeInfer(
                Context::enter(Context::TypeInfer, context)); infer) {
                return infer;
            }
            if (auto tuple = handleTypeTuple(
                Context::enter(Context::TupleType, context)); tuple) {
                return tuple;
            }
            return handleTypeIdentifier(
                Context::enter(Context::TypeIdentifier, context));
        }();
        if (!lhs) {
            return {};
        }

        auto rhsContext = Context::enter(Context::Type, context);
        while (true) {
            if (auto array = handleArrayExpr(
                Context::enter(Context::ArrayExpr, context)); array) {
                lhs = syntax::type::Array{
                    *lhs, std::move(*array), context.location()
                };
                continue;
            }
            if (auto genericArgs = handleGenericArguments(
                Context::enter(Context::GenericTypeArguments, context)); genericArgs) {
                lhs = syntax::type::Generic{
                    *lhs, std::move(*genericArgs), context.location()
                };
                continue;
            }
            if (rhsContext.stream().match(lexeme::minusGreater)) {
                rhsContext.to(Context::FunctionType);

                auto fnResultType = handleType(Context::enter(Context::Type, context))
                    .value_or(syntax::RequiredButMissing{});
                rhsContext.emitIfNodeMissing(fnResultType, Reason::MissingType);

                lhs = syntax::type::Function{
                    *lhs, std::move(fnResultType), context.location()
                };
                continue;
            }
            if (rhsContext.stream().match(syntax::isBinaryTypeOperator)) {
                rhsContext.to(Context::BinaryTypeExpr);

                auto const op = rhsContext.stream().current().lexeme();
                auto const p = syntax::opPrecedence(
                    op, syntax::EOperator::Binary
                );

                if (rhsContext.backtrackIf(p <= context.minPrecedence())) {
                    break;
                }

                auto rhs = handleType(
                    Context::enter(Context::Type, context,
                                   syntax::isLeftAssociative(op) ? p + 1 : p)
                ).value_or(syntax::RequiredButMissing{});
                rhsContext.emitIfNodeMissing(rhs, Reason::MissingType);

                lhs = syntax::type::Binary{
                    *lhs, op, std::move(rhs), context.location()
                };
                continue;
            }
            break;
        }

        return lhs;
    }

    auto handleTypeIdentifier(Context context) -> Opt<syntax::Node> {
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

    auto handleTypeTuple(Context context) -> Opt<syntax::Node> {
        if (!context.stream().match(lexeme::leftParen)) {
            return {};
        }
        if (context.stream().match(lexeme::rightParen)) {
            return syntax::type::Tuple{{}, context.location()};
        }

        Vec<syntax::Node> types;
        do {
            auto type = handleType(Context::enter(Context::Type, context))
                .value_or(syntax::RequiredButMissing{});
            context.emitIfNodeMissing(type, Reason::MissingType);
            types.push_back(std::move(type));
        }
        while (context.stream().match(lexeme::comma));

        context.emitIfLexemeNotPresent(
            lexeme::rightParen, Reason::MissingEnclosingSymbol
        );
        return syntax::type::Tuple{std::move(types), context.location()};
    }

    auto handleTypeInfer(Context context) -> Opt<syntax::Node> {
        if (context.backtrackIf(!context.stream().match(lexeme::leftBracket) ||
            !context.stream().match(lexeme::leftBracket))) {
            return {};
        }

        auto expr = handleExpr(Context::enter(Context::Expr, context))
            .value_or(syntax::RequiredButMissing{});
        context.emitIfNodeMissing(expr, Reason::MissingExpr);

        context.emitIfLexemeNotPresent(
            lexeme::rightBracket, Reason::MissingEnclosingSymbol
        );
        context.emitIfLexemeNotPresent(
            lexeme::rightBracket, Reason::MissingEnclosingSymbol
        );
        return syntax::type::Infer{std::move(expr), context.location()};
    }

    auto handleGenericArguments(Context context) -> Opt<syntax::Node> {
        if (!context.stream().match(lexeme::less)) {
            return {};
        }

        Vec<syntax::Node> args;
        do {
            auto type = handleType(Context::enter(Context::Type, context))
                .value_or(syntax::RequiredButMissing{});
            context.emitIfNodeMissing(type, Reason::MissingType);
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
