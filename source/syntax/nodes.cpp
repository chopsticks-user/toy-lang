#include "nodes.hpp"

namespace tlc::syntax {
    namespace expr {
        Integer::Integer(i64 const value)
            : NodeBase({}), m_value(value) {}

        Float::Float(f64 const value)
            : NodeBase({}), m_value(value) {}

        String::String(Str value, Vec<Node> placeholders)
            : NodeBase({std::move(placeholders)}), m_value(std::move(value)) {}

        Boolean::Boolean(bool const value)
            : NodeBase({}), m_value(value) {}

        IdentifierBase::IdentifierBase(Vec<Str> path)
            : NodeBase({}), m_path(std::move(path)) {}

        VarId::VarId(Vec<Str> path)
            : IdentifierBase(std::move(path)) {}

        TypeId::TypeId(Vec<Str> path)
            : IdentifierBase(std::move(path)) {}

        OpId::OpId(Str op)
            : IdentifierBase({std::move(op)}) {}

        Ternary::Ternary(
            Node operand1, Node operand2, Node operand3,
            Str op1, Str op2
        ): NodeBase(
               {std::move(operand1), std::move(operand2), std::move(operand3)}
           ), m_op1(std::move(op1)), m_op2(std::move(op2)) {}

        Binary::Binary(Node lhs, Node rhs, Str op)
            : NodeBase({std::move(lhs), std::move(rhs)}),
              m_op(std::move(op)) {}

        Unary::Unary(Node operand, Str op)
            : NodeBase({std::move(operand)}), m_op(std::move(op)) {}

        Tuple::Tuple(Vec<Node> ids) : NodeBase(std::move(ids)) {}

        FunApp::FunApp(Node callee, Node args
        ) : NodeBase({std::move(callee), std::move(args)}) {}

        // auto FunctionCallExpr::fromPipeExpr(
        //   ASTNode lhs, ASTNode rhs
        // ) -> Opt<FunctionCallExpr> {
        //   if (matchAstType<FunctionCallExpr>(rhs)) {
        //     Vec<ASTNode> args = astCast<TupleExpr>(
        //       astCast<FunctionCallExpr>(rhs).args()
        //     ).children();
        //     args.insert(args.begin(), lhs);
        //
        //     return FunctionCallExpr{
        //       astCast<FunctionCallExpr>(rhs).callee(), TupleExpr{std::move(args)}
        //     };
        //   }
        //
        //   if (matchAstType<VarId>(rhs)) {
        //     return FunctionCallExpr{rhs, {lhs}};
        //   }
        //
        //   return {};
        // }

        SubScript::SubScript(
            Node collection,
            Node subscript
        ): NodeBase({std::move(collection), std::move(subscript)}) {}

        Access::Access(Node&& object, Node&& field)
            : NodeBase({std::forward<Node>(object), std::forward<Node>(field)}) {}

        Array::Array(Vec<Node> elements) : NodeBase(std::move(elements)) {}
    }

    namespace stmt {
        ForStmt::ForStmt(Node condition, Node body)
            : NodeBase({std::move(condition), std::move(body)}) {}

        ForRangeFragment::ForRangeFragment(Node iterator, Node iterable)
            : NodeBase({std::move(iterator), std::move(iterable)}) {}

        // MatchStmt::MatchStmt(Node matchedExpr, Node defaultBody, Vec<Node> cases)
        //   : NodeBase({
        //     [&]() {
        //       // todo: move args
        //       auto v = std::vector{matchedExpr, defaultBody};
        //       v.insert(v.end(), cases.begin(), cases.end());
        //       return v;
        //     }()
        //   }) {
        // }

        MatchStmt::MatchStmt(Node matchedExpr, Node defaultBody, Vec<Node> cases)
            : NodeBase(
                rv::concat(Vec{std::move(matchedExpr), std::move(defaultBody)}, std::move(cases)) | rng::to<Vec<Node>>()
            ) {}

        MatchStmtCase::MatchStmtCase(Node value, Node condition, Node body)
            : NodeBase({std::move(value), std::move(condition), std::move(body)}) {}

        BlockStmt::BlockStmt(Vec<Node> statements)
            : NodeBase(std::move(statements)) {}

        LetStmt::LetStmt(Node decl, Node init)
            : NodeBase({std::move(decl), std::move(init)}) {}

        ConditionalStmt::ConditionalStmt(Node condition, Node body)
            : NodeBase({std::move(condition), std::move(body)}) {}

        ReturnStmt::ReturnStmt(Node expr)
            : NodeBase({std::move(expr)}) {}

        AssignStmt::AssignStmt(Node left, Node right, Str op)
            : NodeBase({std::move(left), std::move(right)}), m_op(std::move(op)) {}

        ExprStmt::ExprStmt(Node expr)
            : NodeBase({std::move(expr)}) {}
    }

    namespace decl {
        Module::Module(Node nsIdentifier, szt const line, szt const column)
            : NodeBase({nsIdentifier}, line, column) {}

        ImportDecl::ImportDecl(Node nsIdentifier)
            : NodeBase({nsIdentifier}) {}

        TypeDecl::TypeDecl(const Storage storage, Node identifier, Node typeExpr)
            : NodeBase({identifier, typeExpr}), m_storage(storage) {}

        IdentifierDecl::IdentifierDecl(
            const bool isMutable, Node const& identifier, Node const& typeExpr
        ): NodeBase({identifier, typeExpr}), m_mutable(isMutable) {}

        TupleDecl::TupleDecl(Vec<Node> idDecls)
            : NodeBase(std::move(idDecls)) {}

        GenericDecl::GenericDecl(Vec<Node> typeIds)
            : NodeBase(std::move(typeIds)) {}
    }

    namespace def {
        FunctionPrototype::FunctionPrototype(
            const FnType fType, Node fIdentifier, Node fParamDecls, Node fReturnDecls
        ): NodeBase({fIdentifier, fParamDecls, fReturnDecls}),
           m_type(fType) {}

        FunctionDef::FunctionDef(
            const Storage fStorage, Node fPrototype, Node fBody
        ): NodeBase({fPrototype, fBody}), m_storage(fStorage) {}

        ConceptDef::ConceptDef(const Storage storage, Node identifier, Vec<Node> requirements)
            : NodeBase(
                  rv::concat(
                      Vec{std::move(identifier)}, std::move(requirements)
                  ) | rng::to<Vec<Node>>()
              ), m_storage(storage) {}
    }

    TokenWrapper::TokenWrapper(token::Token token)
        : NodeBase{{}}, m_token{std::move(token)} {}

    TranslationUnit::TranslationUnit(Vec<Node> definitions)
        : NodeBase(std::move(definitions)) {}
}
