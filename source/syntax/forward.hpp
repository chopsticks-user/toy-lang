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
        // struct String;
        struct Array;
        struct Tuple;

        // postfix
        struct FnApp;
        struct Subscript;
        struct Access;

        struct Prefix;
        struct Binary;
        // struct Ternary;

        struct Record;
    }

    namespace type {
        struct Identifier;
        struct Array;
        struct Tuple;
        struct Function;
        struct Infer;
        struct Sum;
        struct Product;
    }

    namespace decl {
        struct Identifier;
        struct Tuple;
    }

    namespace stmt {
        struct Let;
        struct Return;
        struct Yield;
        struct Preface;
        struct Defer;
        struct Match;
        struct Loop;
        struct Cond;
        struct Block;
        struct Assign;
        struct Expr;
    }

    namespace def {}

    struct TranslationUnit;

    using Node = Poly<
        expr::Integer, expr::Float, expr::Boolean, expr::Identifier, expr::Array,
        expr::Tuple, expr::FnApp, expr::Subscript, expr::Access, expr::Prefix,
        expr::Binary, expr::Record,

        type::Identifier, type::Array, type::Tuple, type::Function, type::Infer,
        decl::Identifier, decl::Tuple,

        stmt::Let, stmt::Return, stmt::Yield, stmt::Preface, stmt::Defer, stmt::Match,
        stmt::Loop, stmt::Cond, stmt::Block, stmt::Assign, stmt::Expr,

        TranslationUnit
    >;
}

#endif // TLC_SYNTAX_FORWARD_HPP
