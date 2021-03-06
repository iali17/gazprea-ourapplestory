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
    | Integer                                                       #integerExpr
    | Real                                                          #realExpr
    | (TRUE|FALSE)                                                  #boolExpr
    | NULLT                                                         #nullExpr
    | IDENTITY                                                      #identityExpr
    | Character                                                     #charExpr
    | tuple                                                         #tupleExpr
    | TupleIndex                                                    #tupleIndexExpr
    | <assoc=right> filter Real                                     #filterIndexExpr
    | matrix                                                        #matrixExpr
    | String                                                        #stringExpr
    | emptyVector                                                   #emptyVectorExpr
    | vector                                                        #vectorExpr
    | expr '[' expr (COMMA expr)? ']'                               #indexExpr
    | expr '[' MUL COMMA expr ']'                                   #fullLeftIndexExpr
    | expr '[' expr COMMA MUL ']'                                   #fullRightIndexExpr
    | expr '[' MUL (COMMA MUL)? ']'                                 #fullIndexExpr
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
    | left=expr DOTDOT right=expr                                   #intervalExpr
    | IntervalThing right=expr                                      #intervalExpr
    | <assoc=right> op=(ADD | SUB | NOT) expr                       #unaryExpr
    | <assoc=right> left=expr op=EXP right=expr                     #arithExpr
    | left=expr op=(MUL | DIV | REM) right=expr                     #arithExpr
    | left=expr op=(ADD | SUB) right=expr                           #arithExpr
    | left=expr op=DOTPRODUCT right=expr                            #dotProductOrMatMultExpr
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
    : Identifier EQL (STD_INPUT | STD_OUTPUT) SEMICOLON                                            #streamAss
    | (Identifier | TupleIndex) (COMMA (Identifier | TupleIndex))+ EQL expr SEMICOLON              #pythonTupleAss
    | Identifier EQL expr SEMICOLON                                                                #normalAss
    | TupleIndex EQL expr SEMICOLON                                                                #tupleMemberAss
    | expr '[' expr (COMMA expr)? ']' EQL expr SEMICOLON                                           #indexAssign
    | expr '[' MUL COMMA expr ']'     EQL expr SEMICOLON                                           #fullLeftIndexAssign
    | expr '[' expr COMMA MUL ']'     EQL expr SEMICOLON                                           #fullRightIndexAssign
    | expr '[' MUL (COMMA MUL)? ']'   EQL expr SEMICOLON                                           #fullIndexAssign
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

explicitVectorExtension
    : '[' (left=expr | MUL) ']'
    ;

explicitMatrixExtension
    : '[' (left=expr | MUL) rightExtension ']'
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

// todo commas
generator
    : '[' Identifier IN expr (COMMA Identifier IN expr)? '|' expr ']'
    ;

filter
    : '[' Identifier IN expr AMPERSAND expr (COMMA expr)* ']'
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
    | intervalType
    | REAL
    | Identifier
    | tupleType
    | STRING
    | vectorType
    | matrixType
    // | STRING '[' Integer ']'
    ;

intervalType
    : INTEGER INTERVAL
    ;

param
    : (CONST | VAR)? type Identifier
    | (CONST | VAR)? type Identifier extension?;

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
    | PROCEDURE Identifier params returnStat oneLineFunction
    ;

function
    : FUNCTION Identifier params returnStat functionReturns
    ;

oneLineFunction
    : EQL expr SEMICOLON
    ;

functionReturns
    : block
    | oneLineFunction
    ;

tuple
    : '(' expr (COMMA expr)+ ')'
    ;

emptyVector
    : '[' ']'
    ;

vector
    : '[' ( expr (COMMA expr)* )? ']';

matrix
    : '[' ( vector (COMMA vector)* ) ']';

tupleType
    : TUPLE '(' tupleTypeIdentifier (COMMA tupleTypeIdentifier)+ ')'
    ;

vectorType
    : (BOOLEAN VECTOR) extension?
    | (CHARACTER VECTOR) extension?
    | (INTEGER VECTOR) extension?
    | (REAL VECTOR) extension?
    | (BOOLEAN | CHARACTER | INTEGER | REAL) explicitVectorExtension
    ;

matrixType
    : (BOOLEAN MATRIX) extension?
    | (CHARACTER MATRIX) extension?
    | (INTEGER MATRIX) extension?
    | (REAL MATRIX) extension?
    | (BOOLEAN | CHARACTER | INTEGER | REAL) explicitMatrixExtension
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

//Character: '\'' (~[\n]? | '\\'[0abtnr"'\\])? '\'' ;
Character : '\'' ValidCharacters? '\'';
String: '"' ValidCharacters* '"' ;


ValidCharacters
    :   ~["\\\r\n]
    |   '\\' [0'"abnrt\\]
    ;

// skip comments
BlockComment: '/*' (BlockComment | LineComment | .)*? '*/' -> skip ;
LineComment: '//' .*? ('\n' | EOF)-> skip ;
