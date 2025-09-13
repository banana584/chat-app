#include "../../include/sockets/sockets.h"

char socket_errbuff[1024] = {0};

int Socket_InitializeLib() {
    CLEAR_SOCKET_ERRBUFF(socket_errbuff);
    errno = 0;
    #if defined(_WIN32) || defined(_WIN64)
        WSADATA wsaData;

        int result = 0;
        if ((result = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0) {
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
    Socket* s = (Socket*)malloc(sizeof(Socket));
    if (!s) {
        int result = errno;
        #if defined(_WIN32) || defined(_WIN64)
            strerror_s(socket_errbuff, sizeof(socket_errbuff), result);
        #elif defined(__linux__) || defined(__unix__)
            strcpy(socket_errbuff, strerror(result));
        #endif
        errno = result;
        return NULL;
    }

    s->fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    int result = 0;
    
    #if defined(_WIN32) || defined(_WIN64)
        if (s->fd == INVALID_SOCKET) {
            result = WSAGetLastError();
        }
    #elif defined(__linux__) || defined(__unix__)
        if (s->fd < 0) {
            result = errno;
        }
    #endif
    if (result != 0) {
        FORMAT_SOCKET_ERROR_NUM(result, &socket_errbuff);
        errno = result;
        return NULL;
    }

    return s;
}

int Socket_Bind(Socket* socket, struct sockaddr_in* addr, socklen_t addr_len) {
    CLEAR_SOCKET_ERRBUFF(socket_errbuff);
    errno = 0;

    int errcode = 0;
    int result = bind(socket->fd, (struct sockaddr*)addr, addr_len);
    if (result < 0) {
        #if defined(_WIN32) || defined(_WIN64)
            errcode = WSAGetLastError();
        #elif defined(__linux__) || defined(__unix__)
            errcode = errno;
        #endif
        FORMAT_SOCKET_ERROR_NUM(errcode, &socket_errbuff);
        errno = errcode;
        return -1;
    }

    return 0;
}

int Socket_Listen(Socket* socket, int backlog) {
    CLEAR_SOCKET_ERRBUFF(socket_errbuff);
    errno = 0;

    int errcode = 0;
    int result = listen(socket->fd, backlog);
    if (result < 0) {
        #if defined(_WIN32) || defined(_WIN64)
            errcode = WSAGetLastError();
        #elif defined(__linux__) || defined(__unix__)
            errcode = errno;
        #endif
        FORMAT_SOCKET_ERROR_NUM(errcode, &socket_errbuff);
        errno = errcode;
        return -1;
    }

    return 0;
}

int Socket_Connect(Socket* socket, struct sockaddr_in* addr) {
    CLEAR_SOCKET_ERRBUFF(socket_errbuff);
    errno = 0;
    int result = connect(socket->fd, (struct sockaddr*)&addr, sizeof(*addr));
    int errcode = 0;

    #if defined(_WIN32) || defined(_WIN64)
        if (result == SOCKET_ERROR) {
            errcode = WSAGetLastError();
        }
    #elif defined(__linux__) || defined(__unix__)
        if (result < 0) {
            errcode = errno;
        }
    #endif
    if (errcode != 0) {
        FORMAT_SOCKET_ERROR_NUM(errcode, &socket_errbuff);
        errno = errcode;
        return -1;
    }
    
    return 0;
}

int Socket_Accept(Socket* socket) {
    CLEAR_SOCKET_ERRBUFF(socket_errbuff);
    errno = 0;

    struct sockaddr_in addr = {0};

    socklen_t addr_len = sizeof(struct sockaddr_in);

    int errcode = 0;
    int result = accept(socket->fd, (struct sockaddr*)&addr, &addr_len);
    #if defined(_WIN32) || defined(_WIN64)
        if (result == SOCKET_ERROR) {
            errcode = WSAGetLastError();
        }
    #elif defined(__linux__) || defined(__unix__)
        if (result < 0) {
            errcode = errno;
        }
    #endif
    if (errcode != 0) {
        FORMAT_SOCKET_ERROR_NUM(errcode, &socket_errbuff);
        errno = errcode;
        return -1;
    }

    return result;
}

int Socket_Send(Socket* socket, Message* message) {
    CLEAR_SOCKET_ERRBUFF(socket_errbuff);
    errno = 0;

    int errcode = 0;
    int result = send(socket->fd, message->string, message->num_chars, 0);
    #if defined(_WIN32) || defined(_WIN64)
        if (result == SOCKET_ERROR) {
            errcode = WSAGetLastError();
        }
    #elif defined(__linux__) || defined(__unix__)
        if (result < 0) {
            errcode = errno;
        }
    #endif
    if (errcode != 0) {
        FORMAT_SOCKET_ERROR_NUM(errcode, &socket_errbuff);
        errno = errcode;
        return -1;
    }

    return result;
}

Message* Socket_Recieve(Socket* socket) {
    CLEAR_SOCKET_ERRBUFF(socket_errbuff);
    errno = 0;

    Message* message = (Message*)malloc(sizeof(Message));
    if (!message) {
        #if defined(_WIN32) || defined(_WIN64)
            strerror_s(socket_errbuff, sizeof(socket_errbuff), errno);
        #elif defined(__linux__) || defined(__unix__)
            strcpy(socket_errbuff, strerror(errno));
        #endif
        return NULL;
    }
    memset(message->string, 0, 1024);
    message->num_chars = 0;
    message->flags = 0;

    int errcode = 0;
    int result = recv(socket->fd, message->string, 1024, 0);
    if (result > 0) {
        return message;
    } else if (result == 0) {
        message->flags = SOCKET_MESSAGE_CONN_CLOSED;
        return message;
    } else {
        #if defined(_WIN32) || defined(_WIN64)
            errcode = WSAGetLastError();
        #elif defined(__linux__) || defined(__unix__)
            errcode = errno;
        #endif
        free(message);
        FORMAT_SOCKET_ERROR_NUM(errcode, &socket_errbuff);
        errno = errcode;
        return NULL;
    }
}
