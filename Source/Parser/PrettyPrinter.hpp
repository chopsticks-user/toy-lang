#ifndef  TOYLANG_PARSER_PRETTY_PRINTER_HPP
#define  TOYLANG_PARSER_PRETTY_PRINTER_HPP

#include "Syntax/Syntax.hpp"

#include "Core/Core.hpp"

namespace tl::parser {
  class PrettyPrinter : public Visitor<
        PrettyPrinter, syntax::BinaryExpr, syntax::UnaryExpr, syntax::Number, syntax::Identifier,
        syntax::StringLiteral
      > {
  public:
    auto operator()(const syntax::BinaryExpr &node) -> std::string;

    auto operator()(const syntax::UnaryExpr &node) -> std::string;

    auto operator()(const syntax::Number &node) -> std::string;

    auto operator()(const syntax::Identifier &node) -> std::string;

    auto operator()(const syntax::StringLiteral &node) -> std::string;

  private:
    std::string result;
  };
}

#endif // TOYLANG_PARSER_PRETTY_PRINTER_HPP
