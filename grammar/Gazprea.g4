grammar Gazprea;

//file: (statement | procedure)* EOF;
file: ( termsAndConditions )* EOF ;

termsAndConditions
    : globalDecl
    | typeDefine
    | procedure
    | function
    | protoFunc
    ;

// TODO: check for precendence
expr
    : '(' expr ')'                                                  #brackExpr
    | Integer                                                       #integerExpr
    | Real                                                          #realExpr
    | (TRUE|FALSE)                                                  #boolExpr
    | NULLT                                                         #nullExpr
    | IDENTITY                                                      #identityExpr
    | Character                                                     #charExpr
    | tuple                                                         #tupleExpr
    | Identifier                                                    #identifierExpr
    | AS '<' type '>' '(' expr ')'                                  #castExpr
    | Identifier '[' expr ']'                                       #indexExpr
    | tupleMember                                                   #tupleIndexExpr
    | functionCall                                                  #functionExpr
    | left=expr DOTDOT right=expr                                   #domainExpr
    | <assoc=right> op=(ADD | SUB | NOT) expr                       #unaryExpr
    | <assoc=right> left=expr op=EXP right=expr                     #arithExpr
    | left=expr op=(MUL | DIV | REM) right=expr                     #arithExpr
    | left=expr op=(ADD | SUB) right=expr                           #arithExpr
    | left=expr op=(LESST | MORET | LESSTE | MORETE) right=expr     #compExpr
    | left=expr op=(EEQL | NEQL) right=expr                         #compExpr
    | left=expr op=AND right=expr                                   #compExpr
    | left=expr op=(OR | XOR) right=expr                            #compExpr
    ;

continueStat
    : CONTINUE SEMICOLON
    ;

breakStat
    : BREAK SEMICOLON
    ;

single_statement
    : declaration
    | assignment
    | conditional
    | loop
    | stream
    | procedureCall
    | continueStat
    | breakStat
    | returnCall
    ;

statement
    : single_statement
    | block
    ;

assignment
    : Identifier EQL (STD_INPUT | STD_OUTPUT) SEMICOLON                     #streamAss
    | Identifier EQL op=(ADD | SUB | NOT)? Identifier
    '(' (expr (COMMA expr)*)? ')' SEMICOLON                                 #procedureCallAss
    | Identifier (COMMA Identifier)+ EQL expr SEMICOLON                     #pythonTupleAss
    | Identifier EQL expr SEMICOLON                                         #normalAss
    | tupleMember EQL expr SEMICOLON                                        #tupleMemberAss
    ;

declaration
    : VAR Identifier EQL (STD_INPUT | STD_OUTPUT) SEMICOLON                 #streamDecl
    | (CONST | VAR | type) type* Identifier EQL
    op=(ADD | SUB | NOT)? Identifier'(' (expr (COMMA expr)*)? ')' SEMICOLON #procedureCallDecl
    | (CONST | VAR | type) type* Identifier EQL expr SEMICOLON               #normalDecl
    | (CONST | VAR | type) type* Identifier SEMICOLON                        #emptyDecl
    ;

conditional
    : IF expr block (ELSE IF expr block)* (ELSE block)?
    ;

loop
    : LOOP block                                                       #infiniteLoop
    | LOOP WHILE expr block                                            #predicatedLoop
    | LOOP block (WHILE expr SEMICOLON)                                #doLoop
    | LOOP Identifier IN expr (COMMA Identifier IN expr)* block        #iteratorLoop
    ;

//TODO - this ugly a block can be single non-block statement
block
    : '{' decBlock? bodyBlock? '}'
    |  single_statement
    ;

tupleMember
    : TupleIndex  (Integer | Identifier)
    ;

decBlock
    : declaration+
    ;

bodyBlock
    : statement+
    ;

stream
    : expr '->' Identifier SEMICOLON                                #outStream
    | Identifier  '<-' Identifier SEMICOLON                         #inStream
    | tupleMember '<-' Identifier SEMICOLON                         #inStream
    ;

typeDefine
    : TYPEDEF type Identifier SEMICOLON
    ;

functionCall
    : Identifier '(' (expr (COMMA expr)*)? ')'
    ;


procedureCall
    : CALL Identifier '(' (expr (COMMA expr)*)? ')' SEMICOLON
    ;

globalDecl
    : CONST (type)* Identifier EQL expr SEMICOLON
    ;

type
    : BOOLEAN
    | CHARACTER
    | INTEGER //'[' Integer ']'
    | REAL
    | Identifier
    | tupleType
    // | STRING '[' Integer ']'
    ;

param
    : (CONST | VAR)? type Identifier;

params
    : '('  (param (COMMA  param)* )? ')';

returnStat
    : RETURNS type
    ;

returnCall
    : RETURN expr? SEMICOLON
    ;

procedure
    : PROCEDURE Identifier params returnStat? block
    ;

function
    : FUNCTION Identifier params RETURNS type functionReturns
    ;

functionReturns
    : block
    | EQL expr SEMICOLON
    ;

tuple
    : '(' expr (COMMA expr)+ ')'
    ;

tupleType
    : TUPLE '(' tupleTypeIdentifier (COMMA tupleTypeIdentifier)+ ')'
    ;

tupleTypeIdentifier
    : type Identifier?
    ;

protoFunc
    :  PROCEDURE Identifier params returnStat? SEMICOLON #procProto
    |  FUNCTION  Identifier params returnStat  SEMICOLON #funcProto
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
DOT: '.';
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

Integer: [0-9][0-9_]* ;

Real
    : Integer? DOT [0-9_]+ Exponent?
    | Integer DOT? Exponent?
    ;

TupleIndex
    : Identifier '.'
    ;

Exponent: 'e' '_'* (ADD | SUB)? [0-9_]+;

Identifier: [a-zA-Z_][a-zA-Z0-9_]* ;

Boolean: (TRUE | FALSE);

Character: '\'' (~[\n]? | '\\'[0abtnr"'\\])? '\'' ;
String: '\'' .*? '\'' ;  //TODO: for part 2

// skip comments
BlockComment: '/*' (BlockComment | LineComment | .)*? '*/' -> skip ;
LineComment: '//' .*? ('\n' | EOF)-> skip ;
