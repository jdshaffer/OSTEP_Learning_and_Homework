//----------------------------------------------------------------------
// Simple program to time system calls.
// Jeffrey D. Shaffer
// 2024-11-08
//
// First  test -- open-read-close an empty text file 10,000 times
// Second test -- mkdir-rm-dir 10,000 times
//
// RESULTS ON OSX:
// Average time for open/read/close system calls: 3.63 microseconds
// Average time for   mkdir/rmdir   system calls: 4.84 microseconds
//
// RESULTS ON RASPBERRY PI 4b+
// Average time for open/read/close system calls: 5.42 microseconds
// Average time for   mkdir/rmdir   system calls: 18.49 microseconds
//
//----------------------------------------------------------------------
// Jds Learning Notes: 
//    <fcntl.h> (File Control)   -- Contains definitions for file access modes
//    <unistd.h> (UNIX Standard) -- Core UNIX system calls interface
//    <sys/stat.h> -- Focuses on file status and properties
//    struct timeval {
//        time_t      tv_sec;     /* seconds */
//        suseconds_t tv_usec;    /* microseconds */
//    };
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
    
    // Get start time for first test
    gettimeofday(&start, NULL);
    
    // Run first test
    for (int i=0; i < number_of_loops; i++) {
        int fd = open("test.txt", O_RDONLY);    // Open a file read-only
        read(fd, buffer, sizeof(buffer) - 1);   // Read in the file
        close(fd);                              // Close the file
    }        

    // Get end time for first test
    gettimeofday(&end, NULL);
    
    // Display average time per system call
    elapsed_time = (end.tv_usec - start.tv_usec) / 3;  // Divide by 3 bc have 3 calls
    printf("\nAverage time for open/read/close system calls: %.2f microseconds\n", elapsed_time / number_of_loops); 

    // Get start time for second test
    gettimeofday(&start, NULL);

    // Run second test
    for (int i=0; i < number_of_loops; i++) {
        mkdir("test_dir", 0755);                    // Create a new directory
        rmdir("test_dir");                          // Delete a new directory
    }    

    // Get end time for second test
    gettimeofday(&end, NULL);
    
    elapsed_time = (end.tv_usec - start.tv_usec) / 2;  // Divide by 2 bc have 2 calls
    printf("Average time for   mkdir/rmdir   system calls: %.2f microseconds\n\n", elapsed_time / number_of_loops); 

    return 0;
}