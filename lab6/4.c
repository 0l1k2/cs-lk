#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void createGrandchildProcesses() {
    for (int i = 0; i < 3; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            fprintf(stderr, "Fork failed\n");
        } else if (pid == 0) {
            // 孙子进程
            // 孙子进程不输出任何内容
            pause(); // 孙子进程挂起，等待被信号唤醒
            _exit(0); // 孙子进程结束
        }
    }

    // 等待所有孙子进程结束
    for (int i = 0; i < 3; i++) {
        wait(NULL);
    }
}

int main() {
    // 创建3个子进程
    for (int i = 0; i < 3; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            fprintf(stderr, "Fork failed\n");
        } else if (pid == 0) {
            // 子进程
            createGrandchildProcesses();
            return 0; // 子进程结束
        }
    }

    // 等待所有子进程结束
    for (int i = 0; i < 3; i++) {
        wait(NULL);
    }

    return 0;
}
