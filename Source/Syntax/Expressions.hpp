#ifndef  TOYLANG_SYNTAX_EXPRESSIONS_HPP
#define  TOYLANG_SYNTAX_EXPRESSIONS_HPP

#include "Base.hpp"

#include "Core/Core.hpp"

namespace tl::syntax {
  class ParameterDeclFragment final : public Node {
  public:
    ParameterDeclFragment(
      VNode identifierDeclFragment,
      String mut
    );

    auto identifier() const -> CRef<VNode> {
      return childAt(0);
    }

    auto typeExpr() const -> CRef<VNode> {
      return childAt(1);
    }

    auto mutibility() const -> CRef<String> {
      return m_mutibility;
    }

  private:
    String m_mutibility;
  };

  class IdentifierDeclFragment final : public Node {
  public:
    IdentifierDeclFragment(
      const VNode &identifier,
      const VNode &typeExpr,
      const VNode &rhsExpr
    );

    auto identifier() -> CRef<VNode> {
      return childAt(0);
    }

    auto typeExpr() -> CRef<VNode> {
      return childAt(1);
    }

    auto rhsExpr() -> CRef<VNode> {
      return childAt(2);
    }

  private:
  };

  class Identifier final : public Node {
  public:
    explicit Identifier(String name);

    auto name() const noexcept -> CRef<String> {
      return m_name;
    }

  private:
    String m_name;
  };

  class TypeExpr final : public Node {
  public:
    explicit TypeExpr(String name);

    auto name() const noexcept -> CRef<String> {
      return m_name;
    }

  private:
    String m_name;
  };

  class BinaryExpr final : public Node {
  public:
    BinaryExpr(VNode lhs, VNode rhs, String op);

    auto left() const noexcept -> CRef<VNode> {
      return childAt(0);
    }

    auto right() const noexcept -> CRef<VNode> {
      return childAt(1);
    }

    auto op() const noexcept -> CRef<String> {
      return m_op;
    }

  private:
    String m_op;
  };

  class TernaryExpr final : public Node {
  public:
    TernaryExpr(
      VNode operand1,
      VNode operand2,
      VNode operand3,
      String op1, String op2
    );

    auto first() const noexcept -> CRef<VNode> {
      return childAt(0);
    }

    auto second() const noexcept -> CRef<VNode> {
      return childAt(1);
    }

    auto third() const noexcept -> CRef<VNode> {
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

  class UnaryExpr final : public Node {
  public:
    UnaryExpr(VNode operand, String op);

    auto operand() const noexcept -> CRef<VNode> {
      return childAt(0);
    }

    auto op() const noexcept -> CRef<String> {
      return m_op;
    }

  private:
    String m_op;
  };

  class PostfixUnaryExpr final : public Node {
  public:
    PostfixUnaryExpr(VNode operand, String op);

    auto operand() const noexcept -> CRef<VNode> {
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

  class IntegerLiteral final : public Node {
  public:
    explicit IntegerLiteral(CRef<String> value);

    auto value() const noexcept -> i64 {
      return m_value;
    }

  private:
    i64 m_value;
  };

  class FloatLiteral final : public Node {
  public:
    explicit FloatLiteral(CRef<String> value);

    auto value() const noexcept -> double {
      return m_value;
    }

  private:
    f64 m_value;
  };

  class StringLiteral final : public Node {
  public:
    explicit StringLiteral(
      String value,
      Vec<VNode> placeholders = {}
    );

    auto value() const noexcept -> CRef<String> {
      return m_value;
    }

    auto placeholder(const sz index) const noexcept -> CRef<VNode> {
      return childAt(index);
    }

  private:
    String m_value;
  };

  class BooleanLiteral final : public Node {
  public:
    explicit BooleanLiteral(CRef<String> value);

    auto value() const noexcept -> bool {
      return m_value;
    }

  private:
    bool m_value;
  };

  class FunctionCallExpr final : public Node {
  public:
    FunctionCallExpr(
      CRef<VNode> callee,
      Vec<VNode> args
    );

    auto callee() const noexcept -> VNode;
  };

  class SubScriptingExpr final : public Node {
  public:
    SubScriptingExpr(
      VNode collection,
      VNode subscript
    );

    auto collection() const noexcept -> VNode;

    auto subscript() const noexcept -> VNode;
  };

  class ModuleExpr final : public Node {
  public:
    explicit ModuleExpr(Vec<VNode> fragments);

    auto fragment(const sz index) -> CRef<VNode> {
      return childAt(index);
    }

  private:
  };

  class ImportExpr final : public Node {
  public:
    explicit ImportExpr(Vec<VNode> fragments);

    auto fragment(const sz index) -> CRef<VNode> {
      return childAt(index);
    }

  private:
  };
}

#endif // TOYLANG_SYNTAX_EXPRESSIONS_HPP
