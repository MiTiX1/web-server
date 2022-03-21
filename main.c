#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#include "server.h"

int main() {
    int sockfd, acceptsockfd;
    
    if((sockfd=create_server()) < 0) {
        fprintf(stderr, "Failed to create the server\n");
        return -1;
    }

    for(;;) {
        if((acceptsockfd=accept(sockfd, NULL, NULL)) < 0) {
            perror("Error accept()");
            exit(-1);
        }

        if(!fork()) {
            close(sockfd);
            handle_client_request(acceptsockfd);
            close(acceptsockfd);
            exit(EXIT_SUCCESS);
        }
        close(acceptsockfd);
    }

    return 0;
}