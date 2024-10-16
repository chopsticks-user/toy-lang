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
    enterScope();
    const Strings results = visitChildren(node);
    return node.storage() + " " + results[0] + ": " + results[1]
           + (node.pure() ? " pure" : "") + " {\n" + results[2];
  }

  auto PrettyPrinter::operator()(const syntax::FunctionPrototype &node) -> String {
    Strings results = visitChildren(node);

    if (results.size() == 1) {
      return "() -> " + results[0];
    }

    return "(" + std::accumulate(
             std::next(results.begin(), 2), results.end(), results[1],
             [](const String &a, const String &b) {
               return a + ", " + b;
             }
           ) + ") -> " + results[0];
  }

  auto PrettyPrinter::operator()(const syntax::ParameterDeclFragment &node) -> String {
    Strings results = visitChildren(node);
    return node.mutibility() + " " + results[0];
  }

  auto PrettyPrinter::operator()(const syntax::IdentifierDeclFragment &node) -> String {
    Strings results = visitChildren(node);
    return results[0] + ": " + results[1];
  }

  auto PrettyPrinter::operator()(const syntax::TypeExpr &node) -> String {
    return node.name();
  }

  auto PrettyPrinter::operator()(const syntax::BlockStatement &node) -> String {
    auto results = visitChildren(node);
    auto str = std::accumulate(
      results.begin(), results.end(), std::string(),
      [](const String &a, const String &b) {
        return a + b + "\n";
      }
    );
    exitScope();
    str += "}\n";
    return str;
  }

  auto PrettyPrinter::operator()(const syntax::ReturnStatement &node) -> String {
    auto results = visitChildren(node);
    return scopeString() + "return" + (results.empty() ? "" : " " + results[0]) + ";";
  }

  auto PrettyPrinter::operator()(const syntax::Identifier &node) -> String {
    return node.name();
  }

  auto PrettyPrinter::operator()(const syntax::BinaryExpr &node) -> String {
    auto results = visitChildren(node);
    return results[0] + " " + node.op() + " " + results[1];
  }

  auto PrettyPrinter::operator()(const syntax::UnaryExpr &node) -> String {
    visitChildren(node);
    return "\n";
  }

  auto PrettyPrinter::operator()(const syntax::IntegerLiteral &node) -> String {
    visitChildren(node);
    return "\n";
  }

  auto PrettyPrinter::operator()(const syntax::FloatLiteral &node) -> String {
    visitChildren(node);
    return "\n";
  }

  auto PrettyPrinter::operator()(const syntax::StringLiteral &node) -> String {
    visitChildren(node);
    return "\n";
  }

  auto PrettyPrinter::operator()(const syntax::BooleanLiteral &node) -> String {
    visitChildren(node);
    return "\n";
  }
}
