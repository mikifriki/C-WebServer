#ifndef HNDLSOCKET_HEADER
#define HNDLSOCKET_HEADER
#define BUFFSIZE 1024
#define PORT 8080
int setupSocket(int *socketfd, struct sockaddr_in sockaddr_host, int *sockaddrlen);
int acceptRequest(int *originalSocketfd, int *newSocketfd, struct sockaddr_in sockaddr_host, int sockaddrlen);
int readClientData(int *socketfd, int *clientAddress, struct sockaddr_in *sockaddr_host, int sockaddrlen);
int readSocketData(int *socketfd, int *readByes, char buffer[]);
int returnResponseData(int newSocketfd, char *response);
#endif