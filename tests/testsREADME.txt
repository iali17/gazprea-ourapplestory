bool_tests:
    these tests tests bool
        - truth table tests

cast_op_tests:
	Theses tests check if implicit and explicit casting works.

cond_tests:
	Theses tests check if conditionals work which include:
	       -Breaks
	       -Continues
	       -else
	       -else if
	       -if
	       -nested conditionals

fail:
	Literally means these tests are meant to fail

file_tests:
	Theses tests check if global variables work (and immutable) and if type inferencing works

filter_tests:
    Theses test check if filters work

function_tests:
    Theses tests check if functions perform the way they are supposed to work

generator_tests:
    Theses tests check if generators work

int_op_tests:
	Theses tests check if integer operations works

interval_tests:
    Theses tests check if intervals work

loop_tests:
	Theses tests check if different types of loops work which includes:
	       -do while loops
	       -while loops
	Including break, and continue and excluding them

matrix_arith_comp_tests:
    These tests check if matrix comparisons work

matrix_op_tests:
    These tests check if matrix arithmetic operations work

matrix_tests:
    Thees tests check if general matrix stuff works

other_tests:
	Theses tests check other functions which include:
	       -print
	       -typedef
	       -grammar
	       -procedures
	       -real operations
	       -typedef operations

procedure_tests:
    These tests tests procedures
        - struct
        - slam

slice_tests:
    Theses test check if slice works as intended which includes:
        -vector slice
        -matrix slice

stream_state:
    Theses tests check if stream state works

string_tests:
    Thees tests check if string types work

tuple_tests:
	Theses test check if tuple functionalities work which includes:
	       -tuple assignment
	       -tuple declarations
	       -python style tuple assignments
	       -tuple type casting
	       -typedef tuples
	       -var tuples
	 Including null, and identity types

typedef_tests:
    These test check if typedef works which include:
        -vectors
        -matrixs
        -scalars

vector_ops_test:
    These test check if vector operations work

vector_tests:
    These test check if general features of vectors work

should work for realz:
    int .
    int       exp
    int . dec
    int .     exp
    int . dec exp
        . dec
        . dec exp

    int _ int _ . _ int _ e _ + _ int _




should not:
        .     exp
