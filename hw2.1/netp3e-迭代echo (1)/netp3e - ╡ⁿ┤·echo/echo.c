/*
 * echo - read and echo text lines until client closes connection
 */
/* $begin echo */
#include "csapp.h"

void echo(int connfd) 
{
    size_t n; 
    char buf[MAXLINE]; 
    rio_t rio;

    Rio_readinitb(&rio, connfd);
	while((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) { //line:netp:echo:eof
		printf("server received %d bytes\n", (int)n);
		printf(buf, MAXLINE);// 服务器显示客户端发来的消息
		Rio_writen(connfd, buf, n);// 服务器将buf的内容发送到客户端（这里可以处理buf内容，控制回显给客户端的消息内容）
    }
    
}
/* $end echo */

