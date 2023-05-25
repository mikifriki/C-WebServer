
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "systemstats.c"
#include "handlesocket.h"

int main()
{
    struct sockaddr_in sockaddr_host;
    FILE *fp;
    double cpuTemp;
    int sockaddrlen = sizeof(sockaddr_host);
    int tcp_socket_fd;
    int newSocketfd;
    int clientAddress;
    int bytesRead;
    char buffer[BUFFSIZE];
    // TCP Layer aka TRANSPORT LAYER. This is the lowest layer I will implement atm
    // 1. Create the socket
    //  Domain: IPv4, Type: STREAM SOCKET as required by TCP, Protocol: 0. as the IP header for TCP has only one protocol then 0 is given.

    // Create, bind and listen to the newly created socket.
    if (setupSocket(&tcp_socket_fd, sockaddr_host, &sockaddrlen) == 1)
    {
        return 1;
    }

    // Now we go onto accept the incoming connections

    for (;;)
    {
        if (acceptRequest(&tcp_socket_fd, &newSocketfd, sockaddr_host, sockaddrlen) == -1)
        {
            continue;
        }
        printf("connection accepted\n");

        // With this we can read client address data. Aka adress and port;
        // This can be optional but is set in a way that it is not.
        if (readClientData(&newSocketfd, &clientAddress, &sockaddr_host, sockaddrlen) == -1)
        {
            continue;
        }

        // Read the socket of the file descriptor into the buffer.
        // Returned value is the count of the bytes read into the file.
        if (readSocketData(&newSocketfd, &bytesRead, buffer) == -1)
        {
            continue;
        }

        // scans the request headers and adds them to the print.
        char method[BUFFSIZE]; 
        char uri[BUFFSIZE];
        char version[BUFFSIZE];
        sscanf(buffer, "%s %s %s", method, uri, version);
        // socket address ip, port, request method, endpoint and HTTP version.
        printf("[%s:%u] %s %s %s\n", inet_ntoa(sockaddr_host.sin_addr), ntohs(sockaddr_host.sin_port), method, uri, version);

        // Check for endpoint here
        // Create data
        char resp[] = "HTTP/1.0 200 OK\r\n"
                      "Server: webserver-c\r\n"
                      "Content-type: text/html\r\n\r\n";
        cpuTemperature(fp, &cpuTemp);

        int converted_number = htonl(cpuTemp);
        char bb[32];
        sprintf(bb, "%lf", cpuTemp);
        strcat(resp, bb);
        // Data creation end
        int socketWrite = write(newSocketfd, resp, strlen(resp));

        if (socketWrite < 0)
        {
            perror("Webserver socket write error");
            continue;
        }
        /* code */

        close(newSocketfd);
    }

    close(tcp_socket_fd);
    return 0;
}
