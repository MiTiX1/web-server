#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "http.h"

static char* get_status_name(int status) {
    switch(status) {
		case 200:
			return "OK";
		case 400:
			return "BAD REQUEST";
		case 404:
			return "NOT FOUND";
		case 500:
			return "INTERNAL SERVER ERROR";
	}
	return "ERROR";
}

int parse_request(char* requestFromClient, int requestSize, char* string, int stringSize) {
	char *charPtr[4], *end;

	requestFromClient[requestSize-1]='\0';

	if((end=strstr(requestFromClient,"\r\n\r\n"))==NULL) return -1;
	*(end+4)='\0';		
	charPtr[0]=requestFromClient;charPtr[1]=strchr(requestFromClient,' ');	

	if(charPtr[1]==NULL) return -1;
	charPtr[2]=strchr(charPtr[1]+1,' ');	
	if(charPtr[2]==NULL) return -1;
	charPtr[3]=strchr(charPtr[2]+1,'\r');	
	if(charPtr[3]==NULL) return -1;

	*charPtr[1]='\0';
	*charPtr[2]='\0';
	*charPtr[3]='\0';

	if(strcmp(charPtr[0],"GET")!=0) return -1;
	if(strcmp(charPtr[2]+1,"HTTP/1.1")!=0) return -1;
	strncpy(string,charPtr[1]+2,stringSize); 

	if(string[stringSize-1]!='\0'){
		fprintf(stderr,"Error parseRequest(): buffer too small");
		exit(3);
	}
	if(*(charPtr[1]+2) == '\0') strcpy(string,"index.html");

	return 0;
}

void send_header(int sockfd, int status) {
    char buf[BUFFER_SIZE];
    sprintf(buf, "HTTP/1.1 %d %s\r\nContent-Type: text/html\r\n\r\n", status, get_status_name(status));
    write(sockfd, buf, strlen(buf));
}