#ifndef  TOYLANG_SYNTAX_EXPRESSIONS_HPP
#define  TOYLANG_SYNTAX_EXPRESSIONS_HPP

#include "Base.hpp"

#include "Core/Core.hpp"

namespace tl::syntax {
  class ParameterDeclFragment final : public ASTNodeBase {
  public:
    ParameterDeclFragment(
      ASTNode identifierDeclFragment,
      String mut
    );

    auto identifier() const -> CRef<ASTNode> {
      return childAt(0);
    }

    auto typeExpr() const -> CRef<ASTNode> {
      return childAt(1);
    }

    auto mutibility() const -> CRef<String> {
      return m_mutibility;
    }

  private:
    String m_mutibility;
  };

  class IdentifierDeclFragment final : public ASTNodeBase {
  public:
    IdentifierDeclFragment(
      const ASTNode &identifier,
      const ASTNode &typeExpr,
      const ASTNode &rhsExpr
    );

    auto identifier() -> CRef<ASTNode> {
      return childAt(0);
    }

    auto typeExpr() -> CRef<ASTNode> {
      return childAt(1);
    }

    auto rhsExpr() -> CRef<ASTNode> {
      return childAt(2);
    }
  };

  class Identifier final : public ASTNodeBase {
  public:
    explicit Identifier(String name);

    auto name() const noexcept -> CRef<String> {
      return m_name;
    }

  private:
    String m_name;
  };

  class TypeExpr final : public ASTNodeBase {
  public:
    explicit TypeExpr(String name);

    auto name() const noexcept -> CRef<String> {
      return m_name;
    }

  private:
    String m_name;
  };

  class BinaryExpr final : public ASTNodeBase {
  public:
    BinaryExpr(ASTNode lhs, ASTNode rhs, String op);

    auto left() const noexcept -> CRef<ASTNode> {
      return childAt(0);
    }

    auto right() const noexcept -> CRef<ASTNode> {
      return childAt(1);
    }

    auto op() const noexcept -> CRef<String> {
      return m_op;
    }

  private:
    String m_op;
  };

  class TernaryExpr final : public ASTNodeBase {
  public:
    TernaryExpr(
      ASTNode operand1,
      ASTNode operand2,
      ASTNode operand3,
      String op1, String op2
    );

    auto first() const noexcept -> CRef<ASTNode> {
      return childAt(0);
    }

    auto second() const noexcept -> CRef<ASTNode> {
      return childAt(1);
    }

    auto third() const noexcept -> CRef<ASTNode> {
      return childAt(2);
    }

    auto firstOp() const noexcept -> CRef<String> {
      return m_op1;
    }

    auto secondOp() const noexcept -> CRef<String> {
      return m_op2;
    }

  private:
    String m_op1;
    String m_op2;
  };

  class UnaryExpr final : public ASTNodeBase {
  public:
    UnaryExpr(ASTNode operand, String op);

    auto operand() const noexcept -> CRef<ASTNode> {
      return childAt(0);
    }

    auto op() const noexcept -> CRef<String> {
      return m_op;
    }

  private:
    String m_op;
  };

  class PostfixUnaryExpr final : public ASTNodeBase {
  public:
    PostfixUnaryExpr(ASTNode operand, String op);

    auto operand() const noexcept -> CRef<ASTNode> {
      return childAt(0);
    }

    auto op() const noexcept -> CRef<String> {
      return m_op;
    }

  private:
    String m_op;
  };

  // Literals belong here to avoid circular dependencies
  // between Expression and Literals

  class IntegerLiteral final : public ASTNodeBase {
  public:
    explicit IntegerLiteral(CRef<String> value);

    auto value() const noexcept -> i64 {
      return m_value;
    }

  private:
    i64 m_value;
  };

  class FloatLiteral final : public ASTNodeBase {
  public:
    explicit FloatLiteral(CRef<String> value);

    auto value() const noexcept -> double {
      return m_value;
    }

  private:
    f64 m_value;
  };

  class StringLiteral final : public ASTNodeBase {
  public:
    explicit StringLiteral(
      String value,
      Vec<ASTNode> placeholders = {}
    );

    auto value() const noexcept -> CRef<String> {
      return m_value;
    }

    auto placeholder(const sz index) const noexcept -> CRef<ASTNode> {
      return childAt(index);
    }

  private:
    String m_value;
  };

  class BooleanLiteral final : public ASTNodeBase {
  public:
    explicit BooleanLiteral(CRef<String> value);

    auto value() const noexcept -> bool {
      return m_value;
    }

  private:
    bool m_value;
  };

  class FunctionCallExpr final : public ASTNodeBase {
  public:
    FunctionCallExpr(
      CRef<ASTNode> callee,
      Vec<ASTNode> args
    );

    auto callee() const noexcept -> ASTNode;
  };

  class SubScriptingExpr final : public ASTNodeBase {
  public:
    SubScriptingExpr(
      ASTNode collection,
      ASTNode subscript
    );

    auto collection() const noexcept -> ASTNode;

    auto subscript() const noexcept -> ASTNode;
  };

  class ModuleExpr final : public ASTNodeBase {
  public:
    explicit ModuleExpr(Vec<ASTNode> fragments);

    auto fragment(const sz index) -> CRef<ASTNode> {
      return childAt(index);
    }
  };

  class ImportExpr final : public ASTNodeBase {
  public:
    explicit ImportExpr(Vec<ASTNode> fragments);

    auto fragment(const sz index) -> CRef<ASTNode> {
      return childAt(index);
    }
  };
}

#endif // TOYLANG_SYNTAX_EXPRESSIONS_HPP
