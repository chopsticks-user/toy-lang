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
            case Context::Tuple: {
                return "Missing a ')'.";
            }
            case Context::Array: {
                return "Missing a ']'.";
            }
            case Context::TypeInfer: {
                return "Missing a ']]'.";
            }
            default: {
                return "Missing enclosing" + endOfSentenceWithInfo();
            }
            }
        }
        case Reason::ExpectedAnExpression: {
            switch (m_context) {
            case Context::Tuple: {
                return "Missing an expression in tuple.";
            }
            case Context::Array: {
                return "Missing an expression in array.";
            }
            default: {
                return "Expected an expression.";
            }
            }
        }
        case Reason::ExpectedAType: {
            switch (m_context) {
            case Context::Tuple: {
                return "Expected a type in tuple.";
            }
            case Context::Array: {
                return "Expected a type in array.";
            }
            default: {
                return "Expected a type.";
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
