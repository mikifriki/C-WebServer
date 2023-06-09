
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <arpa/inet.h>
#include "handlesocket.h"
int acceptRequest(int *originalSocketfd, int *newSocketfd, struct sockaddr_in sockaddr_host, int sockaddrlen)
{
    // Sets the file descriptor of the new socket if the accepting is successful.
    *newSocketfd = accept(*originalSocketfd, (struct sockaddr *)&sockaddr_host, (socklen_t *)&sockaddrlen);
    if (*newSocketfd < 0)
    {
        perror("Webserver socket accept error:");
        return -1;
    }
}

int readClientData(int *socketfd, int *clientAddress, struct sockaddr_in *sockaddr_host, int sockaddrlen)
{
    *clientAddress = getsockname(*socketfd, (struct sockaddr *)sockaddr_host, (socklen_t *)&sockaddrlen);
    if (clientAddress < 0)
    {
        perror("ClientAddress socket read error");
        return -1;
    }
}

int readSocketData(int *socketfd, int *readByes, char buffer[])
{
    // Read the socket of the file descriptor into the buffer.
    // Returned value is the count of the bytes read into the file.
    *readByes = read(*socketfd, buffer, BUFFSIZE);
    if (readByes < 0)
    {
        perror("Webserver socket read error");
        return -1;
    }
}

int setupSocket(int *socketfd, struct sockaddr_in sockaddr_host, int *sockaddrlen)
{
    *socketfd = socket(AF_INET, SOCK_STREAM, 0);
    int option = 1;
    // Allows the socker to be reused right after closing. Option has to be non zero
    setsockopt(*socketfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    // Now we basically bind the socket to our port and ip.
    sockaddr_host.sin_family = AF_INET;
    // Port and ADDR have to be in a agreed upon network byte order
    sockaddr_host.sin_port = htons(PORT);
    // We use INADDR_ANY as we dont care which ip gets assigned at the moment. If we do care then this has to be replaced.
    sockaddr_host.sin_addr.s_addr = htonl(INADDR_ANY);

    // perror has to be called right after the failure. Otherwise the errno value could be altered.
    if (*socketfd == -1)
    {
        perror("Webserver socket creation error");
        return 1;
    }

    printf("socket created successfully\n");

    // Bind socket. The cast is. Whatever addr is pointing to. Act like  sockaddr.
    // In my case it it will treat sockaddr_in as sockaddr
    // if it is successful it will bind the socket. If not then it will exit and print an error.
    if (bind(*socketfd, (struct sockaddr *)&sockaddr_host, *sockaddrlen))
    {
        perror("Webserver socket bind error: ");
        return 1;
    }
    printf("socket bound successfully\n");

    // Now we listen to the created and bound socket
    // Create a passive scoket
    if (listen(*socketfd, SOMAXCONN) != 0)
    {
        perror("Webserver socket listen error");
        return 1;
    }

    printf("Listening to socket\n");
}