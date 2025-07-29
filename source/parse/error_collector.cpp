#include "error_collector.hpp"

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
            case Context::Tuple: {
                return "Missing ')'.";
            }
            case Context::Array: {
                return "Missing ']'.";
            }
            case Context::TypeInfer: {
                return "Missing ']]'.";
            }
            case Context::Record: {
                return "Missing '}'.";
            }
            default: {
                return "Missing enclosing" + endOfSentenceWithInfo();
            }
            }
        }
        case Reason::MissingExpr: {
            switch (m_context) {
            case Context::Tuple: {
                return "Missing expressions in tuple.";
            }
            case Context::Array: {
                return "Missing expressions in array.";
            }
            default: {
                return "Missing expressions.";
            }
            }
        }
        case Reason::MissingType: {
            switch (m_context) {
            case Context::Tuple: {
                return "Missing type identifiers in tuple.";
            }
            case Context::Array: {
                return "Missing type identifiers in array.";
            }
            default: {
                return "Missing type identifiers.";
            }
            }
        }
        case Reason::MissingId: {
            switch (m_context) {
            case Context::Access: {
                return "Missing identifiers after '.'.";
            }
            default: {
                return "Missing identifiers.";
            }
            }
        }
        case Reason::MissingDecl: {
            switch (m_context) {
            case Context::Tuple: {
                return "Missing declarations in tuple.";
            }
            default: {
                return "Missing declarations.";
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
