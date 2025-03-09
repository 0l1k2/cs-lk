/*
 * echoclient.c - An echo client
 */
/* $begin echoclientmain */
#include "csapp.h"

int main(int argc, char **argv) 
{
    int clientfd;// 定义套接字描述符
    char *host, *port, buf[MAXLINE]; //定义客户端主机、端口和控制台输入缓冲区
	char *bye = "bye\n"; // 客户端退出命令
    rio_t rio; // 读缓冲区

    if (argc != 3) { //判断命令行格式
		fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);
		exit(0);
    }
    host = argv[1]; // 从参数中得到服务器host
    port = argv[2]; // 从参数中得到服务器port

    clientfd = Open_clientfd(host, port); // 连接服务器，返回打开的套接字描述符clientfd
    Rio_readinitb(&rio, clientfd);// 在套接字连接clientfd上绑定rio数据结构

    while (Fgets(buf, MAXLINE, stdin) != NULL) { // 如果控制台进行了输入（输入到了buf中）
		Rio_writen(clientfd, buf, strlen(buf));// 向clientfd套接字连接，写buf中的消息（发送到服务器）
		Rio_readlineb(&rio, buf, MAXLINE);// 读取服务器发来的消息，读到buf中
		Fputs(buf, stdout);// 将buf中的内容写到控制台
		if(strcmp(buf,bye) == 0) // 判断是否为退出命令，若是"bye"则退出
			break;
    }
    Close(clientfd); //line:netp:echoclient:close
    exit(0);
}
/* $end echoclientmain */
