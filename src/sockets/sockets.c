#include "../../include/sockets/sockets.h"

int Socket_InitializeLib() {
    CLEAR_SOCKET_ERRBUFF(socket_errbuff);
    errno = 0;
    #if defined(_WIN32) || defined(_WIN64)
        WSADATA wsaData;

        int result = 0;
        if ((result = WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            sprintf(socket_errbuff, "%d", result);
            errno = result;
            return -1;
        }
    #endif
    return 0;
}

void Socket_DeInitializeLib() {
    CLEAR_SOCKET_ERRBUFF(socket_errbuff);
    errno = 0;
    #if defined(_WIN32) || defined(_WIN64)
        WSACleanup();
    #endif
}

Socket* Socket_Create() {
    CLEAR_SOCKET_ERRBUFF(socket_errbuff);
    errno = 0;
    Socket* socket = (Socket*)malloc(sizeof(Socket));
    if (!socket) {
        int result = errno;
        socket_errbuff = strerror(result);
        errno = result;
        return -1;
    }

    socket->fd = socket(AF_INET, SOCK_STREAM, 0);
    int result = 0;
    #if defined(_WIN32) || defined(_WIN64)
        if (socket->fd == INVALID_SOCKET) {
            result = WSAGetLastError();
            FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, result, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&socket_errbuff, 0, NULL);
            errno = result;
            return -1;
        }
    #elif defined(__linux__) || defined(__unix__)
        if (socket->fd < 0) {
            result = errno;
            socket_errbuff = strerror(result);
            errno = result;
            return -1;
        }
    #endif

    return socket;
}

int Socket_Connect(Socket* socket, struct sockaddr_in* addr) {
    int result = connect(socket->fd, (struct sockaddr*)&addr, sizeof(*addr));

    #if defined(_WIN32) || defined(_WIN64)
        if (result == SOCKET_ERROR) {
            result = WSAGetLastError();
            FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, result, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&socket_errbuff, 0, NULL);
            errno = result;
            return -1;
        }
    #elif defined(__linux__) || defined(__unix__)
        if (result < 0) {
            result = errno;
            socket_errbuff = strerror(result);
            errno = result;
            return -1;
        }
    #endif

    return 0;
}
