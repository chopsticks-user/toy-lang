#ifndef TOYLANG_SYNTAX_AST_HPP
#define TOYLANG_SYNTAX_AST_HPP


namespace tl::syntax::ast {
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
