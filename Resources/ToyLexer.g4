lexer grammar ToyLexer;

// Operators

Greater2Equal       : '>>=';
Less2Equal          : '<<=';
Dot3                : '...';
ColonEqualGreater   : ':=>';
ColonTildeGreater   : ':~>';
LessBarGreater      : '<|>';

ExclaimEqual    : '!=';
StarEqual       : '*=';
AmpersandEqual  : '&=';
BarEqual        : '|=';
HatEqual        : '^=';
FwdSlashEqual   : '/=';
PercentEqual    : '%=';
GreaterEqual    : '>=';
LessEqual       : '<=';
PlusEqual       : '+=';
MinusEqual      : '-=';
BarGreater      : '|>';
MinusGreater    : '->';
//LessMinus       : '<-';
Colon2          : '::';
Star2           : '**';
Ampersand2      : '&&';
Bar2            : '||';
Plus2           : '++';
Minus2          : '--';
FwdSlash2       : '//';
Exclaim2        : '!!';
Equal2          : '==';
Greater2        : '>>';
Less2           : '<<';
QMark2          : '??';
Dot2            : '..';
RightBracket2   : '[[';
LeftBracket2    : ']]';
EqualGreater    : '=>';

LeftParen       : '(';
RightParen      : ')';
LeftBracket     : '[';
RightBracket    : ']';
LeftBrace       : '{';
RightBrace      : '}';
Dot             : '.';
Comma           : ',';
Colon           : ':';
Semicolon       : ';';
Star            : '*';
Ampersand       : '&';
Bar             : '|';
Plus            : '+';
Minus           : '-';
FwdSlash        : '/';
Percent         : '%';
Exclaim         : '!';
Equal           : '=';
Greater         : '>';
Less            : '<';
SQuote          : '\'';
DQuote          : '"';
QMark           : '?';
Tilde           : '~';
Dollar          : '$';
Hat             : '^';
At              : '@';
Hash            : '#';

// Reserved keywords

Int         : 'Int';
Float       : 'Float';
Bool        : 'Bool';
Void        : 'Void';
String      : 'String';
Any         : 'Any';
Optional    : 'Optional' | 'Opt';
Atomic      : 'Atomic' | 'Atm';
Reactive    : 'Reactive' | 'Rxt';
Range       : 'Range' | 'Rng';
Iterator    : 'Iterator' | 'It';

Export      : 'export';
Internal    : 'internal';
Local       : 'local';
Main        : 'main';
Class       : 'class';
Super       : 'super';
Self        : 'self';
Public      : 'public';
Private     : 'private';
Protected   : 'protected';
Return      : 'return';
For         : 'for';
If          : 'if';
Else        : 'else';
Import      : 'import';
Module      : 'module';
Pure        : 'pure';
Var         : 'var';
Const       : 'const';
Let         : 'let';
Print       : 'print';
Extern      : 'extern';
Abstract    : 'abstract';
Interface   : 'interface';
True        : 'true';
False       : 'false';
Null        : 'null';
Fn          : 'fn';

// Basic

NonNegativeDecimal : NonNegativeInteger? '.' NonNegativeInteger;

NonNegativeInteger : [0-9]+;

StringLiteral : '"' StringLiteralContent? '"';

StringLiteralContent : ( EscapeSequence | ~[\\"{}] )+;

EscapeSequence : '\\' ( '"' | '\'' | '\\' | 'n' | 'r' | 't' | 'b' | 'f' | 'u' | '{' | '}' ) ;

TypeIdentifier : [A-Z][a-zA-Z0-9]*;

Identifier : [a-z][a-zA-Z0-9]*;

UnnamedIdentifier: '_';

Whitespace : [ \t\n\r]+ -> skip;

Newline: ('\r' '\n'? | '\n') -> skip;

BlockComment: '/*' .*? '*/' -> skip;

LineComment: '//' ~ [\r\n]* -> skip;