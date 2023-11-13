#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "systeminfo/systemstats.h"
#include "socket/handlesocket.h"
#define SMALLSTRINGBUFFER 32
#define MAXOUTPUTLENGTH 400
#define NOENDPOINT "no endpoint"
int main()
{
    struct sockaddr_in sockaddr_host;
    int sockaddrlen = sizeof(sockaddr_host);
    int tcp_socket_fd, newSocketfd, clientAddress, bytesRead, cpuTemp, mem, storageSize;
    char *headerString = "HTTP/1.0 200 OK;\r\nServer: webserver-c;\r\nContent-type: text/plain; charset=UTF-8;\n\n";
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

        // With this we can read client address data. Aka adress and port;
        // This can be optional but is set in a way that it is not.
        if (readClientData(&newSocketfd, &clientAddress, &sockaddr_host, sockaddrlen) == -1)
        {
            continue;
        }
        char buffer[BUFFSIZE];
        // Read the socket of the file descriptor into the buffer.
        // Returned value is the count of the bytes read into the file.
        if (readSocketData(&newSocketfd, &bytesRead, buffer) == -1)
        {
            continue;
        }
        char method[BUFFSIZE];
        char uri[BUFFSIZE];
        char version[BUFFSIZE];
        // Get the method, uri and version of the request from the buffer.
        sscanf(buffer, "%s %s %s", method, uri, version);
        // socket address ip, port, request method, endpoint and HTTP version.
        printf("[%s:%u] %s %s %s\n", inet_ntoa(sockaddr_host.sin_addr), ntohs(sockaddr_host.sin_port), method, uri, version);

        if (strcmp(uri, "/ip") == 0)
        {
            char machineIp[SMALLSTRINGBUFFER];
            getIp(machineIp, SMALLSTRINGBUFFER);
            returnResponseData(newSocketfd, headerString, machineIp, strlen(machineIp), strlen(headerString));
            continue;
        }
        if (strcmp(uri, "/status") == 0)
        {
            char machineName[SMALLSTRINGBUFFER];
            getSystemName(machineName, SMALLSTRINGBUFFER);
            returnResponseData(newSocketfd, headerString, machineName, strlen(machineName), strlen(headerString));
            continue;
        }
        if (strcmp(uri, "/cpu") == 0)
        {
            cpuTemperature(&cpuTemp);
            char cpuTempString[SMALLSTRINGBUFFER];
            snprintf(cpuTempString, SMALLSTRINGBUFFER, "%i", cpuTemp);
            returnResponseData(newSocketfd, headerString, cpuTempString, strlen(cpuTempString), strlen(headerString));
            continue;
        }
        if (strcmp(uri, "/memTotal") == 0)
        {
            getSystemMemoryInformation(&mem, 0);
            char memString[SMALLSTRINGBUFFER];
            snprintf(memString, SMALLSTRINGBUFFER, "%i", mem);
            returnResponseData(newSocketfd, headerString, memString, strlen(memString), strlen(headerString));
            continue;
        }
        if (strcmp(uri, "/memAvailable") == 0)
        {
            getSystemMemoryInformation(&mem, 1);
            char memString[SMALLSTRINGBUFFER];
            snprintf(memString, SMALLSTRINGBUFFER, "%i", mem);
            returnResponseData(newSocketfd, headerString, memString, strlen(memString), strlen(headerString));
            continue;
        }
        if (strcmp(uri, "/totalStorage") == 0)
        {
            systemStorageSpace(&storageSize, "total");
            char storageString[SMALLSTRINGBUFFER];
            snprintf(storageString, SMALLSTRINGBUFFER, "%i", storageSize);
            returnResponseData(newSocketfd, headerString, storageString, strlen(storageString), strlen(headerString));
            continue;
        }
        if (strcmp(uri, "/availableStorage") == 0)
        {
            systemStorageSpace(&storageSize, "available");
            char storageString[SMALLSTRINGBUFFER];
            snprintf(storageString, SMALLSTRINGBUFFER, "%i", storageSize);
            returnResponseData(newSocketfd, headerString, storageString, strlen(storageString), strlen(headerString));
            continue;
        }
        if (strcmp(uri, "/kernelInfo") == 0)
        {
            char kernelInfo[SMALLSTRINGBUFFER];
            getSystemKernelInfo(kernelInfo, SMALLSTRINGBUFFER);
            returnResponseData(newSocketfd, headerString, kernelInfo, strlen(kernelInfo), strlen(headerString));
            continue;
        }
        if (strstr(uri, "/getProccessData") != NULL && strstr(uri, "?process=") != NULL)
        {
            char topLine[MAXOUTPUTLENGTH];
            char *const sep_at = strchr(uri, '=');
            if (sep_at == NULL)
            {
                printf("first part: '%s'\nsecond part: '%s'\n", uri, sep_at);
                goto noendpoint;
            }
            getProcessesData(topLine, sep_at + 1, MAXOUTPUTLENGTH);
            returnResponseData(newSocketfd, headerString, topLine, strlen(topLine), strlen(headerString));
            continue;
        }
        if (strstr(uri, "/getPm2Process") != NULL && strstr(uri, "?index=") != NULL)
        {
            char pm2Process[MAXOUTPUTLENGTH];
            char *const uriVariableValue = strchr(uri, '=');
            if (uriVariableValue == NULL)
            {
                printf("first part: '%s'\nsecond part: '%s'\n", uri, uriVariableValue);
                goto noendpoint;
            }
            getPm2Data(pm2Process, uriVariableValue + 1, MAXOUTPUTLENGTH);
            returnResponseData(newSocketfd, headerString, pm2Process, strlen(pm2Process), strlen(headerString));
            continue;
        }
        goto noendpoint;

    // Default end if no results are found
    noendpoint:
        returnResponseData(newSocketfd, headerString, NOENDPOINT, strlen(NOENDPOINT), strlen(headerString));
        continue;
    }

    close(tcp_socket_fd);
    return 0;
}
