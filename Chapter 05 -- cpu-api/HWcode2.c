// --------------------------------------------------------------------
// Write a program that opens a file (with the open() system call)
// and then calls fork() to create a new process. Can both the child
// and parent access the file descriptor returned by open()? What
// happens when they are writing to the file concurrently, i.e., at the
// same time?
// 
// Note -- Remember to delete the text_sample.txt file before 
//         running so old results do not get mixed in.
// 
// RESULTS:
// 	- opening the file again OVERWRITES the previously written contents
// 	- I tried using a loop to write from both parent and child at the
//    same time. No errors, but again the child overwrites previous
//    parent writes.
// 
// --------------------------------------------------------------------

#include <fcntl.h>     // For open()
#include <unistd.h>    // For close()
#include <stdio.h>     // For perror()
#include <string.h>    // For strings
#include <sys/wait.h>  // For wait()


int main(int argc, char *argv[])
{
    // Opening for write only, create if doesn't exist
    int fd = open("text_sample.txt", O_WRONLY | O_CREAT, 0644);
    printf("File id: %d\n", fd);
	printf("Writing to fd (parent)...\n");
	int i = 0;
	while (i < 10){  // Try to force both parent and child to write at same time
	    char *text0 = "Writing to parent-opened file from parent!\n";
	    write(fd, text0, strlen(text0));
		i++;	
	}

	// FORK
	printf("Starting fork...\n");
	int rc = fork();

	if (rc < 0){
		printf("Fork failed\n");
	} else if (rc == 0){  // INSIDE FORK
		// OPEN FILE IN FORK
		printf("Trying to open the same file...\n");
		int fd2 = open("text_sample.txt", O_WRONLY | O_CREAT, 0644);
	    printf("File id (inside child): %d\n", fd);
	
		// WRITE TO CHILD OPENED FILE
		printf("Writing to fd2 (inside child)...\n");
		int j = 0;
		while (j < 5){  // Try to force both parent and child to write at same time
		    char *text1 = "Writing to child-opened file from child!\n";
		    write(fd2, text1, strlen(text1));
			j++;	
		}
		// WRITE TO PARENT OPENED FILE
		printf("Writing to fd (inside child)...\n");
	    char *text2 = "Writing to parent-opened file from child AGAIN!\n";
	    write(fd, text2, strlen(text2));
	} else {   // BACK INSIDE PARENT
	    char *text3 = "Writing to parent-opened file from parent AGAIN!\n";
	    write(fd, text3, strlen(text3));
	}

	return 0;
}