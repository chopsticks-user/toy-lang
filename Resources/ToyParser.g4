parser grammar ToyParser;

options {
    tokenVocab = ToyLexer;
}

translationUnit
    : moduleDeclStatement importDeclStatement* definition*
    ;

moduleDeclStatement
    : Module Identifier (Colon2 Identifier)* Semicolon
    ;

importDeclStatement
    : Import Identifier (Colon2 Identifier)* Semicolon
    ;

definition
    : functionDefinition
    | identifierDefStatement
    ;

functionDefinition
    : visibilitySpecifier? Fn Identifier Colon functionPrototype blockStatement
    ;

statement
    : assignStatement
    | identifierDefStatement
    | returnStatement
    | expressionStatement
    | blockStatement
    ;

blockStatement
    : LeftBrace statement* RightBrace
    ;

expressionStatement
    : expression Semicolon
    ;

assignStatement
    : expression assignmentOperators expression Semicolon
    ;

loopStatement
    : loopStatementPrefix ColonTildeGreater blockStatement
    ;

loopStatementPrefix
    : Let (identifierDeclFragment | structuredBinding) Less Tilde expression
    ;

matchStatement
    : ((LeftParen commaSeparatedExprList RightParen) | Identifier) ColonEqualGreater
        LeftBrace ifThenStatement* matchWildcardStatement RightBrace
    ;

matchWildcardStatement
    : UnnamedIdentifier ifThenStatementPostfix
    ;

ifThenStatement
    : expression ifThenStatementPostfix
    ;

ifThenStatementPostfix
    : EqualGreater (blockStatement | (statement Semicolon))
    ;

returnStatement
    : Return expression? Semicolon
    ;

// Identifier constructs

functionPrototype
    : functionPrototypePrefix MinusGreater functionPrototypePostfix?
    ;

functionPrototypePrefix
    : LeftParen structuredBinding? RightParen
    ;

functionPrototypePostfix
    : typeTupleExpression
    | functionPrototypePrefix
    ;

identifierDefStatement
    : Let identifierDeclFragment (Equal expression)? Semicolon
    | Let structuredBinding Equal commaSeparatedExprList Semicolon
    ;

structuredBinding
    : structuredBindingFragment (Comma structuredBindingFragment)+
    ;

structuredBindingFragment
    : identifierDeclFragment
    | UnnamedIdentifier
    | UnnamedIdentifier Colon typeExpression
    ;

identifierDeclFragment
    : Identifier Colon typeExpression
    ;

// Expressions

expression
    : sequenceExpression
    ;

sequenceExpression
    : ternaryExpression (Dot2 ternaryExpression (At ternaryExpression)?)?
    ;

ternaryExpression
    : nullCoalescingExpression (QMark nullCoalescingExpression Colon nullCoalescingExpression)?
    ;

nullCoalescingExpression // from JavaScript
    : logicalOrExpression (QMark2 logicalOrExpression)*
    ;

logicalOrExpression
    : logicalAndExpression (Bar2 logicalAndExpression)*
    ;

logicalAndExpression
    : inclusiveOrExpression (Ampersand2 inclusiveOrExpression)*
    ;

inclusiveOrExpression
    : exclusiveOrExpression (Bar exclusiveOrExpression)*
    ;

exclusiveOrExpression
    : andExpression (Hat andExpression)*
    ;

andExpression
    : equalityExpression (Ampersand equalityExpression)*
    ;

equalityExpression
    : relationalExpression ((Equal2 | ExclaimEqual) relationalExpression)*
    ;

relationalExpression
    : shiftExpression ((Less | Greater | GreaterEqual | LessEqual) shiftExpression)*
    ;

shiftExpression
    : additiveExpression ((Less2 | Greater2) additiveExpression)*
    ;

additiveExpression
    : multiplicativeExpression ((Plus | Minus) multiplicativeExpression)*
    ;

multiplicativeExpression
    : exponentialExpression ((Star | FwdSlash | Percent) exponentialExpression)*
    ;

exponentialExpression // from python
    : prefixUnaryExpression (Star2 prefixUnaryExpression)*
    ;

prefixUnaryExpression
    : prefixUnaryOperators postfixExpression
    ;

postfixExpression
    : primaryExpression (
          (LeftBracket expression RightBracket)+ // array subscripting
        | LeftParen commaSeparatedExprList? RightParen // function call
        | BarGreater postfixExpression // pipe operator from Elixir
        | Dot Identifier // field access
        | postfixUnaryOperators
      )*
    ;

primaryExpression
    : lambdaExpression
    | typeExpression
    | numberExpression
    | stringExpression
    | arrayExpression
    | typeQueryExpression
    | LeftParen expression RightParen
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

lambdaExpression
    : functionPrototype blockStatement
    ;

stringExpression
    : StringLiteral
    | DQuote (StringLiteralContent | stringPlaceholder)* DQuote
    ;

stringPlaceholder
    : LeftBrace expression RightBrace
    ;

//iteratorExpression // from Go
//    : prefixUnaryExpression (LessMinus iteratorExpression)*
//    ;

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
    | Hash
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

visibilitySpecifier
    : Export
    | Internal
    | Local
    ;

// Types

typeExpression
    : Dollar? (
            typeArrayExpression
            | typeExpressionUnit
            | typeGenericExpression
            | typeFunctionExpression
      ) QMark?
    ;

typeFunctionExpression
    : typeTupleExpression MinusGreater typeTupleExpression
    ;

typeTupleExpression
//    : LeftParen typeExpression (Comma typeExpression)+ RightParen
    : LeftParen typeExpression (Comma typeExpression)* RightParen
    ;

typeArrayExpression
    : LeftBracket RightBracket typeExpression
    ;

typeGenericExpression
    : typeGenericPrefix Less (typeExpression | typeTupleExpression)  Greater
    ;

typeGenericPrefix
    : Atomic
    | Reactive
    | Range
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
    | Range
    | TypeIdentifier
    ;