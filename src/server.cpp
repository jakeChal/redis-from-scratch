#include <arpa/inet.h>
#include <errno.h>
#include <netinet/ip.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

static void msg(const char *msg) { fprintf(stderr, "%s\n", msg); }

static void die(const char *msg) {
    int err = errno;
    fprintf(stderr, "[%d] %s\n", err, msg);
    abort();
}

static void do_something(int connfd) {
    char rbuf[64] = {};
    // read() receives data from a TCP connection
    ssize_t n = read(connfd, rbuf, sizeof(rbuf) - 1);
    if (n < 0) {
        msg("read() error");
        return;
    }
    printf("client says: %s\n", rbuf);

    char wbuf[] = "world";
    write(connfd, wbuf, strlen(wbuf));
}

int main() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        die("socket()");
    }

    // this is needed for most server applications:
    // purpose is to restart a closed/killed process on the same address.
    int val = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

    // bind: associates an address to a socket fd
    struct sockaddr_in addr = {};

    // IPv4
    addr.sin_family = AF_INET;

    // Internet headers standardized on using the big endian format.
    // convert a 16-bit number from a network representation into the local
    // processor's format [1234BE = 0x04D2BE = 0xD204LE = 53764LE]
    addr.sin_port = ntohs(1234);
    addr.sin_addr.s_addr = ntohl(0);  // wildcard address 0.0.0.0
    int rv = bind(fd, (const sockaddr *)&addr, sizeof(addr));
    if (rv) {
        die("bind()");
    }

    // listen: enables us to accept connections to that address
    rv = listen(fd, SOMAXCONN);
    if (rv) {
        die("listen()");
    }

    while (true) {
        // accept: takes a listening fd and:
        //         when a client makes a connection to the listening address,
        //         the accept() returns an fd that represents the connection
        //         socket
        struct sockaddr_in client_addr = {};
        socklen_t socklen = sizeof(client_addr);
        int connfd = accept(fd, (struct sockaddr *)&client_addr, &socklen);
        if (connfd < 0) {
            continue;  // error
        }

        do_something(connfd);
        // close: destroys the resource referred by the fd and recycles the fd
        // number.
        close(connfd);
    }

    return 0;
}