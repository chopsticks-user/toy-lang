#ifndef TLC_SYNTAX_FORWARD_HPP
#define TLC_SYNTAX_FORWARD_HPP

#include "core/core.hpp"

namespace tlc::syntax {
    namespace expr {
        // primary
        struct Integer;
        struct Float;
        struct Boolean;
        struct Identifier;
        struct String;
        struct Array;
        struct Tuple;
        struct RecordEntry;
        struct Record;
        struct Try;

        // postfix
        struct FnApp;
        struct Subscript;

        struct Prefix;
        struct Binary;
        // struct Ternary;
    }

    namespace type {
        struct Identifier;
        struct Array;
        struct Tuple;
        struct Function;
        struct Infer;
        struct GenericArguments;
        struct Generic;
    }

    namespace decl {
        struct Identifier;
        struct Tuple;
        struct GenericIdentifier;
        struct GenericParameters;
    }

    namespace stmt {
        struct Decl;
        struct Return;
        struct Defer;
        struct MatchCase;
        struct Match;
        struct Loop;
        struct Conditional;
        struct Block;
        struct Assign;
        struct Expression;
    }

    namespace global {
        struct ModuleDecl;
        struct ImportDecl;
        struct FunctionPrototype;
        struct Function;
    }

    struct TranslationUnit;

    using Node = Poly<
        expr::Integer, expr::Float, expr::Boolean, expr::Identifier, expr::Array,
        expr::Tuple, expr::String, expr::FnApp, expr::Subscript, expr::Prefix,
        expr::Binary, expr::RecordEntry, expr::Record, expr::Try,

        type::Identifier, type::Array, type::Tuple, type::Function, type::Infer,
        type::GenericArguments, type::Generic,

        decl::Identifier, decl::Tuple, decl::GenericIdentifier,
        decl::GenericParameters,

        stmt::Decl, stmt::Return, stmt::Defer, stmt::Loop, stmt::Match,
        stmt::MatchCase, stmt::Conditional, stmt::Block, stmt::Assign,
        stmt::Expression,

        global::ModuleDecl, global::ImportDecl, global::FunctionPrototype,
        global::Function,

        TranslationUnit
    >;
}

#endif // TLC_SYNTAX_FORWARD_HPP
