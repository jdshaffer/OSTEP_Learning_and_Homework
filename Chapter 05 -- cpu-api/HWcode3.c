// --------------------------------------------------------------------
// Write another program using fork(). The child process should
// print “hello”; the parent process should print “goodbye”. You should
// try to ensure that the child process always prints first; can you do
// this without calling wait() in the parent?
// 
// RESULTS:
//    - I tried doing this by using global ints, but I forgot that
//      a forked process doesn't share memory, so my method failed.
//    - Asked Claude about it and it offered three different methods:
//         - write to a file to signal the parent it's OK to print
//         - use pipes (not there yet in the textbook)
//         - use signals (not there yet in the textbook)
//    - I will write Claude's "write to file" answer here for reference.
//      But, to be honest, it's not my work. Mine failed.
// 
// --------------------------------------------------------------------

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    // Create/truncate the file before fork
    int fd = open("HW3sync.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
    int rc = fork();
    
    if (rc < 0) {
        printf("Fork failed.\n");
        return 1;
    } 
    else if (rc == 0) {  // Child
        printf("hello\n");
        // Signal to parent by writing to file
        write(fd, "1", 1);
        close(fd);
    } 
    else {  // Parent
        char buf[1];
        // Keep checking file until child writes
        while (1) {
            lseek(fd, 0, SEEK_SET);  // Reset file position to start
            if (read(fd, buf, 1) > 0 && buf[0] == '1') {
                printf("goodbye\n");
                break;
            }
        }
        close(fd);
    }
    return 0;
}