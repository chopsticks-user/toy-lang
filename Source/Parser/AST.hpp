#ifndef TOYLANG_SYNTAX_AST_HPP
#define TOYLANG_SYNTAX_AST_HPP

#include "Core/Core.hpp"

namespace tl::parser::ast {
  class Program;
  class Function;
  class BinaryExpr;
  class UnaryExpr;
  class BlockScope;
  class Number;

  // using VNode = std::variant<Program, Function, BinaryExpr, UnaryExpr, BlockScope>;
  using VNode = std::variant<BinaryExpr, UnaryExpr, Number>;

  template<sz NChildren>
  class Node {
  public:
    auto children() const noexcept -> std::array<std::unique_ptr<VNode>, NChildren> & {
      return m_children;
    }

    auto childAt(sz index) noexcept -> VNode & {
      return *m_children[index];
    }

  protected:
    explicit Node(std::array<std::unique_ptr<VNode>, NChildren> children) noexcept
      : m_children(std::move(children)) {
    }

  private:
    std::array<std::unique_ptr<VNode>, NChildren> m_children;
  };

  class BinaryExpr final : public Node<2> {
  public:
    BinaryExpr(std::unique_ptr<VNode> left, std::unique_ptr<VNode> right, std::string op)
      : Node({std::move(left), std::move(right)}), m_op(std::move(op)) {
    }

    auto op() const noexcept -> const std::string & {
      return m_op;
    }

  private:
    std::string m_op;
  };

  class UnaryExpr final : public Node<1> {
  public:
    UnaryExpr(std::unique_ptr<VNode> operand, std::string op)
      : Node({std::move(operand)}), m_op(std::move(op)) {
    }

    auto op() const noexcept -> const std::string & {
      return m_op;
    }

  private:
    std::string m_op;
  };

  class Number final {
  public:
    explicit Number(const std::string &value)
      : m_value(std::stod(value)) {
    }

    auto value() const noexcept -> double {
      return m_value;
    }

  private:
    f64 m_value;
  };

  // class AST {
  // public:
  //   // virtual auto children() const -> std::vector<>
  // };
  //
  // class ProgramAST : virtual public AST {
  // public:
  // };
  //
  // class ExpressionAST : virtual public AST {
  // public:
  // };
  //
  // class BinaryExpressionAST : public ExpressionAST {
  // public:
  // };
  //
  // class StatementAST : virtual public AST {
  // public:
  // };
  //
  // class PrototypeAST : virtual public AST {
  // public:
  //   PrototypeAST(std::vector<std::shared_ptr<ExpressionAST> > params)
  //     : m_params(std::move(params)) {
  //   }
  //
  // private:
  //   ExpressionAST m_decl;
  //   std::vector<std::shared_ptr<ExpressionAST> > m_params;
  // };
  //
  // // using Printable = std::variant<BinaryExpressionAST, ExpressionAST, PrototypeAST>;
  //
  // template<typename... TVisitable>
  // class Visitor {
  // public:
  //   using Visitable = std::variant<TVisitable...>;
  //
  // protected:
  //   Visitor() = default;
  // };
  //
  // class PrettyPrinter : public Visitor<BinaryExpressionAST, ExpressionAST, PrototypeAST> {
  // public:
  //   auto operator()(BinaryExpressionAST const &node) -> std::string {
  //     return result += "Visiting BinaryExpressionAST\n";
  //   }
  //
  //   auto operator()(ExpressionAST const &node) -> std::string {
  //     return result += "Visiting ExpressionAST\n";
  //   }
  //
  //   auto operator()(PrototypeAST const &node) -> std::string {
  //     return result += "Visiting PrototypeAST\n";
  //   }
  //
  // private:
  //   std::string result;
  // };
  //
  // template<typename T>
  // concept IsVisitor = std::same_as<T, PrettyPrinter> && std::constructible_from<T>;
  //
  // template<IsVisitor TVisitor>
  // auto visit(typename TVisitor::Visitable const &visitable) {
  //   return std::visit(TVisitor{}, visitable);
  // }
}


#endif // TOYLANG_SYNTAX_AST_HPP
