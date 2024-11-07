// --------------------------------------------------------------------
// Write a program that creates two children, and connects the 
// standard output of one to the standard input of the other, using 
// the pipe() system call.
// 
// RESULTS:
//    - A lot more complicated than I imagined, but perhaps not 
//      too hard to understand. Maybe we don't need to close off
//      the unused pipe ends, but for safe-coding sake we did.
//    - Tested it -- yes, not closing the pipes still works.
//    - I still don't like how "messy" it is to store and sent
//      strings, but that cannot be helped.
// 
// --------------------------------------------------------------------

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char buffer[256];  // An array to hold character data (to share between forks)

    int pid1, pid2;    // Make int variables to hold fork pids
    int pipefd[2];     // Make an array to hold two file descriptors for pipe
    pipe(pipefd);      // Make the pipe (and get the two file descriptors
                       // pipefd[0] is used for reading
                       // pipefd[1] is used for writing

    pid1 = fork();

    if (pid1 < 0){
        printf("First fork failed.\n");
        
    } else if (pid1 == 0){
        // INSIDE FIRST CHILD
        printf("(Inside the FIRST child process)\n");
        close(pipefd[0]);
        printf("Closed the pipe read-end.\n");

        // WRITE TO THE PIPE
        char *message = "Hello from first child process!";
        write(pipefd[1], message, strlen(message));
        printf("Message written to pipe.\n");

        // CLOSE THE WRITE-END
        close(pipefd[1]);
        printf("Closed the pipe write-end.\n\n");

    } else {
        // INSIDE PARENT
        printf("\n");
        printf("Inside the parent process.\n");
        pid2 = fork();
        
        if (pid2 < 0){
            printf("Second fork failed.\n");
            
        } else if (pid2 == 0){
            // INSIDE SECOND CHILD
            printf("(Inside the SECOND child process)\n");
            
            // CLOSE THE UNUSED WRITE-END OF THE PIPE
            close(pipefd[1]);
            printf("Closed the pipe write-end.\n");
            
            // READ FROM THE PIPLE AND STORE IN BUFFER
            read(pipefd[0], buffer, 256 - 1);
            printf("Read the message from the read-end.\n");

            // PRINT RECEIVED MESSAGE TO THE SCREEN
            printf("Received the message: %s\n", buffer);
            
            // CLOSE THE READ-END OF THE PIPE
            close(pipefd[0]);
            printf("Closed the pipe read-end.\n\n");
            
        } else {
            // INSIDE PARENT (AGAIN)
            printf("Inside the parent process, again.\n\n");
        }
    }

    return 0;
}