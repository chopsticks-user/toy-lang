export module parse:unit_fwd;

export import lexeme;
export import syntax;
export import :error;
export import :context;

namespace tlc::parse {
    auto handleExpr(Context context) -> Opt<syntax::Node>;
    auto handlePrimaryExpr(Context context) -> Opt<syntax::Node>;
    auto handleRecordExpr(Context context) -> Opt<syntax::Node>;
    auto handleTupleExpr(Context context) -> Opt<syntax::Node>;
    auto handleArrayExpr(Context context) -> Opt<syntax::Node>;
    auto handleSingleTokenLiteral(Context context) -> Opt<syntax::Node>;
    auto handleIdentifierLiteral(Context context) -> Opt<syntax::Node>;
    auto handleString(Context context) -> Opt<syntax::Node>;
    auto handleTryExpr(Context context) -> Opt<syntax::Node>;

    auto handleType(Context context) -> Opt<syntax::Node>;
    auto handleTypeIdentifier(Context context) -> Opt<syntax::Node>;
    auto handleTypeTuple(Context context) -> Opt<syntax::Node>;
    auto handleTypeInfer(Context context) -> Opt<syntax::Node>;
    auto handleGenericArguments(Context context) -> Opt<syntax::Node>;

    auto handleDecl(Context context) -> Opt<syntax::Node>;
    auto handleIdentifierDecl(Context context) -> Opt<syntax::Node>;
    auto handleTupleDecl(Context context) -> Opt<syntax::Node>;
    auto handleGenericParamsDecl(Context context) -> Opt<syntax::Node>;

    auto handleStmt(Context context) -> Opt<syntax::Node>;
    auto handleDeclStmt(Context context) -> Opt<syntax::Node>;
    auto handleReturnStmt(Context context) -> Opt<syntax::Node>;
    auto handleDeferStmt(Context context) -> Opt<syntax::Node>;
    auto handleExprPrefixedStmt(Context context) -> Opt<syntax::Node>;
    auto handleLoopStmt(Context context) -> Opt<syntax::Node>;
    auto handleMatchStmt(Context context) -> Opt<syntax::Node>;
    auto handleBlockStmt(Context context) -> Opt<syntax::Node>;

    auto handleFunctionDef(Context context) -> Opt<syntax::Node>;
    auto handleFunctionPrototype(Context context) -> Opt<syntax::Node>;
    auto handleTypeDef(Context context) -> Opt<syntax::Node>;
    auto handleEnumDef(Context context) -> Opt<syntax::Node>;
    auto handleTraitDef(Context context) -> Opt<syntax::Node>;
    auto handleFlagDef(Context context) -> Opt<syntax::Node>;
    auto handleModuleDecl(Context context) -> Opt<syntax::Node>;
    auto handleImportDecl(Context context) -> Opt<syntax::Node>;
    auto handleTranslationUnit(Context context) -> Opt<syntax::Node>;
}
