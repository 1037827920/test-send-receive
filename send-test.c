#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/ip_icmp.h>

/* 计算 ICMP 校验和 */
static unsigned short checksum(void *buf, int len)
{
    unsigned int sum = 0;
    unsigned short *ptr = buf;

    while (len > 1) {
        sum += *ptr++;
        len -= 2;
    }

    if (len == 1)
        sum += *(unsigned char *)ptr;

    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);

    return (unsigned short)(~sum);
}

int main(void)
{
    int sockfd;
    struct sockaddr_in addr;
    struct icmphdr icmp;
    const char *ip = "183.2.172.17"; // www.baidu.com

    /* 创建 raw socket */
    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0) {
        perror("socket");
        return 1;
    }

    /* 直接使用 IP 地址 */
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);

    /* 构造 ICMP Echo Request */
    memset(&icmp, 0, sizeof(icmp));
    icmp.type = ICMP_ECHO;
    icmp.code = 0;
    icmp.un.echo.id = htons(getpid() & 0xffff);
    icmp.un.echo.sequence = htons(1);
    icmp.checksum = checksum(&icmp, sizeof(icmp));

    /* 发送 ICMP 包 */
    if (sendto(sockfd, &icmp, sizeof(icmp), 0,
               (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("sendto");
        return 1;
    }

    printf("ICMP packet sent to %s (%s)\n",
            "www.baidu.com", inet_ntoa(addr.sin_addr));

    close(sockfd);
    return 0;
}
