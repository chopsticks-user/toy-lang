#ifndef  TOYLANG_PARSER_PRETTY_PRINTER_HPP
#define  TOYLANG_PARSER_PRETTY_PRINTER_HPP

#include "AST.hpp"

#include "Core/Core.hpp"

namespace tl::parser {
  class PrettyPrinter : public Visitor<
        PrettyPrinter, ast::BinaryExpr, ast::UnaryExpr, ast::Number, ast::Identifier
      > {
  public:
    auto operator()(const ast::BinaryExpr &node) -> std::string;

    auto operator()(const ast::UnaryExpr &node) -> std::string;

    auto operator()(const ast::Number &node) -> std::string;

    auto operator()(const ast::Identifier &node) -> std::string;

  private:
    std::string result;
  };
}

#endif // TOYLANG_PARSER_PRETTY_PRINTER_HPP
