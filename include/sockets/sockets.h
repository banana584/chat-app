#ifndef SOCKETS_UTILS_H
#define SOCKETS_UTILS_H

#if definied(__linux__) || defined(__unix__)
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
#elif definied(_WIN32) || defined(_WIN64)
    #define WIN32_LEAN_AND_MEAN
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <windows.h>
    #pragma comment(lib, "ws2_32.lib")
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

char socket_errbuff[1024];
#define SOCKET_ERRBUFF_SIZE 1024

#ifndef CLEAR_SOCKET_ERRBUFF
    #define CLEAR_SOCKET_ERRBUFF(errbuff) memset(errbuff, 0, SOCKET_ERRBUFF_SIZE)
#endif

#if definied(__linux__) || definied(__unix__)
    typedef struct Socket {
        int fd;
    } Socket;
#elif definied(_WIN32) || defined(__unix__)
    typedef struct Socket {
        SOCKET fd;
    } Socket;
#endif

int Socket_InitializeLib();

void Socket_DeInitializeLib();

Socket* Socket_Create();

int Socket_Connect(Socket* socket, struct sockaddr_in* addr);

#endif
