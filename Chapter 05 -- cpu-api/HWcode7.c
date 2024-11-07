// --------------------------------------------------------------------
// Write a program that creates a child process, and then in the child
// closes standard output (STDOUT_FILENO). What happens if the child
// calls printf() to print some output after closing the descriptor?
// 
// RESULTS:
//    - As expected. Once I closed the standard output in the child
//      process, it could no longer print to the screen.
//    - If I uncomment the opening of the standard output, then even
//      the "lost" printf runs because it was stored in buffer while
//      the standard output was closes.
// 
// --------------------------------------------------------------------

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>   // Needed for wait()

int main(int argc, char *argv[])
{
    int rc = fork();
    if (rc < 0){
        printf("Fork failed.\n");
    } else if (rc == 0){
        // INSIDE CHILD
        printf("Inside the child process.\n");
        printf("Going to close the standard output...\n");
    	close(STDOUT_FILENO);         
        printf("Closed the standard output.\n");
//        open("/dev/tty", O_WRONLY);
//        printf("Opened the standard output.\n");        
    } else {
        // INSIDE PARENT
        printf("Parent processing here...\n");
    }

    return 0;
}