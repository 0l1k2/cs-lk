/* 
 * echoserveri.c - An iterative echo server 
 */ 
/* $begin echoserverimain */
#include "csapp.h"

void echo(int connfd);

int main(int argc, char **argv) 
{
    int listenfd, connfd;// 定义监听描述符listenfd (Open_listenfd方法的返回，准备接收连接请求)，以及已接连描述符connfd (accept方法的返回)
    socklen_t clientlen;// socklen_t是一种数据类型，它其实和int差不多，在32位机下，size_t和int的长度相同，都是32 bits,但在64位机下，size_t（32bits）和int（64 bits）的长度是不一样的,socket编程中的accept函数的第三个参数的长度必须和int的长度相同。于是便有了socklen_t类型。
    struct sockaddr_storage clientaddr;  // 通用结构体2: struct sockaddr_storage,128个字节，该结构体提供了足够大的空间用于存放地址族，而且内存是对齐的。/* Enough space for any address */  //line:netp:echoserveri:sockaddrstorage 
    char client_hostname[MAXLINE], client_port[MAXLINE];// 用于存放主机、端口的字符串

    if (argc != 2) {// argc是包括程序本身在内的参数个数，这里表示运行该程序时，应该是“程序本身 端口”的命令行形式
		fprintf(stderr, "usage: %s <port>\n", argv[0]);//fprintf是C/C++中的一个格式化库函数，位于头文件<cstdio>中，其作用是格式化输出到一个流文件中；函数原型为int fprintf( FILE *stream, const char *format, [ argument ]...)，fprintf()函数根据指定的格式(format)，向输出流(stream)写入数据(argument)。
		exit(0);
    }

    listenfd = Open_listenfd(argv[1]);// 打开监听，准备接收连接请求，返回监听描述符listenfd
    while (1) { // 执行循环
		// 需要具体的大小
		clientlen = sizeof(struct sockaddr_storage); 
		// 1、等待连接（服务器阻塞在accept方法，等待listenfd上的连接请求）
		// 2、客户端通过connect方法连接服务器，创建连接请求到listenfd
		// 3、服务器处理连接请求，accept方法返回connfd（返回之前，clientaddr中会填上客户端的套接字地址）
		// 4、客户端从connect方法返回，现在clientfd和connfd之间建立了连接
		connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen); // 注意clientaddr要被强制类型转换成SA类型，即struct sockaddr
		// 将套接字地址clientaddr转换为对应的主机和端口字符串
        Getnameinfo((SA *) &clientaddr, clientlen, client_hostname, MAXLINE, 
                    client_port, MAXLINE, 0);
		// 输出连接到客户端的信息
        printf("Connected to (%s, %s)\n", client_hostname, client_port);
		// 回显connfd上的连接中客户端的信息到客户端
		echo(connfd);
		// 关闭连接connfd上的连接
		Close(connfd);
    }
    exit(0);
}
/* $end echoserverimain */
