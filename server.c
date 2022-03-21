#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>

#include "config.h"
#include "server.h"
#include "http.h"

int handle_file(int sockfd, int status, char *filename) {
    int fd, size_read;
    char file[BUFFER_SIZE];

    fd = open(filename, O_RDWR);
    if(fd < 0) {
        if(errno == ENOENT) {
            handle_file(sockfd, 404, PAGE_404);
        } else if(errno == EACCES) {
            handle_file(sockfd, 500, PAGE_500);
        }
    } else {
        send_header(sockfd, status);
        while((size_read = read(fd, file, BUFFER_SIZE)) > 0) {
            write(sockfd, file, size_read);
        }
    }

    close(fd);
    return 0;
}

void handle_client_request(int sockfd) {
    char req[BUFFER_SIZE], buf[BUFFER_SIZE];
    read(sockfd, req, BUFFER_SIZE);

    if(parse_request(req, BUFFER_SIZE, buf, BUFFER_SIZE) < 0) {
        handle_file(sockfd, 400, PAGE_400);
    }
    printf("Request: %s\n", buf);
    handle_file(sockfd, 200, buf);
}

int create_server() {
    struct addrinfo hints, *res = NULL;
    int sockfd, err, tr=1;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET6;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    
    if((err=getaddrinfo(NULL, LISTEN_PORT, &hints, &res)) != 0) {
        fprintf(stderr, "Error getaddrinfo: %s\n", gai_strerror(err));
        freeaddrinfo(res);
        return -1;
    }

    if((sockfd=socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0) {
        perror("Error socket()");
        freeaddrinfo(res);
        return -1;
    }

    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &tr, sizeof(int)) < 0) {
        perror("Error setsockopt()");
        freeaddrinfo(res);
        return -1;
    }

    if(bind(sockfd, res->ai_addr, res->ai_addrlen) < 0) {
        perror("Error bind()");
        freeaddrinfo(res);
        close(sockfd);
        return -1;
    }

    if(listen(sockfd, LISTEN_BACKLOG) < 0) {
        perror("Error listen()");
        freeaddrinfo(res);
        close(sockfd);
        return -1;
    }

    return sockfd;
}