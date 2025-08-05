// ReSharper disable CppIncompleteSwitchStatement
// ReSharper disable CppDefaultCaseNotHandledInSwitchStatement
#include "parse_error.hpp"

namespace tlc {
    template <>
    auto Error<parse::EParseErrorContext, parse::EParseErrorReason>::
    message() const -> Str {
        using parse::EParseErrorContext;
        using parse::EParseErrorReason;

        switch (m_params.context) {
        case EParseErrorContext::Unknown: {
            switch (m_params.reason) {
            case EParseErrorReason::NotAnError: {}
            case EParseErrorReason::MissingSymbol: {}
            case EParseErrorReason::MissingKeyword: {}
            case EParseErrorReason::MissingEnclosingSymbol: {}
            case EParseErrorReason::MissingExpr: {}
            case EParseErrorReason::MissingType: {}
            case EParseErrorReason::MissingId: {}
            case EParseErrorReason::MissingDecl: {}
            case EParseErrorReason::MissingStmt: {}
            case EParseErrorReason::MissingBody: {}
            case EParseErrorReason::Unknown: {}
            }
            break;
        }
        case EParseErrorContext::Tuple: {
            switch (m_params.reason) {
            case EParseErrorReason::NotAnError: {}
            case EParseErrorReason::MissingSymbol: {}
            case EParseErrorReason::MissingKeyword: {}
            case EParseErrorReason::MissingEnclosingSymbol: {}
            case EParseErrorReason::MissingExpr: {}
            case EParseErrorReason::MissingType: {}
            case EParseErrorReason::MissingId: {}
            case EParseErrorReason::MissingDecl: {}
            case EParseErrorReason::MissingStmt: {}
            case EParseErrorReason::MissingBody: {}
            case EParseErrorReason::Unknown: {}
            }
            break;
        }
        case EParseErrorContext::Array: {
            switch (m_params.reason) {
            case EParseErrorReason::NotAnError: {}
            case EParseErrorReason::MissingSymbol: {}
            case EParseErrorReason::MissingKeyword: {}
            case EParseErrorReason::MissingEnclosingSymbol: {}
            case EParseErrorReason::MissingExpr: {}
            case EParseErrorReason::MissingType: {}
            case EParseErrorReason::MissingId: {}
            case EParseErrorReason::MissingDecl: {}
            case EParseErrorReason::MissingStmt: {}
            case EParseErrorReason::MissingBody: {}
            case EParseErrorReason::Unknown: {}
            }
            break;
        }
        case EParseErrorContext::Access: {
            switch (m_params.reason) {
            case EParseErrorReason::NotAnError: {}
            case EParseErrorReason::MissingSymbol: {}
            case EParseErrorReason::MissingKeyword: {}
            case EParseErrorReason::MissingEnclosingSymbol: {}
            case EParseErrorReason::MissingExpr: {}
            case EParseErrorReason::MissingType: {}
            case EParseErrorReason::MissingId: {}
            case EParseErrorReason::MissingDecl: {}
            case EParseErrorReason::MissingStmt: {}
            case EParseErrorReason::MissingBody: {}
            case EParseErrorReason::Unknown: {}
            }
            break;
        }
        case EParseErrorContext::TypeInfer: {
            switch (m_params.reason) {
            case EParseErrorReason::NotAnError: {}
            case EParseErrorReason::MissingSymbol: {}
            case EParseErrorReason::MissingKeyword: {}
            case EParseErrorReason::MissingEnclosingSymbol: {}
            case EParseErrorReason::MissingExpr: {}
            case EParseErrorReason::MissingType: {}
            case EParseErrorReason::MissingId: {}
            case EParseErrorReason::MissingDecl: {}
            case EParseErrorReason::MissingStmt: {}
            case EParseErrorReason::MissingBody: {}
            case EParseErrorReason::Unknown: {}
            }
            break;
        }
        case EParseErrorContext::Record: {
            switch (m_params.reason) {
            case EParseErrorReason::NotAnError: {}
            case EParseErrorReason::MissingSymbol: {}
            case EParseErrorReason::MissingKeyword: {}
            case EParseErrorReason::MissingEnclosingSymbol: {}
            case EParseErrorReason::MissingExpr: {}
            case EParseErrorReason::MissingType: {}
            case EParseErrorReason::MissingId: {}
            case EParseErrorReason::MissingDecl: {}
            case EParseErrorReason::MissingStmt: {}
            case EParseErrorReason::MissingBody: {}
            case EParseErrorReason::Unknown: {}
            }
            break;
        }
        case EParseErrorContext::IdDecl: {
            switch (m_params.reason) {
            case EParseErrorReason::NotAnError: {}
            case EParseErrorReason::MissingSymbol: {}
            case EParseErrorReason::MissingKeyword: {}
            case EParseErrorReason::MissingEnclosingSymbol: {}
            case EParseErrorReason::MissingExpr: {}
            case EParseErrorReason::MissingType: {}
            case EParseErrorReason::MissingId: {}
            case EParseErrorReason::MissingDecl: {}
            case EParseErrorReason::MissingStmt: {}
            case EParseErrorReason::MissingBody: {}
            case EParseErrorReason::Unknown: {}
            }
            break;
        }
        case EParseErrorContext::TupleDecl: {
            switch (m_params.reason) {
            case EParseErrorReason::NotAnError: {}
            case EParseErrorReason::MissingSymbol: {}
            case EParseErrorReason::MissingKeyword: {}
            case EParseErrorReason::MissingEnclosingSymbol: {}
            case EParseErrorReason::MissingExpr: {}
            case EParseErrorReason::MissingType: {}
            case EParseErrorReason::MissingId: {}
            case EParseErrorReason::MissingDecl: {}
            case EParseErrorReason::MissingStmt: {}
            case EParseErrorReason::MissingBody: {}
            case EParseErrorReason::Unknown: {}
            }
            break;
        }
        case EParseErrorContext::Stmt: {
            switch (m_params.reason) {
            case EParseErrorReason::NotAnError: {}
            case EParseErrorReason::MissingSymbol: {}
            case EParseErrorReason::MissingKeyword: {}
            case EParseErrorReason::MissingEnclosingSymbol: {}
            case EParseErrorReason::MissingExpr: {}
            case EParseErrorReason::MissingType: {}
            case EParseErrorReason::MissingId: {}
            case EParseErrorReason::MissingDecl: {}
            case EParseErrorReason::MissingStmt: {}
            case EParseErrorReason::MissingBody: {}
            case EParseErrorReason::Unknown: {}
            }
            break;
        }
        case EParseErrorContext::LetStmt: {
            switch (m_params.reason) {
            case EParseErrorReason::NotAnError: {}
            case EParseErrorReason::MissingSymbol: {}
            case EParseErrorReason::MissingKeyword: {}
            case EParseErrorReason::MissingEnclosingSymbol: {}
            case EParseErrorReason::MissingExpr: {}
            case EParseErrorReason::MissingType: {}
            case EParseErrorReason::MissingId: {}
            case EParseErrorReason::MissingDecl: {}
            case EParseErrorReason::MissingStmt: {}
            case EParseErrorReason::MissingBody: {}
            case EParseErrorReason::Unknown: {}
            }
            break;
        }
        case EParseErrorContext::BlockStmt: {
            switch (m_params.reason) {
            case EParseErrorReason::NotAnError: {}
            case EParseErrorReason::MissingSymbol: {}
            case EParseErrorReason::MissingKeyword: {}
            case EParseErrorReason::MissingEnclosingSymbol: {}
            case EParseErrorReason::MissingExpr: {}
            case EParseErrorReason::MissingType: {}
            case EParseErrorReason::MissingId: {}
            case EParseErrorReason::MissingDecl: {}
            case EParseErrorReason::MissingStmt: {}
            case EParseErrorReason::MissingBody: {}
            case EParseErrorReason::Unknown: {}
            }
            break;
        }
        case EParseErrorContext::PrefaceStmt: {
            switch (m_params.reason) {
            case EParseErrorReason::NotAnError: {}
            case EParseErrorReason::MissingSymbol: {}
            case EParseErrorReason::MissingKeyword: {}
            case EParseErrorReason::MissingEnclosingSymbol: {}
            case EParseErrorReason::MissingExpr: {}
            case EParseErrorReason::MissingType: {}
            case EParseErrorReason::MissingId: {}
            case EParseErrorReason::MissingDecl: {}
            case EParseErrorReason::MissingStmt: {}
            case EParseErrorReason::MissingBody: {}
            case EParseErrorReason::Unknown: {}
            }
            break;
        }
        case EParseErrorContext::DeferStmt: {
            switch (m_params.reason) {
            case EParseErrorReason::NotAnError: {}
            case EParseErrorReason::MissingSymbol: {}
            case EParseErrorReason::MissingKeyword: {}
            case EParseErrorReason::MissingEnclosingSymbol: {}
            case EParseErrorReason::MissingExpr: {}
            case EParseErrorReason::MissingType: {}
            case EParseErrorReason::MissingId: {}
            case EParseErrorReason::MissingDecl: {}
            case EParseErrorReason::MissingStmt: {}
            case EParseErrorReason::MissingBody: {}
            case EParseErrorReason::Unknown: {}
            }
            break;
        }
        case EParseErrorContext::AssignStmt: {
            switch (m_params.reason) {
            case EParseErrorReason::NotAnError: {}
            case EParseErrorReason::MissingSymbol: {}
            case EParseErrorReason::MissingKeyword: {}
            case EParseErrorReason::MissingEnclosingSymbol: {}
            case EParseErrorReason::MissingExpr: {}
            case EParseErrorReason::MissingType: {}
            case EParseErrorReason::MissingId: {}
            case EParseErrorReason::MissingDecl: {}
            case EParseErrorReason::MissingStmt: {}
            case EParseErrorReason::MissingBody: {}
            case EParseErrorReason::Unknown: {}
            }
            break;
        }
        case EParseErrorContext::ExprStmt: {
            switch (m_params.reason) {
            case EParseErrorReason::NotAnError: {}
            case EParseErrorReason::MissingSymbol: {}
            case EParseErrorReason::MissingKeyword: {}
            case EParseErrorReason::MissingEnclosingSymbol: {}
            case EParseErrorReason::MissingExpr: {}
            case EParseErrorReason::MissingType: {}
            case EParseErrorReason::MissingId: {}
            case EParseErrorReason::MissingDecl: {}
            case EParseErrorReason::MissingStmt: {}
            case EParseErrorReason::MissingBody: {}
            case EParseErrorReason::Unknown: {}
            }
            break;
        }
        case EParseErrorContext::CondStmt: {
            switch (m_params.reason) {
            case EParseErrorReason::NotAnError: {}
            case EParseErrorReason::MissingSymbol: {}
            case EParseErrorReason::MissingKeyword: {}
            case EParseErrorReason::MissingEnclosingSymbol: {}
            case EParseErrorReason::MissingExpr: {}
            case EParseErrorReason::MissingType: {}
            case EParseErrorReason::MissingId: {}
            case EParseErrorReason::MissingDecl: {}
            case EParseErrorReason::MissingStmt: {}
            case EParseErrorReason::MissingBody: {}
            case EParseErrorReason::Unknown: {}
            }
            break;
        }
        case EParseErrorContext::YieldStmt: {
            switch (m_params.reason) {
            case EParseErrorReason::NotAnError: {}
            case EParseErrorReason::MissingSymbol: {}
            case EParseErrorReason::MissingKeyword: {}
            case EParseErrorReason::MissingEnclosingSymbol: {}
            case EParseErrorReason::MissingExpr: {}
            case EParseErrorReason::MissingType: {}
            case EParseErrorReason::MissingId: {}
            case EParseErrorReason::MissingDecl: {}
            case EParseErrorReason::MissingStmt: {}
            case EParseErrorReason::MissingBody: {}
            case EParseErrorReason::Unknown: {}
            }
            break;
        }
        case EParseErrorContext::LoopStmt: {
            switch (m_params.reason) {
            case EParseErrorReason::NotAnError: {}
            case EParseErrorReason::MissingSymbol: {}
            case EParseErrorReason::MissingKeyword: {}
            case EParseErrorReason::MissingEnclosingSymbol: {}
            case EParseErrorReason::MissingExpr: {}
            case EParseErrorReason::MissingType: {}
            case EParseErrorReason::MissingId: {}
            case EParseErrorReason::MissingDecl: {}
            case EParseErrorReason::MissingStmt: {}
            case EParseErrorReason::MissingBody: {}
            case EParseErrorReason::Unknown: {}
            }
            break;
        }
        case EParseErrorContext::MatchStmt: {
            switch (m_params.reason) {
            case EParseErrorReason::NotAnError: {}
            case EParseErrorReason::MissingSymbol: {}
            case EParseErrorReason::MissingKeyword: {}
            case EParseErrorReason::MissingEnclosingSymbol: {}
            case EParseErrorReason::MissingExpr: {}
            case EParseErrorReason::MissingType: {}
            case EParseErrorReason::MissingId: {}
            case EParseErrorReason::MissingDecl: {}
            case EParseErrorReason::MissingStmt: {}
            case EParseErrorReason::MissingBody: {}
            case EParseErrorReason::Unknown: {}
            }
            break;
        }
        case EParseErrorContext::MatchCaseStmt: {
            switch (m_params.reason) {
            case EParseErrorReason::NotAnError: {}
            case EParseErrorReason::MissingSymbol: {}
            case EParseErrorReason::MissingKeyword: {}
            case EParseErrorReason::MissingEnclosingSymbol: {}
            case EParseErrorReason::MissingExpr: {}
            case EParseErrorReason::MissingType: {}
            case EParseErrorReason::MissingId: {}
            case EParseErrorReason::MissingDecl: {}
            case EParseErrorReason::MissingStmt: {}
            case EParseErrorReason::MissingBody: {}
            case EParseErrorReason::Unknown: {}
            }
            break;
        }
        case EParseErrorContext::MatchCaseDefaultStmt: {
            switch (m_params.reason) {
            case EParseErrorReason::NotAnError: {}
            case EParseErrorReason::MissingSymbol: {}
            case EParseErrorReason::MissingKeyword: {}
            case EParseErrorReason::MissingEnclosingSymbol: {}
            case EParseErrorReason::MissingExpr: {}
            case EParseErrorReason::MissingType: {}
            case EParseErrorReason::MissingId: {}
            case EParseErrorReason::MissingDecl: {}
            case EParseErrorReason::MissingStmt: {}
            case EParseErrorReason::MissingBody: {}
            case EParseErrorReason::Unknown: {}
            }
            break;
        }
        }
        return "";
    }
}
