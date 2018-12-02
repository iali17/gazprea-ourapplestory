Test Case: errorAssTest.in
Outcome: This test case checks if the user is assigning a new type to an existing variable of another type that doesn't
         have an implicit cast. (Any other type promotion that isn't integer to real).

         - Type error: Cannot convert between char and int on line 5

         In this test case, the user is trying to assign an integer to a character, which is invalid so it returns the
         error above.

Test Case: errorOpTest.in
Outcome: This test case checks type promotion (implicit casting) between two variables of different types.

         - Type error: Cannot convert between char and int on line 3

         In this test case, the user is trying to add a char and an integer, which is invalid so it returns the error
         above.

Test Case: errorTupleAssTest.in
Outcome: This test case checks python style tuple assignment between different types using a tuple.

	     - Type error: Cannot convert between char and int on line 8

	     In this test case, the user is trying to assign to a character an integer, which is invalid so it returns
	     the error above.

Test Case: errorTupleCast.in
Outcome: This test case checks if a user is trying to cast a tuple to another type

         - Type error: Cannot convert between integer and tuple(*) on line 6

         In this test case, the user is trying to cast a tuple type to an integer type, which is invalid so it returns
         the error above.

Test Case: errorReturningVector.in
Outcome: This test case checks to see that they are returning the same size vector to what they expect
         -Type error: Cannot convert from and integer vector [3] to integer vector [2] on line 1

          In this test case, the user is returning a vector of a larger size than what is expected from the return
          statement so it returns the errors above.