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

