parser grammar ToyParser;

options {
    tokenVocab = ToyLexer;
}

translationUnit
    : definition* EOF
    ;

definition
    : functionDefinition
    | identifierDeclStatement
    | classDefinition
//    | moduleDeclStatement
//    | importDeclStatement
    ;

classDefinition
    : specifier? (Class | Interface) (Colon classParentList) classDefinitionBody
    ;

classParentList
    : typeExpression (Comma typeExpression)*
    ;

classDefinitionBody
    : blockStatement
    ;

functionDefinition
    : specifier? functionPrototype qualifier? functionDefinitionBody
    ;

functionDefinitionBody
    : blockStatement
    | expression Semicolon
    ;

specifier
    : visibilitySpecifier
    ;

functionPrototype
    : Identifier Colon functionPrototypePostfix
    | Fn Identifier functionPrototypePostfix
    ;

functionPrototypePostfix
    : LeftParen parameterDeclFragment (Comma parameterDeclFragment)*
        RightParen MinusGreater typeExpression
    | parameterDeclFragment MinusGreater typeExpression
    ;

statement
    : assignStatement
    | identifierDeclStatement
    | controlStatement
    | returnStatement
    | expressionStatement
    | blockStatement
    ;

blockStatement
    : LeftBrace (statement)* RightBrace
    ;

expressionStatement
    : expression Semicolon
    ;

assignStatement
    : expression assignmentOperators expression Semicolon
    ;

controlStatement
    : forStatement
    | ifStatement
    ;

forStatement
    : For LeftParen (forRangePrototype | forRegularPrototype) RightParen blockStatement
    ;

forRangePrototype
    : identifierDeclStatement Colon expression
    ;

forRegularPrototype
    : (identifierDeclStatement | Semicolon) (expressionStatement | Semicolon) expression?
    ;

parameterDeclFragment
    : mutibilitySpecifier? identifierDeclFragment
    ;

identifierDeclStatement
    : mutibilitySpecifier identifierDeclFragmentList Semicolon
    ;

identifierDeclFragmentList
    : identifierDeclFragment (Comma identifierDeclFragment)*
    ;

identifierDeclFragment
    : Identifier Colon typeExpression (Equal expression)?
    ;

ifStatement
    : If LeftParen ifPrototype RightParen blockStatement (Else (blockStatement | ifStatement))?
    ;

ifPrototype
    : identifierDeclStatement? expression
    ;

returnStatement
    : Return expression Semicolon
    ;

primaryExpression
    : Identifier
    | typeExpression
    | numberExpression
    | stringExpression
    | lambdaExpression
    | LeftParen expression RightParen
    | Self
    ;

lambdaExpression
    : functionPrototypePostfix qualifier? blockStatement
    ;

stringExpression
    : StringLiteral
    | DQuote (StringLiteralContent | stringPlaceholder)* DQuote
    ;

stringPlaceholder
    : LeftBrace expression RightBrace
    ;

argumentList
    : expression (Comma expression)*
    ;

// References: https://github.com/antlr/grammars-v4/blob/master/c/C.g4

postfixExpression
    : primaryExpression (
          LeftBracket expression RightBracket
        | LeftParen argumentList? RightParen
        | BarGreater postfixExpression // Pipe operator from Elixir
        | Dot Identifier
        | postfixUnaryOperators
      )*
    ;

prefixUnaryExpression
    : prefixUnaryOperators postfixExpression
    ;

multiplicativeExpression
    : prefixUnaryExpression ((Star | FwdSlash | Percent) prefixUnaryExpression)*
    ;

additiveExpression
    : multiplicativeExpression ((Plus | Minus) multiplicativeExpression)*
    ;

shiftExpression
    : additiveExpression ((Less2 | Greater2) additiveExpression)*
    ;

relationalExpression
    : shiftExpression ((Less | Greater | GreaterEqual | LessEqual) shiftExpression)*
    ;

equalityExpression
    : relationalExpression ((Equal2 | ExclaimEqual) relationalExpression)*
    ;

andExpression
    : equalityExpression (Ampersand equalityExpression)*
    ;

exclusiveOrExpression
    : andExpression (Hat andExpression)*
    ;

inclusiveOrExpression
    : exclusiveOrExpression (Bar exclusiveOrExpression)*
    ;

logicalAndExpression
    : inclusiveOrExpression (Ampersand2 inclusiveOrExpression)*
    ;

logicalOrExpression
    : logicalAndExpression (Bar2 logicalAndExpression)*
    ;

ternaryExpression
    : nullCoalescingExpression (QMark nullCoalescingExpression Colon nullCoalescingExpression)?
    ;

expression
    : sequenceExpression
    ;

//=============================================================================

// References: other languages

iteratorExpression // from Go
    : prefixUnaryExpression (LessMinus iteratorExpression)*
    ;

exponentialExpression // from python
    : iteratorExpression (Star2 exponentialExpression)*
    ;

nullCoalescingExpression // from JavaScript
    : logicalOrExpression (QMark2 logicalOrExpression)*
    ;

//=============================================================================

sequenceExpression
    : ternaryExpression (Dot2 ternaryExpression (By ternaryExpression)?)?
    ;

// Terminals

assignmentOperators
    : StarEqual
    | FwdSlashEqual
    | PercentEqual
    | PlusEqual
    | MinusEqual
    | Greater2Equal
    | Less2Equal
    | AmpersandEqual
    | HatEqual
    | BarEqual
    | Equal
    ;

prefixUnaryOperators
    : Exclaim
    | Tilde
    | Plus
    | Minus
    ;

postfixUnaryOperators
    : Plus2
    | Minus2
    ;

numberExpression
    : integerLiteral
    | floatLiteral
    ;

integerLiteral
    : NonNegativeInteger
    ;

floatLiteral
    : NonNegativeDecimal
    ;

qualifier
    : Pure
    ;

visibilitySpecifier
    : Export
    | Internal
    | Local
    ;

mutibilitySpecifier
    : Var
    | Const
    ;

typeExpression
    : Int
    | Float
    | Bool
    | String
    | Void
    | TypeIdentifier
    ;