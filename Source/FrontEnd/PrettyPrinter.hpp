#ifndef  TOYLANG_FRONTEND_PRETTY_PRINTER_HPP
#define  TOYLANG_FRONTEND_PRETTY_PRINTER_HPP

#include "Syntax/Syntax.hpp"

#include "Core/Core.hpp"

namespace tl::fe {
  class PrettyPrinter : public syntax::SyntaxTreeVisitor<PrettyPrinter, String> {
  public:
    using Super::operator();

    auto operator()(const syntax::TranslationUnit &node) -> String;

    auto operator()(const syntax::ModuleExpr &node) -> String;

    auto operator()(const syntax::ImportExpr &node) -> String;

    auto operator()(const syntax::Function &node) -> String;

    auto operator()(const syntax::FunctionPrototype &node) -> String;

    auto operator()(const syntax::ParameterDeclFragment &node) -> String;

    auto operator()(const syntax::IdentifierDeclFragment &node) -> String;

    auto operator()(const syntax::TypeExpr &node) -> String;

    auto operator()(const syntax::BlockStatement &node) -> String;

    auto operator()(const syntax::ReturnStatement &node) -> String;

    auto operator()(const syntax::Identifier &node) -> String;

    auto operator()(const syntax::BinaryExpr &node) -> String;

    auto operator()(const syntax::UnaryExpr &node) -> String;

    auto operator()(const syntax::IntegerLiteral &node) -> String;

    auto operator()(const syntax::FloatLiteral &node) -> String;

    auto operator()(const syntax::StringLiteral &node) -> String;

    auto operator()(const syntax::BooleanLiteral &node) -> String;

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
