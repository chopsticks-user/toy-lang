#ifndef  TOYLANG_FRONTEND_PRETTY_PRINTER_HPP
#define  TOYLANG_FRONTEND_PRETTY_PRINTER_HPP

#include "Syntax/Syntax.hpp"

#include "Core/Core.hpp"

namespace tl::fe {
  class PrettyPrinter : public syntax::SyntaxTreeVisitor<PrettyPrinter, std::string> {
  public:
    using Super::operator();

    auto operator()(const syntax::Identifier &node) -> std::string;

    auto operator()(const syntax::BinaryExpr &node) -> std::string;

    auto operator()(const syntax::UnaryExpr &node) -> std::string;

    auto operator()(const syntax::IntegerLiteral &node) -> std::string;

    auto operator()(const syntax::FloatLiteral &node) -> std::string;

    auto operator()(const syntax::StringLiteral &node) -> std::string;

    auto operator()(const syntax::BooleanLiteral &node) -> std::string;

  private:
    std::string result;
  };
}

#endif // TOYLANG_FRONTEND_PRETTY_PRINTER_HPP
