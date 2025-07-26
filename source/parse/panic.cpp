#include "panic.hpp"

namespace tlc::parse {
    auto Error::message() const -> Str {
        switch (m_reason) {
        case Reason::NotAnError: {
            return "Not an error.";
        }
        case Reason::MissingSymbol: {
            return "Missing" + endOfSentenceWithInfo();
        }
        case Reason::MissingEnclosingSymbol: {
            switch (m_context) {
            case Context::TupleExpr: {
                return "Missing a ')' at the end of tuple expression.";
            }
            case Context::ArrayExpr: {
                return "Missing a ']' at the end of array expression.";
            }
            default: {
                return "Missing enclosing" + endOfSentenceWithInfo();
            }
            }
        }
        case Reason::NotAnExpression: {
            switch (m_context) {
            case Context::TupleExpr: {
                return "Tuple expression requires a comma-separated list of expressions.";
            }
            case Context::ArrayExpr: {
                return "Array expression requires a comma-separated list of expressions.";
            }
            default: {
                return "Expected an expression.";
            }
            }
        }
        default: {
            // Reason::Unknown
            return "Unknown";
        }
        }
    }
}
