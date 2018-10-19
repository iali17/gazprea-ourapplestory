grammar Gazprea;

file: .*? EOF;

//-------------------------------------------------

ADD: '+' ;
SUB: '-' ;
MUL: '*' ;
DIV: '/' ;
EQL: '=' ;
EEQL: '==';
NEQL: '!=' ;
LESST: '<' ;
MORET: '>' ;
SEMICOLON: ';' ;
DOTDOT: '..';
IF: 'if' ;
FI: 'fi' ;
INT: 'int' ;
LOOP: 'loop' ;
POOL: 'pool' ;
PRINT: 'print' ;
IN: 'in';
VECTOR: 'vector';
AND: 'and';
AS: 'as';
BOOLEAN: 'boolean';
BREAK:
BY:
CALL:
CHARACTER:
COLUMNS:
CONST:
CONTINUE:
ELSE:
FALSE:
FILTER:
FUNCTION:
IDENTITY:





Integer: [0-9]+ ;
ID: [a-zA-Z][a-zA-Z0-9]* ;

// Skip whitespace
WS : [ \t\r\n]+ -> skip ;
