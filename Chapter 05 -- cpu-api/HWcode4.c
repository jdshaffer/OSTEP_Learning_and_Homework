// --------------------------------------------------------------------
// Write a program that calls fork() and then calls some form of
// exec() to run the program /bin/ls. See if you can try all of the
// variants of exec(), including (on Linux) execl(), execle(),
// execlp(), execv(), execvp(), and execvpe(). Why do
// you think there are so many variants of the same basic call?
// 
// Jds Note: These same variants are available on OSX.
// 
// RESULTS:
//    - exec()  and execvpe() do not work under C99
//    - the others work fine.
//    - My notes are added as comments below
// 
// --------------------------------------------------------------------

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    int rc = fork();
    if (rc < 0){
        printf("Fork failed.\n");
    } else if (rc == 0){
        // CHILD PROCESS
        printf("Entered the child process...\n");

        // Experiment with all the variants of exec() to call /bin/ls
        // DOESN'T WORK -- Invalid in C99
        // exec("/bin/ls", "ls", NULL, NULL);

        // WORKS -- Takes in a list of arguments
        //       -- (cannot pass in *.txt as the shell takes care of the * part)
         execl("/bin/ls", "ls", "-l", NULL);

        // WORKS -- Takes in a list of arguments
        //       -- Take in set specific environment settings
        //       -- Setting the path tells LS what it can find, not what execle() can find
        // char *envp[] = {"PATH=/bin", "HOME=/home/user", NULL};
        // execle("/bin/ls", "ls", "-l", NULL, envp);

        // WORKS -- Takes in a list of arguments
        //       -- Searches the current environment PATH for the program to run
        // execlp("ls", "ls", "-l", NULL);

        // WORKS -- Takes in a vector (an array) of arguments
        // char *args[] = {"ls", "-l", NULL};     // <-- the array of arguments to be passed
        // execv("/bin/ls", args);

        // WORKS -- Takes in a vector (an array) of arguments
        //       -- Searches the current environment PATH for the program to run
        // char *args[] = {"ls", "-l", NULL};
        // execvp("ls", args);

        // DOESN'T WORK -- Takes in a vector of arguments
        //              -- Take in set specific environment settings
        //              -- BUT doesn't work under C99 
        // char *envp[] = {"PATH=/bin", "HOME=/home/user", NULL};
        // char *args[] = {"ls", "-l", NULL};
        // execvpe("ls", args, envp);

    } else {
        // PARENT PROCESS
        printf("Back to parent process -- finishing up.\n");
    }
    
    return 0;
}