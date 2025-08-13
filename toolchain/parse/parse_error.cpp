export module parse:error;

import core;

export namespace tlc {
    namespace parse {
        enum class EParseErrorContext {
            Unknown,

            LiteralExpr, IdentifierExpr, TupleExpr, ArrayExpr, RecordExpr,
            RecordEntryExpr, TryExpr, Expr, PrefixExpr, PrimaryExpr, BinaryExpr,
            StringExpr,

            Type, FunctionType, TypeIdentifier, TupleType, TypeInfer,
            BinaryTypeExpr, GenericTypeArguments,

            Decl, IdDecl, TupleDecl, GenericParamsDecl,

            DeclStmt, ReturnStmt, Stmt, LetStmt, BlockStmt, PrefaceStmt, DeferStmt,
            AssignStmt, ExprStmt, CondStmt, YieldStmt, LoopStmt, MatchStmt,
            MatchCaseStmt, MatchCaseDefaultStmt,

            TranslationUnit, ModuleDecl, ImportDeclGroup, ImportDecl,
            FunctionPrototype, Function,
        };

        enum class EParseErrorReason {
            NotAnError, MissingSymbol, MissingKeyword, MissingEnclosingSymbol,
            MissingExpr, MissingType, MissingId, MissingDecl, MissingStmt,
            MissingBody, RestrictedAction, Unknown,
        };

        using Reason = EParseErrorReason;
    }

    // todo: error: declaration of 'message' in module parse:error follows declaration in the global module
    // template <>
    // auto Error<parse::EParseErrorContext, parse::EParseErrorReason>::
    // message() const -> Str {
    //     using parse::EParseErrorContext;
    //     using parse::EParseErrorReason;
    //     return "";
    // }
}
