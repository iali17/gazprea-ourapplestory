grammar Gazprea;

//file: (statement | procedure)* EOF;
file: ( typeDefine | procedure )* EOF;

// TODO: check for precendence
// TODO TUPLE
expr
    : Integer                                                       #integerExpr
    | Real                                                          #realExpr
    | (TRUE | FALSE)                                                #boolExpr
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

// todo may need to label continue and break
statement
    : declaration
    | assignment
    | conditional
    | loop
    | stream
    | procedureCall
    | CONTINUE SEMICOLON
    | BREAK SEMICOLON
    | returnCall
    | block
    ;

// TODO : remember to do a check in tuple ass where expr must be a tuple
assignment
    : Identifier EQL expr SEMICOLON                                         #normalAss
    | Identifier EQL Identifier '(' (expr (COMMA expr)*)? ')' SEMICOLON     #procedureCallAss
    | Identifier (COMMA Identifier)+ EQL expr                               #pythonTupleAss
    ;

//TODO - non initialized declaration
declaration
    : VAR Identifier EQL (STD_INPUT | STD_OUTPUT) SEMICOLON                 #streamDecl
    |  CONST? (VAR | type) type* Identifier EQL Identifier
    '(' (expr (COMMA expr)*)? ')' SEMICOLON                                 #procedureCallDecl
    | CONST? (VAR | type) type* Identifier EQL expr SEMICOLON               #normalDecl
    |  CONST? (VAR | type) type* Identifier SEMICOLON                       #emptyDecl
    ;

conditional
    : IF expr block (ELSE IF expr block)* (ELSE block)?
    ;

loop
    : LOOP block                                                       #infiniteLoop
    | LOOP (WHILE expr) block                                          #predicatedLoop
    | LOOP block (WHILE expr SEMICOLON)                                #doLoop
    | LOOP Identifier IN expr (COMMA Identifier IN expr)* block        #iteratorLoop
    ;

//TODO - this ugly a block can be single non-block statement
block
    : '{' decBlock? bodyBlock? '}'
    | ( declaration
      | assignment
      | conditional
      | loop
      | stream
      | procedureCall
      | CONTINUE SEMICOLON
      | BREAK SEMICOLON
      | returnCall
      )
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
    | INTEGER //'[' Integer ']'
    | REAL
    | Identifier
    | tupleType
    // | STRING '[' Integer ']'
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

tuple
    : '(' expr (COMMA expr)+ ')'
    ;

tupleType
    : TUPLE '(' tupleTypeIdentifier (COMMA tupleTypeIdentifier)+ ')'
    ;

tupleTypeIdentifier
    : type Identifier?
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
    : Integer? '.' [0-9_]+ Exponent?
    | Integer '.'? Exponent?
    ;

Exponent: 'e' '_'* (ADD | SUB)? [0-9_]+;

Identifier: [a-zA-Z_][a-zA-Z0-9_]* ;

Boolean: (TRUE | FALSE);

Character: '\'' (~[\n]? | '\\'[0abtnr"'\\])? '\'' ;
String: '\'' .*? '\'' ;  //TODO: for part 2

// skip comments
BlockComment: '/*' .*? '*/' -> skip ;
LineComment: '//' .*? '\n'-> skip ;
