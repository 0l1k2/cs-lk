#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip_icmp.h>
#include <netinet/ip.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/select.h>
#include <getopt.h>
#include <sys/signal.h>  // 引入信号处理相关头文件

#define RECV_BUF_SIZE 4096

unsigned short checksum(unsigned short *buf, int len);
void ping(const char *ipaddr, int count);
void sigint_handler(int signum);  // 声明信号处理函数

int sent_count = 0;  // 全局变量，记录发送的ICMP消息个数
int received_count = 0;  // 全局变量，记录接收的ICMP消息个数

int main(int argc, char **argv) {
    int count = 0;
    int opt;
    char *ipaddr = NULL;

    // 使用getopt解析命令行参数，记录初始返回值
    int getopt_return = getopt(argc, argv, "c:");
    if (getopt_return == -1) {
        // 如果没有提供参数选项，按默认持续发送ICMP消息，注册信号处理函数
        struct sigaction sa;
        sa.sa_handler = sigint_handler;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;
        if (sigaction(SIGINT, &sa, NULL) == -1) {
            perror("sigaction");
            exit(1);
        }
    } else {
        while (getopt_return!= -1) {
            switch (getopt_return) {
                case 'c':
                    if (optarg == NULL) {
                        // 如果-c选项后没参数，且下一次调用getopt返回-1（表示参数解析结束），说明就是-c缺少参数的情况，直接返回，让getopt输出默认提示并结束程序
                        if (getopt(argc, argv, "") == -1) {
                            return 1;
                        }
                        // 否则继续正常解析参数
                        getopt(argc, argv, "c:");
                    } else {
                        count = atoi(optarg);
                        // 判断count值是否超出设定范围（这里假设范围是1到100，可按需调整）
                        if (count < 1 || count > 100) {
                            fprintf(stderr, "Error: The count value should be between 1 and 100.\n");
                            exit(1);
                        }
                    }
                    break;
                default:
                    // 当遇到其他无法识别的选项时，输出错误提示并退出
                    fprintf(stderr, "Error: Unrecognized option '%c'.\n", getopt_return);
                    exit(1);
            }
            getopt_return = getopt(argc, argv, "c:");
        }
    }

    // 检查是否获取到了目的主机地址
    if (optind >= argc) {
        fprintf(stderr, "Error: You must specify a destination host or IP address.\n");
        exit(1);
    }
    ipaddr = argv[optind];

    if (count == 0) {
        // 如果没有指定-c选项及count值，按默认持续发送模式调用ping函数
        ping(ipaddr, -1);
    } else {
        ping(ipaddr, count);
    }
    return 0;
}

