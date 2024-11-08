//----------------------------------------------------------------------
// Simple program to time context switching.
// This one was written by Claude as I had no idea how to do it. Sadly.
// 
// First  test -- 
// Second test -- 
//
//----------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <pthread.h>
#include <mach/thread_policy.h>
#include <mach/thread_act.h>

#define NUM_ITERATIONS 10000
#define WARM_UP_ITERATIONS 1000

// Function to set thread affinity on macOS (use only one CPU)
void set_thread_affinity(int cpu) {
    thread_affinity_policy_data_t policy = { cpu };
    thread_port_t mach_thread = pthread_mach_thread_np(pthread_self());
    thread_policy_set(mach_thread, THREAD_AFFINITY_POLICY,
                     (thread_policy_t)&policy, THREAD_AFFINITY_POLICY_COUNT);
}

int main() {
    int pipe1[2], pipe2[2];  // Two pipes for bidirectional communication
    char buf[1];
    pid_t pid;
    struct timeval start, end;
    long long total_time = 0;
    
    // Create pipes
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("pipe creation failed");
        exit(1);
    }
    
    pid = fork();
    if (pid == -1) {
        perror("fork failed");
        exit(1);
    }
    
    if (pid == 0) {  // Child process
        set_thread_affinity(0);  // Bind to CPU 0
        
        close(pipe1[1]);  // Close write end of pipe1
        close(pipe2[0]);  // Close read end of pipe2
        
        // Warm-up phase
        for (int i = 0; i < WARM_UP_ITERATIONS; i++) {
            read(pipe1[0], buf, 1);
            write(pipe2[1], "x", 1);
        }
        
        // Actual measurement
        for (int i = 0; i < NUM_ITERATIONS; i++) {
            read(pipe1[0], buf, 1);
            write(pipe2[1], "x", 1);
        }
        
        exit(0);
    } else {  // Parent process
        set_thread_affinity(0);  // Bind to CPU 0
        
        close(pipe1[0]);  // Close read end of pipe1
        close(pipe2[1]);  // Close write end of pipe2
        
        // Warm-up phase
        for (int i = 0; i < WARM_UP_ITERATIONS; i++) {
            write(pipe1[1], "x", 1);
            read(pipe2[0], buf, 1);
        }
        
        // Start timing
        gettimeofday(&start, NULL);
        
        // Actual measurement
        for (int i = 0; i < NUM_ITERATIONS; i++) {
            write(pipe1[1], "x", 1);
            read(pipe2[0], buf, 1);
        }
        
        // End timing
        gettimeofday(&end, NULL);
        
        // Calculate total time
        total_time = (end.tv_usec - start.tv_usec);
        
        // Calculate average time per context switch
        double avg_switch_time = (double)total_time / (NUM_ITERATIONS * 2);
        printf("Average context switch time: %.2f microseconds\n", avg_switch_time);
        
        wait(NULL);  // Wait for child to finish
    }
    
    return 0;
}
