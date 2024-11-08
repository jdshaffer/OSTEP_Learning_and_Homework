//----------------------------------------------------------------------
// Simple program to time system calls.
// First  test -- open-read-close an empty text file 10,000 times
// Second test -- mkdir-rm-dir 10,000 times
//
// RESULTS ON OSX:
// Average time for open/read/close system calls: 3.63 microseconds
// Average time for   mkdir/rmdir   system calls: 4.84 microseconds
//
// RESULTS ON RASPI
// Average time for open/read/close system calls: 5.42 microseconds
// Average time for   mkdir/rmdir   system calls: 18.49 microseconds
//
//----------------------------------------------------------------------


#include <stdio.h>
#include <sys/time.h>  // For gettimeofday()
#include <fcntl.h>     // For open()
#include <unistd.h>    // For read(), write(), close()
#include <sys/stat.h>  // For mkdir()


int main(int argc, char *argv[]) {
    struct timeval start, end;
    double elapsed_time = 0;
    int number_of_loops = 10000;
    char buffer[100];
    
    // Get start time
    gettimeofday(&start, NULL);
    
    for (int i=0; i < number_of_loops; i++) {
        int fd = open("test.txt", O_RDONLY); // Open a file read-only
        ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
        close(fd);  // Close the file
    }        

    // Get end time
    gettimeofday(&end, NULL);
    
    // Display average time per system call
    elapsed_time = (end.tv_usec - start.tv_usec) / 3;
    printf("\nAverage time for open/read/close system calls: %.2f microseconds\n", elapsed_time / number_of_loops); 

    // Get start time
    gettimeofday(&start, NULL);

    for (int i=0; i < number_of_loops; i++) {
        mkdir("test_dir", 0755);
        rmdir("test_dir");    
    }    

    // Get end time
    gettimeofday(&end, NULL);
    
    elapsed_time = (end.tv_usec - start.tv_usec) / 2;
    printf("Average time for   mkdir/rmdir   system calls: %.2f microseconds\n\n", elapsed_time / number_of_loops); 

    return 0;
}