#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LENGTH 100

void printHelp() {
    printf("-----------------\n");
    printf("内部命令：\n");
    printf("help：打印帮助信息\n");
    printf("xuehao：打印学号\n");
    printf("exit：退出\n");
    printf("------------------\n");
    printf("外部命令：\n");
    printf("exec + 命令：执行外部命令\n");
    printf("--------------------------\n");
}

void printXuehao() {
    printf("2022214189\n");
}

int main() {
    char command[MAX_LENGTH];

    printf("welcome to my shell!\n");
    while (1) {
        printf("myshell> ");
        fgets(command, MAX_LENGTH, stdin);

        // 去除结尾的换行符
        command[strcspn(command, "\n")] = 0;

        if (strcmp(command, "help") == 0) {
            printHelp();
        } else if (strcmp(command, "exit") == 0) {
            printf("bye\n");
            break;
        } else if (strcmp(command, "xuehao") == 0) {
            printXuehao();
        } else if (strncmp(command, "exec", 4) == 0) {
            char *cmd = command + 5;
            int status;
            pid_t pid = fork();
            if (pid == 0) {
                execlp(cmd, cmd, NULL);
                exit(0);
            } else {
                waitpid(pid, &status, 0);
            }
        } else {
            printf("无效命令\n");
        }
    }

    return 0;
}
