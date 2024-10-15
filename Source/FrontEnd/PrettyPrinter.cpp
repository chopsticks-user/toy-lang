#include "PrettyPrinter.hpp"

namespace rv = std::ranges::views;

namespace tl::fe {
  auto PrettyPrinter::operator()(const syntax::TranslationUnit &node) -> String {
    Strings results = visitChildren(node);
    return std::accumulate(
             results.begin(), results.end(), std::string(""),
             [](const String &a, const String &b) {
               return a + "\n" + b;
             }) + "\n";
  }

  auto PrettyPrinter::operator()(const syntax::ModuleExpr &node) -> String {
    Strings results = visitChildren(node);
    return "module " + std::accumulate(
             std::next(results.begin()), results.end(), results[0],
             [](const String &a, const String &b) {
               return a + "::" + b;
             }
           ) + ";\n";
  }

  auto PrettyPrinter::operator()(const syntax::ImportExpr &node) -> String {
    Strings results = visitChildren(node);
    return "import " + std::accumulate(
             std::next(results.begin()), results.end(), results[0],
             [](const String &a, const String &b) {
               return a + "::" + b;
             }
           ) + ";\n";
  }

  auto PrettyPrinter::operator()(const syntax::Function &node) -> String {
    Strings results = visitChildren(node);
    auto str = node.storage() + " " + results[0] + ": ";
    return str;
  }

  auto PrettyPrinter::operator()(const syntax::FunctionPrototype &node) -> String {
  }

  auto PrettyPrinter::operator()(const syntax::BlockStatement &node) -> String {
    return "{...}";
  }

  auto PrettyPrinter::operator()(const syntax::Identifier &node) -> String {
    return node.name();
  }

  auto PrettyPrinter::operator()(const syntax::BinaryExpr &node) -> String {
    visitChildren(node);
    return "Visiting BinaryExpr\n";
  }

  auto PrettyPrinter::operator()(const syntax::UnaryExpr &node) -> String {
    visitChildren(node);
    return "Visiting UnaryExpr\n";
  }

  auto PrettyPrinter::operator()(const syntax::IntegerLiteral &node) -> String {
    visitChildren(node);
    return "Visiting IntegerLiteral\n";
  }

  auto PrettyPrinter::operator()(const syntax::FloatLiteral &node) -> String {
    visitChildren(node);
    return "Visiting FloatLiteral\n";
  }

  auto PrettyPrinter::operator()(const syntax::StringLiteral &node) -> String {
    visitChildren(node);
    return "Visiting StringLiteral\n";
  }

  auto PrettyPrinter::operator()(const syntax::BooleanLiteral &node) -> String {
    visitChildren(node);
    return "Visiting BooleanLiteral\n";
  }
}
