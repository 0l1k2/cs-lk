/*
 * echoclient.c - An echo client
 */
/* $begin echoclientmain */
#include "csapp.h"

int main(int argc, char **argv) 
{
    int clientfd;// �����׽���������
    char *host, *port, buf[MAXLINE]; //����ͻ����������˿ںͿ���̨���뻺����
	char *bye = "bye\n"; // �ͻ����˳�����
    rio_t rio; // ��������

    if (argc != 3) { //�ж������и�ʽ
		fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);
		exit(0);
    }
    host = argv[1]; // �Ӳ����еõ�������host
    port = argv[2]; // �Ӳ����еõ�������port

    clientfd = Open_clientfd(host, port); // ���ӷ����������ش򿪵��׽���������clientfd
    Rio_readinitb(&rio, clientfd);// ���׽�������clientfd�ϰ�rio���ݽṹ

    while (Fgets(buf, MAXLINE, stdin) != NULL) { // �������̨���������루���뵽��buf�У�
		Rio_writen(clientfd, buf, strlen(buf));// ��clientfd�׽������ӣ�дbuf�е���Ϣ�����͵���������
		Rio_readlineb(&rio, buf, MAXLINE);// ��ȡ��������������Ϣ������buf��
		Fputs(buf, stdout);// ��buf�е�����д������̨
		if(strcmp(buf,bye) == 0) // �ж��Ƿ�Ϊ�˳��������"bye"���˳�
			break;
    }
    Close(clientfd); //line:netp:echoclient:close
    exit(0);
}
/* $end echoclientmain */
