grammar Gazprea;

//file: (statement | procedure)* EOF;
file: procedure* EOF;

// TODO: add labels for many of the expr and etc (ex left=expr)
// TODO TUPLE DOESNT WORK
expr
    : Real                                                          #realExpr
    | Integer                                                       #integerExpr
    | NULLT                                                         #nullExpr
    | Identifier                                                    #identifierExpr
    | '(' expr ')'                                                  #brackExpr
    | '(' expr COMMA expr (COMMA expr)* ')'                         #tupleExpr
    | AS '<' type '>' '(' expr ')'                                  #castExpr
    | Identifier '[' expr ']'                                       #indexExpr
    | Identifier '.' (Integer | Identifier)                         #tupleIndexExpr
    | left=expr DOTDOT right=expr                                   #domainExpr
    | <assoc=right> op=(ADD | SUB | NOT) expr                       #unaryExpr
    | <assoc=right> left=expr EXP right=expr                        #exponentExpr
    | left=expr op=(MUL | DIV | REM) right=expr                     #mulExpr
    | left=expr op=(ADD | SUB) right=expr                           #addExpr
    | left=expr op=(LESST | MORET | LESSTE | MORETE) right=expr     #lessExpr
    | left=expr op=(EEQL | NEQL) right=expr                         #eqlExpr
    | left=expr AND right=expr                                      #andExpr
    | left=expr op=(OR | XOR) right=expr                            #orExpr
    ;

statement
    : declaration
    | assignment
    | conditional
    | loop
    | block
    | stream
    | typeDefine
    | procedureCall
    | CONTINUE SEMICOLON
    | BREAK SEMICOLON
    | RETURN SEMICOLON
    ;

declaration
    : CONST? VAR? type? Identifier EQL expr SEMICOLON
    ;

// TODO : remember to do a check in tuple ass where expr must be a tuple
assignment
    : Identifier EQL expr SEMICOLON                                         #normalAss
    | Identifier (COMMA Identifier)+ EQL expr SEMICOLON                     #tupleAss
    ;

conditional
    : IF expr block (ELSE IF block)* (ELSE block)?
    ;

loop
    : LOOP statement?                                                       #infiniteLoop
    | LOOP (WHILE expr) statement?                                          #predicatedLoop
    | LOOP statement? (WHILE expr SEMICOLON)                                #doLoop
    | LOOP Identifier IN expr (COMMA Identifier IN expr)* statement?        #iteratorLoop
    ;

block
    : '{' decBlock? bodyBlock? '}'
    ;

decBlock
    :  declaration+
    ;

bodyBlock
    : statement+
    ;

stream
    : expr '->' Identifier SEMICOLON                                #outStream
    | Identifier '<-' Identifier SEMICOLON                          #inStream
    ;

// TODO: probably could be better
typeDefine
    : TYPEDEF type Identifier SEMICOLON
    ;

procedureCall
    : CALL Identifier '(' (expr (COMMA expr)*)? ')' SEMICOLON
    ;

type
    : BOOLEAN
    | CHARACTER
    | INTEGER
    | REAL
    | TUPLE '(' type COMMA type (COMMA type)* ')'
    | Identifier
    ;


procedure
    : PROCEDURE Identifier '(' (type Identifier (COMMA type Identifier)* )? ')' block
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
E: 'e';
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
NULLT: 'null';
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
TYPEDEF: 'typedef';
VAR: 'var';
VECTOR: 'vector';
WHILE: 'while';
XOR: 'xor';

BlockComment: '/*' .*? '*/' ;
LineComment: '//' .*? '\n' ;

Integer: [0-9][0-9_]* ;  // TODO: refer to 7.3.4 in spec
Identifier: [a-zA-Z][a-zA-Z0-9]* ;
Boolean: TRUE | FALSE;
Real: ([0-9][0-9_]*)? '.' [0-9_]* (E (ADD | SUB)? Integer)?;
Character: '\'' [a-zA-Z] '\'' ;




// Skip whitespace
WS : [ \t\r\n]+ -> skip ;