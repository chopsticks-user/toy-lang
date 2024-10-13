parser grammar ToyParser;

options {
    tokenVocab = ToyLexer;
}

translationUnit
    : definition* EOF
    ;

definition
    : functionDefinition
    ;

functionDefinition
    : specifier functionPrototype qualifier block
    ;

specifier
    : visibilitySpecifier
    ;

functionPrototype
    : Identifier Colon LeftParen (parameterDecl)* RightParen
    ;

block
    : emptyBlock
    | LeftBrace (statement | block)* RightBrace
    ;

emptyBlock
    : LeftBrace RightBrace
    ;

statement
    : emptyStatement
    | assignStatement
    | variableLocalDecl
    | controlStatement
    | returnStatement
    | expressionStatement
    ;

emptyStatement
    : Semicolon
    ;

expressionStatement
    : expression Semicolon
    ;

assignStatement
    : expression Equal expression Semicolon
    ;

controlStatement
    : forStatement
    ;

forStatement
    : For (forRangePrototype | forRegularPrototype) block
    ;

forRangePrototype
    : variableLocalDecl Colon expression
    ;

forRegularPrototype
    : (variableLocalDecl | Semicolon) (expressionStatement | Semicolon) expression?
    ;

parameterDecl
    : variableLocalDecl
    ;

variableLocalDecl
    : mutibilitySpecifier (variableTypeDecl (Equal expression)?)+ Semicolon
    ;

variableTypeDecl
    : Identifier Colon typeSpecifier
    ;

//ifStatement
//    :
//    ;

returnStatement
    : Return expression Semicolon
    ;

expression
    : primaryExpression
    ;

primaryExpression
    : Identifier
    | Self
    | LeftParen expression RightParen
    ;

qualifier
    : Pure?
    ;

visibilitySpecifier
    : (Export | Internal | Local)?
    ;

mutibilitySpecifier
    : Var | Const
    ;

typeSpecifier
    : Int | Float | Bool | String | Void | TypeIdentifier
    ;