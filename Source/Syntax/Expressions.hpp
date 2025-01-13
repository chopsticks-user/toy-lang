#ifndef  TOYLANG_SYNTAX_EXPRESSIONS_HPP
#define  TOYLANG_SYNTAX_EXPRESSIONS_HPP

#include "Base.hpp"
#include "Core/Core.hpp"

namespace tl::syntax {
  // Literals belong here to avoid circular dependencies
  // between Expression and Literals

  class IntegerLiteral final : public ASTNodeBase {
  public:
    explicit IntegerLiteral(i64 value);

    auto value() const noexcept -> i64 {
      return m_value;
    }

  private:
    i64 m_value;
  };

  class FloatLiteral final : public ASTNodeBase {
  public:
    explicit FloatLiteral(f64 value);

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
    explicit BooleanLiteral(bool value);

    auto value() const noexcept -> bool {
      return m_value;
    }

  private:
    bool m_value;
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

  class Identifier final : public ASTNodeBase {
  public:
    explicit Identifier(Vec<String> path);

    auto name() const noexcept -> String {
      return isAnonymous() ? "" : m_path.back();
    }

    auto path() const noexcept -> String {
      if (isAnonymous()) {
        return "";
      }

      String pathStr = m_path.front();

      if (!isImported()) {
        return pathStr;
      }

      for (CRef<String> s: m_path | rv::drop(1)) {
        pathStr += "::" + s;
      }

      return pathStr;
    }

    auto isImported() const noexcept -> bool {
      return m_path.size() > 1;
    }

    auto isType() const noexcept -> bool {
      if (isAnonymous()) {
        return false;
      }
      const char c = name()[0];
      return c >= 'A' && c <= 'Z';
    }

    auto isOverloadedOp() const noexcept -> bool {
      return m_path.size() == 1 && overloadableOps.contains(m_path.back());
    }

    auto isAnonymous() const noexcept -> bool {
      return m_path.empty();
    }

  private:
    Vec<String> m_path;
  };

  class TupleExpr final : public ASTNodeBase {
  public:
    explicit TupleExpr(Vec<ASTNode> ids);

    auto size() const noexcept -> sz {
      return nChildren();
    }

    auto expr(const u64 index) const noexcept -> CRef<ASTNode> {
      return childAt(index);
    }
  };

  class FunctionCallExpr final : public ASTNodeBase {
  public:
    FunctionCallExpr(ASTNode callee, ASTNode args);

    static auto fromPipeExpr(ASTNode lhs, ASTNode rhs) -> Opt<FunctionCallExpr>;

    auto callee() const noexcept -> CRef<ASTNode> {
      return childAt(0);
    }

    auto args() const noexcept -> CRef<ASTNode> {
      return childAt(1);
    }
  };

  class SubScriptingExpr final : public ASTNodeBase {
  public:
    SubScriptingExpr(ASTNode collection, ASTNode subscript);

    auto collection() const noexcept -> CRef<ASTNode> {
      return childAt(0);
    }

    auto subscript() const noexcept -> CRef<ASTNode> {
      return childAt(1);
    }
  };

  class AccessExpr final : public ASTNodeBase {
  public:
    AccessExpr(ASTNode object, ASTNode field);

    auto object() const noexcept -> CRef<ASTNode> {
      return childAt(0);
    }

    auto field() const noexcept -> CRef<ASTNode> {
      return childAt(1);
    }
  };

  class TypeOfExpr final : public ASTNodeBase {
  public:
    explicit TypeOfExpr(ASTNode identifier);

    auto identifier() const noexcept -> CRef<ASTNode> {
      return childAt(0);
    }
  };

  class TypeExpr final : public ASTNodeBase {
  public:
    explicit TypeExpr(Vec<ASTNode> types);

    auto nTypes() const noexcept -> sz {
      return nChildren();
    }

    auto type(const sz index) const -> CRef<ASTNode> {
      return childAt(index);
    }
  };

  class ArrayExpr final : public ASTNodeBase {
  public:
    explicit ArrayExpr(Vec<ASTNode> elements);

    auto size() const noexcept -> sz {
      return nChildren();
    }

    auto element(const sz index) const -> CRef<ASTNode> {
      return childAt(index);
    }
  };
}

#endif // TOYLANG_SYNTAX_EXPRESSIONS_HPP
