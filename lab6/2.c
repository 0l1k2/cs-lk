#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    for (int i = 0; i < 6; i++) {
        pid_t pid = fork();
        
        if (pid < 0) {
            printf("Error!\n");
            exit(1);
        } else if (pid == 0) {
            printf("Child process %d with PID: %d\n", i+1, getpid());
            getchar();
            exit(0);
        }
    }

    // Parent process
    getchar();
    return 0;
}

