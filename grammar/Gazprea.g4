grammar Gazprea;

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
    | left=expr (DOTDOT right=expr)                                 #intervalExpr
    | IntervalThing right=expr                                      #intervalExpr
    | Integer                                                       #integerExpr
    | Real                                                          #realExpr
    | (TRUE|FALSE)                                                  #boolExpr
    | NULLT                                                         #nullExpr
    | IDENTITY                                                      #identityExpr
    | Character                                                     #charExpr
    | tuple                                                         #tupleExpr
    | TupleIndex                                                    #tupleIndexExpr
    | matrix                                                        #matrixExpr
    | String                                                        #stringExpr
    | vector                                                        #vectorExpr
    | Identifier '[' expr (COMMA expr)? ']'                         #indexExpr
    | Identifier                                                    #identifierExpr
    | AS '<' type '>' '(' expr ')'                                  #castExpr
    | functionCall                                                  #functionExpr
    | generator                                                     #generatorExpr
    | filter                                                        #filterExpr
    | vectorLength                                                  #vectorLengthExpr
    | rowLength                                                     #rowLengthExpr
    | colLength                                                     #colLengthExpr
    | reverse                                                       #reverseExpr
    | streamState                                                   #streamStateExpr
    | <assoc=right> op=(ADD | SUB | NOT) expr                       #unaryExpr
    | <assoc=right> left=expr op=EXP right=expr                     #arithExpr
    | left=expr op=(MUL | DIV | REM) right=expr                     #arithExpr
    | left=expr op=(ADD | SUB) right=expr                           #arithExpr
    | left=expr op=DOTPRODUCT right=expr                            #dotProductExpr
    | left=expr op=BY right=expr                                    #byExpr
    | left=expr op=(LESST | MORET | LESSTE | MORETE) right=expr     #compExpr
    | left=expr op=(EEQL | NEQL) right=expr                         #compExpr
    | left=expr op=AND right=expr                                   #compExpr
    | left=expr op=(OR | XOR) right=expr                            #compExpr
    | left=expr op=CONCAT right=expr                                #concatExpr
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
    | Identifier (COMMA Identifier)+ EQL expr SEMICOLON                     #pythonTupleAss
    | Identifier EQL expr SEMICOLON                                         #normalAss
    | TupleIndex EQL expr SEMICOLON                                         #tupleMemberAss
    ;

declaration
    : VAR Identifier EQL (STD_INPUT | STD_OUTPUT) SEMICOLON                                                    #streamDecl
    | (CONST | VAR | type) type* Identifier extension? EQL expr SEMICOLON                                      #normalDecl
    | (CONST | VAR | type) type* Identifier extension? SEMICOLON                                               #emptyDecl
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

decBlock
    : declaration+
    ;

bodyBlock
    : statement+
    ;

extension
    : '[' (left=expr | MUL) rightExtension? ']'
    ;

rightExtension
    : COMMA (right=expr | MUL)
    ;

stream
    : expr '->' Identifier SEMICOLON                                #outStream
    | Identifier  '<-' Identifier SEMICOLON                         #inStream
    | TupleIndex '<-' Identifier SEMICOLON                          #inStream
    ;

streamState
    : STREAM_STATE '(' Identifier ')'
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

generator
    : '[' Identifier IN expr (COMMA Identifier IN expr)? '|' expr ']'
    ;

filter
    : '[' Identifier IN expr (AMPERSAND expr)+ ']'
    ;

vectorLength
    : LENGTH '(' expr ')'
    ;

rowLength
    : ROWS '(' expr ')'
    ;

colLength
    : COLUMNS '(' expr ')'
    ;

reverse
    : REVERSE '(' expr ')'
    ;

globalDecl
    : CONST (type)* Identifier EQL expr SEMICOLON
    ;

type
    : BOOLEAN
    | CHARACTER
    | INTEGER //'[' Integer ']'
    | INTERVAL
    | REAL
    | Identifier
    | tupleType
    | STRING
    | vectorType
    | matrixType
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

vector
    : '[' ( expr (COMMA expr)* )? ']';

matrix
    : '[' ( vector (COMMA vector)* )? ']';

tupleType
    : TUPLE '(' tupleTypeIdentifier (COMMA tupleTypeIdentifier)+ ')'
    ;

vectorType
    : (BOOLEAN VECTOR) extension?
    | (CHARACTER VECTOR) extension?
    | (INTEGER VECTOR) extension?
    | (REAL VECTOR) extension?
    ;

matrixType
    : BOOLEAN MATRIX
    | CHARACTER MATRIX
    | INTEGER MATRIX
    | REAL MATRIX
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
CONCAT: '||' ;
AMPERSAND: '&';
DOTPRODUCT: '**';
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

Integer: [0-9] Digit? ;

TupleIndex
    : Identifier DOT Identifier
    | Identifier DOT Integer
    ;

Real
    : Integer? DOT Digit Exponent?
    | Integer DOT? Exponent?
    ;


//Interval: IntervalThing (Integer | Identifier) ;

IntervalThing
    : (Identifier | Integer) '..'
    ;

fragment Exponent: 'e' '_'* (ADD | SUB)? Digit;
fragment Digit: [0-9_]+;

Identifier: [a-zA-Z_][a-zA-Z0-9_]* ;

Boolean: (TRUE | FALSE);

Character: '\'' (~[\n]? | '\\'[0abtnr"'\\])? '\'' ;
String: '"' .*? '"' ;  //TODO: for part 2

// skip comments
BlockComment: '/*' (BlockComment | LineComment | .)*? '*/' -> skip ;
LineComment: '//' .*? ('\n' | EOF)-> skip ;
