#ifndef TLC_PARSE_ERROR_HPP
#define TLC_PARSE_ERROR_HPP

#include "core/core.hpp"
#include "syntax/syntax.hpp"
#include "lex/lex.hpp"

namespace tlc::parse {
    enum class EParseErrorContext {
        Unknown,

        LiteralExpr, IdentifierExpr, TupleExpr, ArrayExpr, RecordExpr,
        RecordEntryExpr, TryExpr, Expr, PrefixExpr, PrimaryExpr, BinaryExpr,
        StringExpr,

        Type, FunctionType, TypeIdentifier, TupleType, TypeInfer,
        BinaryTypeExpr, GenericTypeArguments,

        IdDecl, TupleDecl, GenericParamsDecl,

        DeclStmt, ReturnStmt, Stmt, LetStmt, BlockStmt, PrefaceStmt, DeferStmt,
        AssignStmt, ExprStmt, CondStmt, YieldStmt, LoopStmt, MatchStmt,
        MatchCaseStmt, MatchCaseDefaultStmt,

        TranslationUnit, ModuleDecl, ImportDecl, FunctionPrototype, Function,
    };

    enum class EParseErrorReason {
        NotAnError, MissingSymbol, MissingKeyword, MissingEnclosingSymbol,
        MissingExpr, MissingType, MissingId, MissingDecl, MissingStmt,
        MissingBody, RestrictedAction, Unknown,
    };
}

#endif // TLC_PARSE_ERROR_HPP
