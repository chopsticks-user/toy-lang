#ifndef  TOYLANG_FRONTEND_PRETTY_PRINTER_HPP
#define  TOYLANG_FRONTEND_PRETTY_PRINTER_HPP

#include "Syntax/Syntax.hpp"

#include "Core/Core.hpp"

namespace tl::fe {
  class ImportedModuleNameCollector
      : public syntax::SyntaxTreeVisitor<ImportedModuleNameCollector, Vec<String> > {
  public:
    using SyntaxTreeVisitor::operator();

    auto operator()(CRef<syntax::TranslationUnit> node) -> Vec<String> {
      auto v = visitChildren(node) | rv::join;
      return {v.begin(), v.end()};
    }

    auto operator()(CRef<syntax::ImportExpr> node) -> Vec<String> {
      auto results = visitChildren(node);
      return {
        std::accumulate(
          std::next(results.begin()), results.end(), results[0].front(),
          [](const String &a, const Vec<String> &b) {
            return a + "::" + b.front();
          }
        )
      };
    }

    auto operator()(CRef<syntax::Identifier> node) -> Vec<String> {
      return {node.name()};
    }
  };

  class PrettyPrinter : public syntax::SyntaxTreeVisitor<PrettyPrinter, String> {
  public:
    using SyntaxTreeVisitor::operator();

    auto operator()(CRef<syntax::TranslationUnit> node) -> String;

    auto operator()(CRef<syntax::ModuleExpr> node) -> String;

    auto operator()(CRef<syntax::ImportExpr> node) -> String;

    auto operator()(CRef<syntax::Function> node) -> String;

    auto operator()(CRef<syntax::FunctionPrototype> node) -> String;

    auto operator()(CRef<syntax::ParameterDeclFragment> node) -> String;

    auto operator()(CRef<syntax::IdentifierDeclFragment> node) -> String;

    auto operator()(CRef<syntax::TypeExpr> node) -> String;

    auto operator()(CRef<syntax::BlockStatement> node) -> String;

    auto operator()(CRef<syntax::ReturnStatement> node) -> String;

    auto operator()(CRef<syntax::Identifier> node) -> String;

    auto operator()(CRef<syntax::BinaryExpr> node) -> String;

    auto operator()(CRef<syntax::UnaryExpr> node) -> String;

    auto operator()(CRef<syntax::IntegerLiteral> node) -> String;

    auto operator()(CRef<syntax::FloatLiteral> node) -> String;

    auto operator()(CRef<syntax::StringLiteral> node) -> String;

    auto operator()(CRef<syntax::BooleanLiteral> node) -> String;

  private:
    auto enterScope() -> void {
      m_whiteSpaces += "  ";
    }

    auto exitScope() -> void {
      m_whiteSpaces = m_whiteSpaces.substr(0, m_whiteSpaces.length() - 2);
    }

    auto scopeString() -> String {
      return m_whiteSpaces;
    }

  private:
    String m_whiteSpaces;
  };
}

#endif // TOYLANG_FRONTEND_PRETTY_PRINTER_HPP
