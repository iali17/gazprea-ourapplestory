grammar Gazprea;

//file: (statement | procedure)* EOF;
file: procedure* EOF;

// TODO: check for precendence
// TODO TUPLE
expr
    : real                                                 #realExpr
    | Integer                                                       #integerExpr
    | NULLT                                                         #nullExpr
    | Character                                                     #charExpr
    | tuple                                                         #tupleExpr
    | Identifier                                                    #identifierExpr
    | '(' expr ')'                                                  #brackExpr
    | AS '<' type '>' '(' expr ')'                                  #castExpr
    | Identifier '[' expr ']'                                       #indexExpr
    | Identifier '.' (Identifier | Integer)                         #tupleIndexExpr
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
    | returnCall
    | returnStat
    ;

// TODO : remember to do a check in tuple ass where expr must be a tuple
assignment
    : Identifier EQL expr SEMICOLON                                         #normalAss
    | Identifier COMMA Identifier (COMMA Identifier)* EQL expr SEMICOLON    #pythonTupleAss
    ;

declaration
    : VAR Identifier EQL (STD_INPUT | STD_OUTPUT) SEMICOLON                 #streamDecl
    | CONST? (VAR | type) type* Identifier (EQL expr)? SEMICOLON               #normalDecl
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
    : declaration+
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
    | Identifier
    | tupleType
    ;

params
    : '(' (type Identifier (COMMA type Identifier)* )? ')';

returnStat
    : RETURNS type
    ;

returnCall
    : RETURN expr? SEMICOLON
    ;

procedure
    : PROCEDURE Identifier params returnStat? block
    ;

real
    : Integer? '.' (Integer | Decimal) Exponent?
    | Integer '.'? Exponent?
    ;

tuple
    : '(' expr COMMA expr (COMMA expr)* ')'
    ;

tupleType
    : TUPLE '(' type Identifier? COMMA type Identifier? (COMMA type Identifier?)* ')'
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
STD_INPUT: 'std_input()';
STD_OUTPUT: 'std_output()';
STREAM_STATE: 'stream_state';
STRING: 'string';
TRUE: 'true';
TUPLE: 'tuple';
TYPEDEF: 'typedef';
VAR: 'var';
VECTOR: 'vector';
WHILE: 'while';
XOR: 'xor';



// Skip whitespace
WS : [ \t\r\n]+ -> skip ;

Exponent: E (ADD | SUB)? Integer;
Integer: [0-9][0-9_]* ;
Decimal: [0-9_]+ ;
Identifier: [a-zA-Z_][a-zA-Z0-9_]* ;
Boolean: TRUE | FALSE;

Character: '\'' (~[\n]? | '\\'[0abtnr"'\\])? '\'' ;
String: '\'' .*? '\'' ;  //TODO: for part 2

// skip comments
BlockComment: '/*' .*? '*/' -> skip ;
LineComment: '//' .*? '\n'-> skip ;
