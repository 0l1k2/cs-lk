/* 
 * echoserveri.c - An iterative echo server 
 */ 
/* $begin echoserverimain */
#include "csapp.h"

void echo(int connfd);

int main(int argc, char **argv) 
{
    int listenfd, connfd;// �������������listenfd (Open_listenfd�����ķ��أ�׼��������������)���Լ��ѽ���������connfd (accept�����ķ���)
    socklen_t clientlen;// socklen_t��һ���������ͣ�����ʵ��int��࣬��32λ���£�size_t��int�ĳ�����ͬ������32 bits,����64λ���£�size_t��32bits����int��64 bits���ĳ����ǲ�һ����,socket����е�accept�����ĵ����������ĳ��ȱ����int�ĳ�����ͬ�����Ǳ�����socklen_t���͡�
    struct sockaddr_storage clientaddr;  // ͨ�ýṹ��2: struct sockaddr_storage,128���ֽڣ��ýṹ���ṩ���㹻��Ŀռ����ڴ�ŵ�ַ�壬�����ڴ��Ƕ���ġ�/* Enough space for any address */  //line:netp:echoserveri:sockaddrstorage 
    char client_hostname[MAXLINE], client_port[MAXLINE];// ���ڴ���������˿ڵ��ַ���

    if (argc != 2) {// argc�ǰ������������ڵĲ��������������ʾ���иó���ʱ��Ӧ���ǡ������� �˿ڡ�����������ʽ
		fprintf(stderr, "usage: %s <port>\n", argv[0]);//fprintf��C/C++�е�һ����ʽ���⺯����λ��ͷ�ļ�<cstdio>�У��������Ǹ�ʽ�������һ�����ļ��У�����ԭ��Ϊint fprintf( FILE *stream, const char *format, [ argument ]...)��fprintf()��������ָ���ĸ�ʽ(format)���������(stream)д������(argument)��
		exit(0);
    }

    listenfd = Open_listenfd(argv[1]);// �򿪼�����׼�������������󣬷��ؼ���������listenfd
    while (1) { // ִ��ѭ��
		// ��Ҫ����Ĵ�С
		clientlen = sizeof(struct sockaddr_storage); 
		// 1���ȴ����ӣ�������������accept�������ȴ�listenfd�ϵ���������
		// 2���ͻ���ͨ��connect�������ӷ�������������������listenfd
		// 3��������������������accept��������connfd������֮ǰ��clientaddr�л����Ͽͻ��˵��׽��ֵ�ַ��
		// 4���ͻ��˴�connect�������أ�����clientfd��connfd֮�佨��������
		connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen); // ע��clientaddrҪ��ǿ������ת����SA���ͣ���struct sockaddr
		// ���׽��ֵ�ַclientaddrת��Ϊ��Ӧ�������Ͷ˿��ַ���
        Getnameinfo((SA *) &clientaddr, clientlen, client_hostname, MAXLINE, 
                    client_port, MAXLINE, 0);
		// ������ӵ��ͻ��˵���Ϣ
        printf("Connected to (%s, %s)\n", client_hostname, client_port);
		// ����connfd�ϵ������пͻ��˵���Ϣ���ͻ���
		echo(connfd);
		// �ر�����connfd�ϵ�����
		Close(connfd);
    }
    exit(0);
}
/* $end echoserverimain */
