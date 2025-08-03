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

        // postfix
        struct FnApp;
        struct Subscript;
        struct Access;

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
    }

    namespace stmt {
        struct Let;
        struct Return;
        struct Preface;
        struct Defer;
        struct MatchCase;
        struct Match;
        struct Loop;
        struct Conditional;
        struct Block;
        struct Assign;
        struct Expression;
    }

    namespace def {}

    struct TranslationUnit;

    using Node = Poly<
        expr::Integer, expr::Float, expr::Boolean, expr::Identifier, expr::Array,
        expr::Tuple, expr::String, expr::FnApp, expr::Subscript, expr::Access,
        expr::Prefix, expr::Binary, expr::RecordEntry, expr::Record,

        type::Identifier, type::Array, type::Tuple, type::Function, type::Infer,
        type::GenericArguments, type::Generic,

        decl::Identifier, decl::Tuple,

        stmt::Let, stmt::Return, stmt::Preface, stmt::Defer, stmt::Loop,
        stmt::Match, stmt::MatchCase, stmt::Conditional, stmt::Block, stmt::Assign,
        stmt::Expression,

        TranslationUnit
    >;
}

#endif // TLC_SYNTAX_FORWARD_HPP
