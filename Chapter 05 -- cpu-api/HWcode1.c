// ------------------------------------------------------------------------
// Write a program that calls fork(). Before calling fork(), have the
// main process access a variable (e.g., x) and set its value to some-
// thing (e.g., 100). What value is the variable in the child process?
// What happens to the variable when both the child and parent change
// the value of x?
// ------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int x = 0;
	printf("X (parent) = %d\n", x);
	x = 100;
	printf("X (parent) = %d\n", x);

	int rc = fork();
	if (rc < 0){
		printf("Fork failed\n");
	} else if (rc == 0){
		printf("X (child)  = %d\n", x);
		x = 42;
		printf("X (child)  = %d\n", x);
	} else {
		wait(NULL);
		printf("X (parent) = %d\n", x);
	}
	return 0;
}