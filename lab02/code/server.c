#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define MAX_SIMULT_CONNECTIONS 1
#define BUF_SIZE 65536

#ifndef SRV_FILE
#define SRV_FILE "srv_data.bin"
#endif

#ifndef FILE_SIZE 
#define FILE_SIZE (1 << 30) /* 1 GB */
#endif

 #define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

static void srv_send(int sock, int file)
{
    ssize_t ret;
    ssize_t bytsToRead = FILE_SIZE;
    u_int8_t buf[BUF_SIZE];
   
    while (bytsToRead) {
        ret = read(file, &buf, min(sizeof(buf),bytsToRead));

        if (ret < 0) {
            perror("listen() error");
            exit(EXIT_FAILURE);
        }

        if (ret != min(sizeof(buf),bytsToRead)) {
            fprintf(stderr, "[%s] Error: sent %ld bytes (expected %ld)\n", __func__, ret,min(sizeof(buf),BUF_SIZE));
            exit(EXIT_FAILURE);
        }

        if (send(sock, &buf, min(sizeof(buf),bytsToRead), 0) != min(sizeof(buf),bytsToRead)) {
            perror("send() error");
            exit(EXIT_FAILURE);
        }
        bytsToRead -= ret;

    }

    printf("[%s] sent %d bytes\n", __func__, FILE_SIZE);
}

void srv_start(const unsigned short port_srv)
{
    int file;
    int sock_srv, sock_client;
    struct sockaddr_in addr_srv, addr_client;
    unsigned int addrlen_client;

    /* file init */
    file = open(SRV_FILE, O_RDONLY|O_DSYNC, 0);
    if (file < 0) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    /* socket init */
    if ((sock_srv = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        perror("Error creating server socket");
        exit(EXIT_FAILURE);
    }

    memset(&addr_srv, 0, sizeof(addr_srv));
    addr_srv.sin_family = AF_INET;
    addr_srv.sin_addr.s_addr = htonl(INADDR_ANY);
    addr_srv.sin_port = htons(port_srv);

    /* Allows this process to bind to a port which remains in TIME_WAIT. So if this
     * process is run again, the user doesn't have to wait for a timeout */
    if (setsockopt(sock_srv, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0) {
        perror("SO_REUSEADDR failed");
        exit(EXIT_FAILURE);
    }

    if (bind(sock_srv, (struct sockaddr *)&addr_srv, sizeof(addr_srv)) < 0) {
        perror("bind() error");
        exit(EXIT_FAILURE);
    }

    if (listen(sock_srv, MAX_SIMULT_CONNECTIONS) < 0) {
        perror("listen() error");
        exit(EXIT_FAILURE);
    }

    addrlen_client = sizeof(addr_client);

    /* wait for a client to connect */
    if ((sock_client = accept(sock_srv, (struct sockaddr *) &addr_client, &addrlen_client)) < 0) { 
        perror("accept() error");
        exit(EXIT_FAILURE);
    }
    printf("[%s] %s connected\n", __func__, inet_ntoa(addr_client.sin_addr));

    /* client connected */
    srv_send(sock_client, file);

    close(sock_srv);
    close(sock_client);
    close(file);
}
