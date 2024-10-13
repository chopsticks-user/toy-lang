lexer grammar ToyLexer;

// Operators

Greater2Equal  : '>>=';
Less2Equal     : '<<=';
Dot3           : '...';

ExclaimEqual   : '!=';
StarEqual      : '*=';
AmpersandEqual : '&=';
BarEqual       : '|=';
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
//While       : 'while';
If          : 'if';
Switch      : 'switch';
Case        : 'case';
Default     : 'default';
Import      : 'import';
Module      : 'module';
Pure        : 'pure';
Var         : 'var';
Const       : 'const';
Print       : 'print';
Extern      : 'extern';
Abstract    : 'abstract';
Interface   : 'interface';

// Basic

TypeIdentifier : [A-Z][a-zA-Z0-9]*;

Identifier : [a-z][a-zA-Z0-9]*;

// Ignore whitespace
Whitespace : [ \t\n\r]+ -> skip;

Newline: ('\r' '\n'? | '\n') -> skip;

BlockComment: '/*' .*? '*/' -> skip;

LineComment: '//' ~ [\r\n]* -> skip;