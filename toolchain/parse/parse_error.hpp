#ifndef TLC_PARSE_STATIC_ERROR_HPP
#define TLC_PARSE_STATIC_ERROR_HPP

namespace tlc::parse {
    enum class EContext {
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

    enum class EReason {
        NotAnError, MissingSymbol, MissingKeyword, MissingEnclosingSymbol,
        MissingExpr, MissingTypeExpr, MissingTypeId, MissingId, MissingDecl,
        MissingStmt, MissingBody, RestrictedAction, Unknown,
    };
}

#endif // TLC_PARSE_STATIC_ERROR_HPP
