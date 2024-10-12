#include "PrettyPrinter.hpp"

namespace tl::parser {
  auto PrettyPrinter::operator()(const ast::BinaryExpr &node) -> std::string {
    visitChildren(node);
    return result += "Visiting BinaryExpr\n";
  }

  auto PrettyPrinter::operator()(const ast::UnaryExpr &node) -> std::string {
    visitChildren(node);
    return result += "Visiting UnaryExpr\n";
  }

  auto PrettyPrinter::operator()(const ast::Number &node) -> std::string {
    visitChildren(node);
    return result += "Visiting Number\n";
  }

  auto PrettyPrinter::operator()(const ast::Identifier &node) -> std::string {
    visitChildren(node);
    return result += "Visiting Identifier\n";
  }
}
