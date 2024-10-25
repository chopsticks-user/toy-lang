parser grammar ToyParser;

options {
    tokenVocab = ToyLexer;
}

translationUnit
    : definition*
    ;

definition
    : functionDefinition
    | identifierDeclStatement
    | classDefinition
    | moduleDeclStatement
    | importDeclStatement
    ;

moduleDeclStatement
    : Module Identifier (Colon2 Identifier)* Semicolon
    ;

importDeclStatement
    : Import Identifier (Colon2 Identifier)* Semicolon
    ;

classDefinition
    : visibilitySpecifier? (Class | Interface) (Colon classParentList) classDefinitionBody
    ;

classParentList
    : nullableTypeExpression (Comma nullableTypeExpression)*
    ;

classDefinitionBody
    : blockStatement
    ;

functionDefinition
    : specifier? functionPrototype qualifier? functionDefinitionBody
    ;

functionDefinitionBody
    : blockStatement
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
        RightParen MinusGreater nullableTypeExpression
    | parameterDeclFragment MinusGreater nullableTypeExpression
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
    : For LeftParen? (forRangePrototype | forRegularPrototype) RightParen? blockStatement
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
    : (Identifier | tupleExpression) Colon nullableTypeExpression (Equal expression)?
    ;

ifStatement
    : If LeftParen? ifPrototype RightParen? blockStatement (Else (blockStatement | ifStatement))?
    ;

ifPrototype
    : identifierDeclStatement? expression
    ;

returnStatement
    : Return expression? Semicolon
    ;

primaryExpression
    : lambdaExpression
    | nullableTypeExpression
    | numberExpression
    | stringExpression
    | arrayExpression
    | tupleExpression
    | typeQueryExpression
    | Identifier
    | Self
    ;

typeQueryExpression
    : LeftBracket2 expression RightBracket2
    ;

commaSeparatedExprList
    : expression (Comma expression)*
    ;

arrayExpression
    : LeftBracket commaSeparatedExprList? RightBracket
    ;

// empty tuples are not allowed
tupleExpression
    : LeftParen commaSeparatedExprList RightParen
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

// References: https://github.com/antlr/grammars-v4/blob/master/c/C.g4

postfixExpression
    : primaryExpression (
          (LeftBracket expression RightBracket)+ // array subscripting
        | tupleExpression // function call
        | BarGreater postfixExpression // pipe operator from Elixir
        | Dot Identifier // field access
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
    | Exclaim2
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

nullableTypeExpression
    : typeExpression QMark?
    ;

typeExpression
    : typeArrayExpression
    | typeTupleExpression
    | typeExpressionUnit
    ;

typeTupleExpression
    : LeftParen typeExpression (Comma typeExpression)* RightParen
    ;

typeArrayExpression
    : (LeftBracket RightBracket)+ typeExpression
    ;

typeGenericExpression
    : typeGenricPrefix Less typeExpression Greater
    ;

typeGenricPrefix
    : Atomic
    | Reactive
    | Optional
    ;

typeExpressionUnit
    : Int
    | Float
    | Bool
    | String
    | Void
    | Any
    | Atomic
    | Reactive
//    | Optional
    | TypeIdentifier
    ;