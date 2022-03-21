#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1000
#endif

int parse_request(char* requestFromClient, int requestSize, char* string, int stringSize);
void send_header(int sockfd, int status);