#ifndef  TOYLANG_FRONTEND_PRETTY_PRINTER_HPP
#define  TOYLANG_FRONTEND_PRETTY_PRINTER_HPP

#include "syntax/Syntax.hpp"

#include "core/Core.hpp"

namespace tl::fe {
  class PrettyPrinter : public syntax::SyntaxTreeVisitor<PrettyPrinter, String> {
  public:
    using SyntaxTreeVisitor::operator();

    auto operator()(CRef<syntax::TranslationUnit> node) -> String;

    auto operator()(CRef<syntax::ModuleDecl> node) -> String;

    auto operator()(CRef<syntax::ImportDecl> node) -> String;

    auto operator()(CRef<syntax::TypeDecl> node) -> String;

    // auto operator()(CRef<syntax::TypeExpr> node) -> String;

    auto operator()(CRef<syntax::VarId> node) -> String;

    // auto operator()(CRef<syntax::Function> node) -> String;
    //
    // auto operator()(CRef<syntax::ParameterDeclFragment> node) -> String;
    //
    // auto operator()(CRef<syntax::IdentifierDeclFragment> node) -> String;
    //
    // auto operator()(CRef<syntax::TypeExpr> node) -> String;
    //
    // auto operator()(CRef<syntax::BlockStatement> node) -> String;
    //
    // auto operator()(CRef<syntax::ReturnStatement> node) -> String;
    //
    // auto operator()(CRef<syntax::Identifier> node) -> String;
    //
    // auto operator()(CRef<syntax::BinaryExpr> node) -> String;
    //
    // auto operator()(CRef<syntax::UnaryExpr> node) -> String;
    //
    // auto operator()(CRef<syntax::IntegerLiteral> node) -> String;
    //
    // auto operator()(CRef<syntax::FloatLiteral> node) -> String;
    //
    // auto operator()(CRef<syntax::StringLiteral> node) -> String;
    //
    // auto operator()(CRef<syntax::BooleanLiteral> node) -> String;

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

    static auto storageString(const syntax::Storage storage) -> String {
      switch (storage) {
        case syntax::Storage::Export: return "export";
        case syntax::Storage::Local: return "local";
        default: return "internal";
      }
    }

  private:
    String m_whiteSpaces;
  };
}

#endif // TOYLANG_FRONTEND_PRETTY_PRINTER_HPP
