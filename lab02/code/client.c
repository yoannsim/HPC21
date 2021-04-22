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

#ifndef CLIENT_FILE
#define CLIENT_FILE "client_data.bin"
#endif

#define BUF_CHUCK (uint64_t)65536
#define BUF_NB_CHUCK (uint64_t)16
#define BUF_SIZE (BUF_CHUCK * BUF_NB_CHUCK)


#ifndef FILE_SIZE 
#define FILE_SIZE (1 << 30) /* 1 GB */
#endif

#define min(a,b) \
({ __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a < _b ? _a : _b; })

static void client_recv(const int sock, const int file)
{
    ssize_t retR;
    ssize_t retW;
    uint8_t buf[BUF_SIZE];
    ssize_t bytsTorecv = FILE_SIZE;
    ssize_t chunkToWrite = 0;
    while (bytsTorecv) {
        /* receive from server */
        if ((retR = recv(sock, &buf[chunkToWrite*BUF_CHUCK], min(BUF_CHUCK,bytsTorecv), 0)) < 0) {
            perror("recv() error");
            exit(EXIT_FAILURE);
        }

        if (retR < 0) {
            perror("listen() error");
            exit(EXIT_FAILURE);
        }

        if (retR != min(BUF_CHUCK,bytsTorecv)) {
            fprintf(stderr, "[%s] Error: sent %ld bytes (expected %ld)\n", __func__, retR,min(BUF_CHUCK,bytsTorecv));
            exit(EXIT_FAILURE);
        }

        bytsTorecv -= retR;

        chunkToWrite++;

        /* write to file */
        if(chunkToWrite == BUF_NB_CHUCK || bytsTorecv == 0){
            retW = write(file, &buf, ((chunkToWrite-1) * BUF_CHUCK)+ retR);

            if (retW < 0) {
                perror("write() error");
                exit(EXIT_FAILURE);
            }

            if (retW != ((chunkToWrite-1) * BUF_CHUCK)+ retR) {
                fprintf(stderr, "[%s] Error: %ld bytes written (expected %ld)", __func__, retW,((chunkToWrite-1) * BUF_CHUCK)+ retR);
                exit(EXIT_FAILURE);
            }

            chunkToWrite = 0;

        }
        

       
    }

    printf("[%s] written %d bytes\n", __func__, FILE_SIZE);
}

void client_start(const char *ipv4_srv, unsigned short port_srv)
{
    int file;
    int sock;
    struct sockaddr_in addr_srv;

    /* file init */
    file = open(CLIENT_FILE, O_RDWR|O_CREAT|O_DSYNC, 0666);
    if (file < 0) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    /* socket init */
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        perror("Error creating client socket");
        exit(EXIT_FAILURE);
    }

    memset(&addr_srv, 0, sizeof(addr_srv));
    addr_srv.sin_family = AF_INET;
    addr_srv.sin_addr.s_addr = inet_addr(ipv4_srv);
    addr_srv.sin_port = htons(port_srv);

    if (connect(sock, (struct sockaddr *) &addr_srv, sizeof(addr_srv)) < 0) {
        perror("connect() error");
        exit(EXIT_FAILURE);
    }

    /* connected */
    client_recv(sock, file);

    close(sock);
    close(file);
}
