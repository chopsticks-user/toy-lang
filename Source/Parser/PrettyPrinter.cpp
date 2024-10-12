#include "PrettyPrinter.hpp"

namespace tl::parser {
  auto PrettyPrinter::operator()(const syntax::BinaryExpr &node) -> std::string {
    visitChildren(node);
    return result += "Visiting BinaryExpr\n";
  }

  auto PrettyPrinter::operator()(const syntax::UnaryExpr &node) -> std::string {
    visitChildren(node);
    return result += "Visiting UnaryExpr\n";
  }

  auto PrettyPrinter::operator()(const syntax::Number &node) -> std::string {
    visitChildren(node);
    return result += "Visiting Number\n";
  }

  auto PrettyPrinter::operator()(const syntax::Identifier &node) -> std::string {
    visitChildren(node);
    return result += "Visiting Identifier\n";
  }

  auto PrettyPrinter::operator()(const syntax::StringLiteral &node) -> std::string {
    visitChildren(node);
    return result += "Visiting StringLiteral\n";
  }
}
