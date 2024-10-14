#include "PrettyPrinter.hpp"

namespace tl::fe {
  auto PrettyPrinter::operator()(const syntax::Identifier &node) -> std::string {
    visitChildren(node);
    return result += "Visiting Identifier\n";
  }

  auto PrettyPrinter::operator()(const syntax::BinaryExpr &node) -> std::string {
    visitChildren(node);
    return result += "Visiting BinaryExpr\n";
  }

  auto PrettyPrinter::operator()(const syntax::UnaryExpr &node) -> std::string {
    visitChildren(node);
    return result += "Visiting UnaryExpr\n";
  }

  auto PrettyPrinter::operator()(const syntax::IntegerLiteral &node) -> std::string {
    visitChildren(node);
    return result += "Visiting IntegerLiteral\n";
  }

  auto PrettyPrinter::operator()(const syntax::FloatLiteral &node) -> std::string {
    visitChildren(node);
    return result += "Visiting FloatLiteral\n";
  }

  auto PrettyPrinter::operator()(const syntax::StringLiteral &node) -> std::string {
    visitChildren(node);
    return result += "Visiting StringLiteral\n";
  }

  auto PrettyPrinter::operator()(const syntax::BooleanLiteral &node) -> std::string {
    visitChildren(node);
    return result += "Visiting BooleanLiteral\n";
  }
}
