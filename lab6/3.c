#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

void handler(int signum) {
    printf("Process %d received signal %d and exiting.\n", getpid(), signum);
    _exit(0); 
}

int main() {
    signal(SIGTERM, handler);

    for (int i = 0; i < 6; i++) {
        pid_t pid = fork();
        
        if (pid < 0) {
            printf("Error creating child process.\n");
            _exit(1);
        } else if (pid == 0) {
            printf("Child process %d with PID: %d\n", i+1, getpid());
            pause(); // Wait for a signal to continue
            _exit(0);
        }
    }

    // Parent process
    getchar();
    return 0;
}

