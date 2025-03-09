#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int pid = getpid();
    int parent_pid = pid; // 保存父进程的初始pid
    for (int i = 0; i < 10; i++) {
        if (fork() == 0) {
            // 子进程输出 hello pid
            printf("hello %d\n", parent_pid);
            break;
        } else {
            // 父进程输出 world pid
            wait(NULL); // 等待子进程结束
            pid++; // 父进程的pid逐渐增加
            printf("world %d\n", pid);
        }
    }

    return 0;
}
