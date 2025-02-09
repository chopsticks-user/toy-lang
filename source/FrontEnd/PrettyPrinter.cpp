#include "PrettyPrinter.hpp"

namespace tl::fe {
  auto PrettyPrinter::operator()(CRef<syntax::TranslationUnit> node) -> String {
    Strings results = visitChildren(node);
    return std::accumulate(
             results.begin(), results.end(), ""s,
             [](CRef<String> a, CRef<String> b) {
               return a + "\n" + b;
             }
           ) + "\n";
  }

  auto PrettyPrinter::operator()(CRef<syntax::ModuleDecl> node) -> String {
    return "module " + syntax::astCast<syntax::VarId>(node.identifier()).path() + ";\n";
  }

  auto PrettyPrinter::operator()(CRef<syntax::ImportDecl> node) -> String {
    return "import " + syntax::astCast<syntax::VarId>(node.identifier()).path() + ";";
  }

  auto PrettyPrinter::operator()(CRef<syntax::TypeDecl> node) -> String {
    const Strings results = visitChildren(node);
    return "\n" + storageString(node.storage()) + " type " + results[0] + " = " + results[1] + ";";
  }

  // auto PrettyPrinter::operator()(CRef<syntax::TypeExpr> node) -> String {
  //   Strings results = visitChildren(node);
  //
  //   if (results.size() == 1) {
  //     return results.front();
  //   }
  //
  //   return results.front() + std::accumulate(
  //            results.begin() + 1, results.end(), ""s,
  //            [](CRef<String> a, CRef<String> b) {
  //              return a + " | " + b;
  //            }
  //          );
  // }

  auto PrettyPrinter::operator()(CRef<syntax::VarId> node) -> String {
    return node.path();
  }

  // auto PrettyPrinter::operator()(const syntax::Function &node) -> String {
  //   enterScope();
  //   const Strings results = visitChildren(node);
  //   return " " + results[0] + ": " + results[1] +
  //          " {\n" + results[2];
  // }
  //
  // auto PrettyPrinter::operator()(const syntax::ParameterDeclFragment &node) -> String {
  //   Strings results = visitChildren(node);
  //   return node.mutibility() + " " + results[0];
  // }
  //
  // auto PrettyPrinter::operator()(const syntax::IdentifierDeclFragment &node) -> String {
  //   Strings results = visitChildren(node);
  //   return results[0] + ": " + results[1];
  // }
  //
  // auto PrettyPrinter::operator()(const syntax::TypeExpr &node) -> String {
  //   return node.name();
  // }
  //
  // auto PrettyPrinter::operator()(const syntax::BlockStatement &node) -> String {
  //   auto results = visitChildren(node);
  //   auto str = std::accumulate(
  //     results.begin(), results.end(), std::string(),
  //     [](const String &a, const String &b) {
  //       return a + b + "\n";
  //     }
  //   );
  //   exitScope();
  //   str += "}\n";
  //   return str;
  // }
  //
  // auto PrettyPrinter::operator()(const syntax::ReturnStatement &node) -> String {
  //   auto results = visitChildren(node);
  //   return scopeString() + "return" + (results.empty() ? "" : " " + results[0]) + ";";
  // }
  //
  // auto PrettyPrinter::operator()(const syntax::Identifier &node) -> String {
  //   return node.name();
  // }
  //
  // auto PrettyPrinter::operator()(const syntax::BinaryExpr &node) -> String {
  //   auto results = visitChildren(node);
  //   return results[0] + " " + node.op() + " " + results[1];
  // }
  //
  // auto PrettyPrinter::operator()(const syntax::UnaryExpr &node) -> String {
  //   visitChildren(node);
  //   return "\n";
  // }
  //
  // auto PrettyPrinter::operator()(const syntax::IntegerLiteral &node) -> String {
  //   visitChildren(node);
  //   return "\n";
  // }
  //
  // auto PrettyPrinter::operator()(const syntax::FloatLiteral &node) -> String {
  //   visitChildren(node);
  //   return "\n";
  // }
  //
  // auto PrettyPrinter::operator()(const syntax::StringLiteral &node) -> String {
  //   visitChildren(node);
  //   return "\n";
  // }
  //
  // auto PrettyPrinter::operator()(const syntax::BooleanLiteral &node) -> String {
  //   visitChildren(node);
  //   return "\n";
  // }
}
