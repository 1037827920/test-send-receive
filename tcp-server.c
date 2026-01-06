#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 12345

int main(void)
{
    int listen_fd, conn_fd;
    struct sockaddr_in addr;
    char buf[64];

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0) {
        perror("socket");
        exit(1);
    }

    int opt = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    if (bind(listen_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        exit(1);
    }

    if (listen(listen_fd, 1) < 0) {
        perror("listen");
        exit(1);
    }

    printf("Server listening on port %d\n", PORT);

    /* ===== 这里 accept() 会触发 sk_clone() ===== */
    conn_fd = accept(listen_fd, NULL, NULL);
    if (conn_fd < 0) {
        perror("accept");
        exit(1);
    }

    printf("Client connected\n");

    read(conn_fd, buf, sizeof(buf));
    write(conn_fd, "pong", 4);

    close(conn_fd);
    close(listen_fd);
    return 0;
}
