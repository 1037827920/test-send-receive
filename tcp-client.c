#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 12345

int main(void)
{
    int sockfd;
    struct sockaddr_in addr;
    char buf[64];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(1);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("Connecting to server on port %d\n", PORT);
    if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("connect");
        exit(1);
    }

    // printf("Connected to server\n");
    // // 使用send代替write
    // if (send(sockfd, "ping", 4, 0) < 0) {
    //     perror("send");
    //     close(sockfd);
    //     exit(1);
    // }

    // // 使用recv代替read
    // int bytes_received = recv(sockfd, buf, sizeof(buf) - 1, 0);
    // if (bytes_received < 0) {
    //     perror("recv");
    //     close(sockfd);
    //     exit(1);
    // }

    // buf[bytes_received] = '\0'; // 确保字符串正确终止
    // printf("recv: %s\n", buf);

    close(sockfd);
    return 0;
}