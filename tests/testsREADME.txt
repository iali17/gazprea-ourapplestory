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

int_op_tests:
	Theses tests check if integer operations works

loop_tests:
	Theses tests check if different types of loops work which includes:
	       -do while loops
	       -while loops
	Including break, and continue and excluding them

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

tuple_tests:
	Theses test check if tuple functionalities work which includes:
	       -tuple assignment
	       -tuple declarations
	       -python style tuple assignments
	       -tuple type casting
	       -typedef tuples
	       -var tuples
	 Including null, and identity types

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
