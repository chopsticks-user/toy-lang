lexer grammar ToyLexer;

// Operators

Greater2Equal  : '>>=';
Less2Equal     : '<<=';
Dot3           : '...';

ExclaimEqual   : '!=';
StarEqual      : '*=';
AmpersandEqual : '&=';
BarEqual       : '|=';
HatEqual       : '^=';
FwdSlashEqual  : '/=';
PercentEqual   : '%=';
GreaterEqual   : '>=';
LessEqual      : '<=';
PlusEqual      : '+=';
MinusEqual     : '-=';
BarGreater     : '|>';
MinusGreater   : '->';
LessMinus      : '<-';
Colon2         : '::';
Star2          : '**';
Ampersand2     : '&&';
Bar2           : '||';
Plus2          : '++';
Minus2         : '--';
FwdSlash2      : '//';
Exclaim2       : '!!';
Equal2         : '==';
Greater2       : '>>';
Less2          : '<<';
QMark2         : '??';
Dot2           : '..';

LeftParen      : '(';
RightParen     : ')';
LeftBracket    : '[';
RightBracket   : ']';
LeftBrace      : '{';
RightBrace     : '}';
Dot            : '.';
Comma          : ',';
Colon          : ':';
Semicolon      : ';';
Star           : '*';
Ampersand      : '&';
Bar            : '|';
Plus           : '+';
Minus          : '-';
FwdSlash       : '/';
Percent        : '%';
Exclaim        : '!';
Equal          : '=';
Greater        : '>';
Less           : '<';
SQuote         : '\'';
DQuote         : '"';
QMark          : '?';
Tilde          : '~';
Dollar         : '$';
Hat            : '^';

// Reserved keywords

Int         : 'Int';
Float       : 'Float';
Bool        : 'Bool';
String      : 'String';
Void        : 'Void';

Export      : 'export';
Internal    : 'internal';
Local       : 'local';
Fn          : 'fn';
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
By          : 'by';
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

// Basic

NonNegativeDecimal : NonNegativeInteger? '.' NonNegativeInteger;

NonNegativeInteger : [0-9]+;

StringLiteral : '"' StringLiteralContent? '"';

StringLiteralContent : ( EscapeSequence | ~[\\"{}] )+;

EscapeSequence : '\\' ( '"' | '\'' | '\\' | 'n' | 'r' | 't' | 'b' | 'f' | 'u' | '{' | '}' ) ;

TypeIdentifier : [A-Z][a-zA-Z0-9]*;

Identifier : [a-z][a-zA-Z0-9]*;

Whitespace : [ \t\n\r]+ -> skip;

Newline: ('\r' '\n'? | '\n') -> skip;

BlockComment: '/*' .*? '*/' -> skip;

LineComment: '//' ~ [\r\n]* -> skip;