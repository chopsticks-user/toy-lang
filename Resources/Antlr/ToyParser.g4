parser grammar ToyParser;

options {
    tokenVocab = ToyLexer;
}

translationUnit
    : (definition | identifierDeclStatement)* EOF
    ;

definition
    : functionDefinition
    ;

functionDefinition
    : specifier functionPrototype qualifier blockStatement
    ;

specifier
    : visibilitySpecifier
    ;

functionPrototype
    : Identifier Colon functionPrototypePostfix
    | Fn Identifier functionPrototypePostfix
    ;

functionPrototypePostfix
    : LeftParen (parameterDeclFragment)* RightParen MinusGreater typeExpression
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
    : identifierDeclFragment+
    ;

identifierDeclStatement
    : Let identifierDeclFragmentList Semicolon
    ;

identifierDeclFragmentList
    : identifierDeclFragment (Equal expression)?  (Comma identifierDeclFragment (Equal expression)?)*
    ;

identifierDeclFragment
    : mutibilitySpecifier? Identifier Colon typeExpression
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
    : functionPrototypePostfix blockStatement
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
    : nullCoalescingExpression (QMark expression Colon ternaryExpression)?
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
    : logicalOrExpression (QMark2)*
    ;

//=============================================================================

sequenceExpression
    : ternaryExpression (Dot2 sequenceExpression (By expression)?)?
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
    : Export | Internal | Local
    ;

mutibilitySpecifier
    : Var | Const
    ;

typeExpression
    : Int | Float | Bool | String | Void | TypeIdentifier
    ;