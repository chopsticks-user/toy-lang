#ifndef  TLC_SYNTAX_EXPR_NODES_HPP
#define  TLC_SYNTAX_EXPR_NODES_HPP

#include "ast_base.hpp"
#include "core/core.hpp"

namespace tlc::syntax {
  // Literals belong here to avoid circular dependencies
  // between Expression and Literals

  class IntegerLiteral final : public NodeBase {
  public:
    explicit IntegerLiteral(i64 value);

    auto value() const noexcept -> i64 {
      return m_value;
    }

  private:
    i64 m_value;
  };

  class FloatLiteral final : public NodeBase {
  public:
    explicit FloatLiteral(f64 value);

    auto value() const noexcept -> double {
      return m_value;
    }

  private:
    f64 m_value;
  };

  class StringLiteral final : public NodeBase {
  public:
    explicit StringLiteral(
      Str value,
      Vec<Node> placeholders = {}
    );

    auto value() const noexcept -> StrV {
      return m_value;
    }

    // auto placeholder(const sz index) const noexcept -> ASTNode const& {
    //   return childAt(index);
    // }

  private:
    Str m_value;
  };

  class BooleanLiteral final : public NodeBase {
  public:
    explicit BooleanLiteral(bool value);

    auto value() const noexcept -> bool {
      return m_value;
    }

  private:
    bool m_value;
  };

  class TernaryExpr final : public NodeBase {
  public:
    TernaryExpr(
      Node operand1,
      Node operand2,
      Node operand3,
      Str op1, Str op2
    );

    auto first() const noexcept -> Node const & {
      return childAt(0);
    }

    auto second() const noexcept -> Node const & {
      return childAt(1);
    }

    auto third() const noexcept -> Node const & {
      return childAt(2);
    }

    auto firstOp() const noexcept -> StrV {
      return m_op1;
    }

    auto secondOp() const noexcept -> StrV {
      return m_op2;
    }

  private:
    Str m_op1;
    Str m_op2;
  };

  class BinaryExpr final : public NodeBase {
  public:
    BinaryExpr(Node lhs, Node rhs, Str op);

    auto left() const noexcept -> Node const & {
      return childAt(0);
    }

    auto right() const noexcept -> Node const & {
      return childAt(1);
    }

    auto op() const noexcept -> StrV {
      return m_op;
    }

  private:
    Str m_op;
  };

  class UnaryExpr final : public NodeBase {
  public:
    UnaryExpr(Node operand, Str op);

    auto operand() const noexcept -> Node const & {
      return childAt(0);
    }

    auto op() const noexcept -> StrV {
      return m_op;
    }

  private:
    Str m_op;
  };

  class IdentifierBase : public NodeBase {
  public:
    explicit IdentifierBase(Vec<Str> path = {});

    auto name() const noexcept -> Str {
      return m_path.empty() ? "" : m_path.back();
    }

    auto path() const noexcept -> Str {
      if (m_path.empty()) {
        return "";
      }

      Str pathStr = m_path.front();

      if (!isImported()) {
        return pathStr;
      }

      for (StrV s: m_path | rv::drop(1)) {
        pathStr += "::"s + Str(s);
      }

      return pathStr;
    }

    auto isImported() const noexcept -> bool {
      return m_path.size() > 1;
    }

  protected:
    Vec<Str> m_path;
  };

  class VarId final : public IdentifierBase {
  public:
    explicit VarId(Vec<Str> path = {})
      : IdentifierBase(std::move(path)) {
    }

    auto isAnonymous() const noexcept -> bool {
      return m_path.empty();
    }
  };

  class TypeId final : public IdentifierBase {
  public:
    explicit TypeId(Vec<Str> path)
      : IdentifierBase(std::move(path)) {
    }
  };

  class OpId final : public IdentifierBase {
  public:
    explicit OpId(Str op)
      : IdentifierBase({std::move(op)}) {
    }

    auto op() const noexcept -> StrV {
      return m_path.back();
    }
  };

  class TupleExpr final : public NodeBase {
  public:
    explicit TupleExpr(Vec<Node> ids);

    auto size() const noexcept -> sz {
      return nChildren();
    }

    auto expr(const u64 index) const noexcept -> Node const & {
      return childAt(index);
    }
  };

  class FunctionCallExpr final : public NodeBase {
  public:
    FunctionCallExpr(Node callee, Node args);

    // static auto fromPipeExpr(ASTNode lhs, ASTNode rhs) -> Opt<FunctionCallExpr>;

    auto callee() const noexcept -> Node const & {
      return childAt(0);
    }

    auto args() const noexcept -> Node const & {
      return childAt(1);
    }
  };

  class SubScriptingExpr final : public NodeBase {
  public:
    SubScriptingExpr(Node collection, Node subscript);

    auto collection() const noexcept -> Node const & {
      return childAt(0);
    }

    auto subscript() const noexcept -> Node const & {
      return childAt(1);
    }
  };

  class AccessExpr final : public NodeBase {
  public:
    AccessExpr(Node object, Node field);

    auto object() const noexcept -> Node const & {
      return childAt(0);
    }

    auto field() const noexcept -> Node const & {
      return childAt(1);
    }
  };

  // class TypeOfExpr final : public ASTNodeBase {
  // public:
  //   explicit TypeOfExpr(ASTNode identifier);
  //
  //   auto identifier() const noexcept -> ASTNode const& {
  //     return childAt(0);
  //   }
  // };
  //
  // class TypeExpr final : public ASTNodeBase {
  // public:
  //   explicit TypeExpr(Vec<ASTNode> types);
  //
  //   auto nTypes() const noexcept -> sz {
  //     return nChildren();
  //   }
  //
  //   auto type(const sz index) const -> ASTNode const& {
  //     return childAt(index);
  //   }
  // };

  class ArrayExpr final : public NodeBase {
  public:
    explicit ArrayExpr(Vec<Node> elements);

    auto size() const noexcept -> sz {
      return nChildren();
    }

    auto element(const sz index) const -> Node const & {
      return childAt(index);
    }
  };
}

#endif // TLC_SYNTAX_EXPR_NODES_HPP
