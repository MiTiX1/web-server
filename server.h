int create_server();
void handle_client_request(int sockfd);
int parse_request(char* req, int req_size, char* string, int string_size);
int handle_file(int sockfd, int status, char *filename);