void ping(const char *ipaddr, int count) {
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0) {
        perror("socket");
        exit(1);
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ipaddr);

    int seq = 0;
    if (count == -1) {
        // 持续发送模式（未使用-c选项时）
        while (1) {
            struct icmphdr icmp_hdr;
            icmp_hdr.type = ICMP_ECHO;
            icmp_hdr.code = 0;
            icmp_hdr.checksum = 0;
            icmp_hdr.un.echo.id = getpid();
            icmp_hdr.un.echo.sequence = seq++;
            icmp_hdr.checksum = checksum((unsigned short *)&icmp_hdr, sizeof(icmp_hdr));

            struct timeval send_time;
            gettimeofday(&send_time, NULL);
            if (sendto(sockfd, &icmp_hdr, sizeof(icmp_hdr), 0, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
                perror("sendto");
                exit(1);
            }
            sent_count++;  // 发送成功，计数加1

            char recv_buf[RECV_BUF_SIZE];
            struct sockaddr_in recv_addr;
            socklen_t addr_len = sizeof(recv_addr);

            fd_set read_fds;
            FD_ZERO(&read_fds);
            FD_SET(sockfd, &read_fds);
            struct timeval timeout;
            timeout.tv_sec = 2;
            timeout.tv_usec = 0;

            int select_result = select(sockfd + 1, &read_fds, NULL, NULL, &timeout);
            if (select_result < 0) {
                perror("select");
                exit(1);
            } else if (select_result == 0) {
                printf("Timeout waiting for reply for sequence %d\n", seq - 1);
                continue;
            }

            ssize_t n;
            if ((n = recvfrom(sockfd, recv_buf, sizeof(recv_buf), 0, (struct sockaddr *)&recv_addr, &addr_len)) < 0) {
                perror("recvfrom");
                exit(1);
            }

            int iphlen, icmplen;
            struct ip *ip = (struct ip *) recv_buf;

            iphlen = ip->ip_hl << 2;
            icmplen = n - iphlen;

            struct icmphdr *icmp_reply = (struct icmphdr *)(recv_buf + sizeof(struct iphdr));
            if (icmp_reply->type == ICMP_ECHOREPLY && icmp_reply->un.echo.id == getpid()) {
                struct timeval recv_time;
                gettimeofday(&recv_time, NULL);
                double rtt = (recv_time.tv_sec - send_time.tv_sec) * 1000.0 + (recv_time.tv_usec - send_time.tv_usec) / 1000.0;
                printf("%d bytes from %s: icmp_seq=%d, ttl=%d, rtt=%.3fms\n", icmplen, inet_ntoa(ip->ip_src), icmp_reply->un.echo.sequence, ip->ip_ttl, rtt);
                received_count++;  // 接收成功，计数加1
            }
        }
    } else {
        // 使用-c选项指定次数发送模式
        for (int i = 0; i < count; i++) {
            struct icmphdr icmp_hdr;
            icmp_hdr.type = ICMP_ECHO;
            icmp_hdr.code = 0;
            icmp_hdr.checksum = 0;
            icmp_hdr.un.echo.id = getpid();
            icmp_hdr.un.echo.sequence = seq++;
            icmp_hdr.checksum = checksum((unsigned short *)&icmp_hdr, sizeof(icmp_hdr));

            struct timeval send_time;
            gettimeofday(&send_time, NULL);
            if (sendto(sockfd, &icmp_hdr, sizeof(icmp_hdr), 0, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
                perror("sendto");
                exit(1);
            }
            sent_count++;  // 发送成功，计数加1

            char recv_buf[RECV_BUF_SIZE];
            struct sockaddr_in recv_addr;
            socklen_t addr_len = sizeof(recv_addr);

            fd_set read_fds;
            FD_ZERO(&read_fds);
            FD_SET(sockfd, &read_fds);
            struct timeval timeout;
            timeout.tv_sec = 2;
            timeout.tv_usec = 0;

            int select_result = select(sockfd + 1, &read_fds, NULL, NULL, &timeout);
            if (select_result < 0) {
                perror("select");
                exit(1);
            } else if (select_result == 0) {
                printf("Timeout waiting for reply for sequence %d\n", seq - 1);
                continue;
            }

            ssize_t n;
            if ((n = recvfrom(sockfd, recv_buf, sizeof(recv_buf), 0, (struct sockaddr *)&recv_addr, &addr_len)) < 0) {
                perror("recvfrom");
                exit(1);
            }

            int iphlen, icmplen;
            struct ip *ip = (struct ip *) recv_buf;

            iphlen = ip->ip_hl << 2;
            icmplen = n - iphlen;

            struct icmphdr *icmp_reply = (struct icmphdr *)(recv_buf + sizeof(struct iphdr));
            if (icmp_reply->type == ICMP_ECHOREPLY && icmp_reply->un.echo.id == getpid()) {
                struct timeval recv_time;
                gettimeofday(&recv_time, NULL);
                double rtt = (recv_time.tv_sec - send_time.tv_sec) * 1000.0 + (recv_time.tv_usec - send_time.tv_usec) / 1000.0;
                printf("%d bytes from %s: icmp_seq=%d, ttl=%d, rtt=%.3fms\n", icmplen, inet_ntoa(ip->ip_src), icmp_reply->un.echo.sequence, ip->ip_ttl, rtt);
                received_count++;  // 接收成功，计数加1
            }
        }

        // 添加统计信息输出部分，计算并输出发送、接收的ICMP消息个数及丢包率
        double loss_rate = (sent_count - received_count) * 1.0 / sent_count * 100;
        printf("\n--- %s ping statistics ---\n", ipaddr);
        printf("%d packets transmitted, %d packets received, %.2f%% packet loss\n",
               sent_count, received_count, loss_rate);
    }

    close(sockfd);

    // 只有在非指定次数发送模式（即用户按Ctrl+c终止程序时）才输出统计信息，避免重复输出
    if (count == -1) {
        double loss_rate = (sent_count - received_count) * 1.0 / sent_count * 100;
        printf("\n--- %s ping statistics ---\n", ipaddr);
        printf("%d packets transmitted, %d packets received, %.2f%% packet loss\n",
               sent_count, received_count, loss_rate);
    }
}

// 信号处理函数定义
void sigint_handler(int signum) {
    double loss_rate = (sent_count - received_count) * 1.0 / sent_count * 100;
    printf("\n--- %s ping statistics ---\n", "Interrupted");
    printf("%d packets transmitted, %d packets received, %.2f%% packet loss\n",
           sent_count, received_count, loss_rate);
    exit(0);
}

unsigned short checksum(unsigned short *buf, int len) {
    unsigned int sum = 0;
    while (len > 1) {
        sum += *buf++;
        len -= 2;
    }
    if (len == 1) {
        sum += (*buf & 0xFF00);
    }
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    return (unsigned short)(~sum);
}
