#ifndef TLC_PARSE_ERROR_HPP
#define TLC_PARSE_ERROR_HPP

#include "core/core.hpp"
#include "syntax/syntax.hpp"
#include "lex/lex.hpp"

namespace tlc::parse {
    enum class EParseErrorContext {
        Unknown, Tuple, Array, Access, TypeInfer, Record, IdDecl, TupleDecl,
        Stmt, LetStmt, BlockStmt, PrefaceStmt, DeferStmt, AssignStmt,
        ExprStmt, CondStmt, YieldStmt, LoopStmt, MatchStmt, MatchCaseStmt,
        MatchCaseDefaultStmt,
    };

    enum class EParseErrorReason {
        NotAnError, MissingSymbol, MissingKeyword, MissingEnclosingSymbol,
        MissingExpr, MissingType, MissingId, MissingDecl, MissingStmt,
        MissingBody, Unknown,
    };
}

#endif // TLC_PARSE_ERROR_HPP
