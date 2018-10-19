grammar Gazprea;

file: .*? EOF;


expr
    : Integer                                                       #integerExpr
    | Identifier                                                    #identifierExpr
    | '(' expr ')'                                                  #brackExpr
    | left=expr DOTDOT right=expr                                   #domainExpr
    // TODO: generators and filters and matrices
    | <assoc=right> op=(ADD | SUB | NOT) expr                       #unaryExpr
    | <assoc=right> left=expr EXP right=expr                        #expExpr
    | left=expr op=(MUL | DIV | REM) right=expr                     #mulExpr
    | left=expr op=(ADD | SUB) right=expr                           #addExpr
    // TODO: by
    | left=expr op=(LESST | MORET | LESSTE | MORETE) right=expr     #lessExpr
    | left=expr op=(EEQL | NEQL) right=expr                         #eqlExpr
    | left=expr AND right=expr                                      #andExpr
    | left=expr op=(OR | XOR) right=expr                            #orExpr
    // TODO : ||
    ;

statement
    : assignment
    | conditional
    | loop
    | block
    | stream
    | CONTINUE
    | BREAK
    | RETURN
    ;

assignment
    : specifier? type? Identifier EQL expr
    | specifier? type? Identifier EQL 'null'
    ;

conditional
    : IF expr statement* ( ELSE IF statement )* (ELSE statement)?
    ;

// TODO: predicated and iterative loop probably needs changing
loop
    : LOOP statement*                                                       #infiniteLoop
    | LOOP WHILE expr? statement* (WHILE expr SEMICOLON)?                   #predicatedLoop
    | LOOP Identifier IN expr (COMMA Identifier IN expr)* statement*        #iteratorLoop
    ;

block
    : '{' statement* '}'
    ;

stream
    : expr '->' 'out'
    | Identifier '<-' 'inp';


specifier
    : CONST
    | VAR
    ;

type
    : BOOLEAN
    | CHARACTER
    | INTEGER
    | REAL
    | TUPLE
    | INTERVAL
    | VECTOR
    | STRING
    | MATRIX
    ;





//-------------------------------------------------

ADD: '+' ;
SUB: '-' ;
MUL: '*' ;
DIV: '/' ;
REM: '%' ;
EXP: '^';
EQL: '=' ;
EEQL: '==';
NEQL: '!=' ;
LESST: '<' ;
MORET: '>' ;
LESSTE: '<=' ;
MORETE: '>=' ;
SEMICOLON: ';' ;
DOTDOT: '..';
COMMA: ',';

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

COMMENT: '/*' .*? '*/' ;

Integer: [0-9]+ ;
Identifier: [a-zA-Z][a-zA-Z0-9]* ;

// Skip whitespace
WS : [ \t\r\n]+ -> skip ;
