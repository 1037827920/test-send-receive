#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>

#define BUF_SIZE 1500

int main(void)
{
    int sockfd;
    unsigned char buf[BUF_SIZE];
    struct sockaddr_in from;
    socklen_t fromlen = sizeof(from);
    struct in_addr target_addr;

    const char *ip = "183.2.172.17";

    /* 直接转换 IP 地址 */
    target_addr.s_addr = inet_addr(ip);

    /* 创建 raw socket 接收 ICMP */
    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0) {
        perror("socket");
        return 1;
    }

    printf("Waiting ICMP replies from %s...\n", ip);

    while (1) {
        ssize_t len = recvfrom(sockfd, buf, sizeof(buf), 0,
                               (struct sockaddr *)&from, &fromlen);
        if (len < 0) {
            perror("recvfrom");
            close(sockfd);
            return 1;
        }

        /* 解析 IP 头 */
        struct iphdr *ip_hdr = (struct iphdr *)buf;
        int iphdr_len = ip_hdr->ihl * 4;

        /* ICMP 头 */
        struct icmphdr *icmp =
            (struct icmphdr *)(buf + iphdr_len);

        /* 只关心 Echo Reply */
        if (icmp->type != ICMP_ECHOREPLY)
            continue;

        /* 只关心来自 183.2.172.17 的包 */
        if (from.sin_addr.s_addr != target_addr.s_addr)
            continue;

        printf("Received ICMP Echo Reply from %s\n",
               inet_ntoa(from.sin_addr));
        printf("  id=%u seq=%u\n",
               ntohs(icmp->un.echo.id),
               ntohs(icmp->un.echo.sequence));

        break;
    }

    close(sockfd);
    return 0;
}