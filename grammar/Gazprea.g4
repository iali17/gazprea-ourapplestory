grammar Gazprea;

file: .*? EOF;




statement: declaration
    | assignment
    | conditional
    | loop
    | block
    | stream
    ;



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

AND: 'and';
AS: 'as';
BOOLEAN: 'boolean';
BREAK: 'break';
BY: 'by';
CALL: 'call';
CHARACTER: 'character';
COLUMNS: 'columns';
CONST: 'const';
CONTINUE: 'continue';
ELSE: 'else';
FALSE: 'false';
FILTER: 'filter';
FUNCTION: 'function';
IDENTITY: 'identity';
IF: 'if' ;
IN: 'in';
INTEGER: 'integer';
INTERVAL: 'interval';
LENGTH: 'length';
LOOP: 'loop' ;
MATRIX: 'matrix';
NOT: 'not';
NULL: 'null';
OR: 'or';
PROCEDURE: 'procedure';
REAL: 'real';
RETURN: 'return';
RETURNS: 'returns';
REVERSE: 'reverse';
ROWS: 'rows';
STD_INPUT: 'std_input';
STD_OUTPUT: 'std_output';
STREAM_STATE: 'stream_state';
STRING: 'string';
TRUE: 'true';
TUPLE: 'tuple';
TYPEDEF: 'tuple';
VAR: 'var';
VECTOR: 'vector';
WHILE: 'while';
XOR: 'xor';

POOL: 'pool' ;
FI: 'fi' ;
INT: 'int' ;
PRINT: 'print' ;


COMMENT: '/*' .*? '*/' ;

Integer: [0-9]+ ;
IDENTIFIER: [a-zA-Z][a-zA-Z0-9]* ;

// Skip whitespace
WS : [ \t\r\n]+ -> skip ;
