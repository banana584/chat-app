#ifndef SOCKETS_UTILS_H
#define SOCKETS_UTILS_H

#include <stdio.h>
#if defined(__linux__) || defined(__unix__)
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    printf("running on linux/unix\n");
#elif defined(_WIN32) || defined(_WIN64)
    #define WIN32_LEAN_AND_MEAN
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <windows.h>
    #pragma comment(lib, "ws2_32.lib")
    printf("running on windows\n");
#endif
#include <stdlib.h>
#include <string.h>
#include <errno.h>

char socket_errbuff[1024];
#define SOCKET_ERRBUFF_SIZE 1024

#ifndef CLEAR_SOCKET_ERRBUFF
    #define CLEAR_SOCKET_ERRBUFF(errbuff) memset(errbuff, 0, SOCKET_ERRBUFF_SIZE)
#endif

#if defined(__linux__) || defined(__unix__)
    typedef struct Socket {
        int fd;
    } Socket;
    #ifndef FORMAT_SOCKET_ERROR_NUM
        #define FORMAT_SOCKET_ERROR_NUM(error_num, buffer) *buffer = strerror(error_num)
    #endif
#elif defined(_WIN32) || defined(_WIN64)
    typedef struct Socket {
        SOCKET fd;
    } Socket;
    #ifndef FORMAT_SOCKET_ERROR_NUM
        #define FORMAT_SOCKET_ERROR_NUM(error_num, buffer) FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, error_num, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)buffer, 0, NULL)
    #endif
#endif

typedef struct Message {
    char string[1024];
    size_t num_chars;
    int flags;
} Message;

int Socket_InitializeLib();

void Socket_DeInitializeLib();

Socket* Socket_Create();

int Socket_Bind(Socket* socket, struct sockaddr_in* addr, socklen_t addr_len);

int Socket_Listen(Socket* socket, int backlog);

int Socket_Connect(Socket* socket, struct sockaddr_in* addr);

int Socket_Accept(Socket* socket);

ssize_t Socket_Send(Socket* socket, Message* message);

Message* Socket_Recieve(Socket* socket);

#endif
