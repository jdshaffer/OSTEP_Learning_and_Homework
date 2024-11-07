// --------------------------------------------------------------------
// Now write a program that uses wait() to wait for the child process
// to finish in the parent. What does wait() return? What happens if
// you use wait() in the child?
// 
// RESULTS:
//    - wait() needs to come before any code to execute
//    - wait() returns the pid of the child process
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
        int cpid =  (int) getpid();
        printf("Inside the child process.\n");
        printf("   The child pid is %d\n", cpid);
    } else {
        // INSIDE PARENT
        int wait_value = wait(NULL);
        printf("Inside the parent process.\n");
        printf("   The value returned by wait: %d\n", wait_value);
    }

    return 0;
}