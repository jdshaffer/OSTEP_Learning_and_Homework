// --------------------------------------------------------------------
// Write a slight modification of the previous program, this time using 
// waitpid() instead of wait(). When would waitpid() be useful?
// 
// RESULTS:
//    - Current version works like wait()
//    - NULL part can be changed to check status and options parameters
//      to get more information about the child process:
//            WIFEXITED(status)   : Did child exit normally?
//            WEXITSTATUS(status) : Get exit status of child
//            WIFSIGNALED(status) : Was child terminated by signal?
//            WTERMSIG(status)    : Get terminating signal number
//            WIFSTOPPED(status)  : Is child currently stopped?
//            WIFCONTINUED(status): Has child been continued?
//    - 0 part can be changed to parameters (options):
//            WNOHANG     : Don't block waiting
//            WUNTRACED   : Report status of stopped children
//            WCONTINUED  : Report status of continued children
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
        int wait_value = waitpid(rc, NULL, 0);
        printf("Inside the parent process.\n");
        printf("   The value returned by wait: %d\n", wait_value);
    }

    return 0;
}