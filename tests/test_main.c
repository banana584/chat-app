#include <stdio.h>
#include <assert.h>
#if defined(__linux__) || defined(__unix__)
  #include <unistd.h>
#endif
#include <sockets/sockets.h>

int main(int argc, char* argv[]) {
  char empty[1024] = {0};
  memset(empty, 0, 1024 * sizeof(char));
  
  assert(Socket_InitializeLib() == 0);

  Socket* socket1 = Socket_Create();
  assert(socket1 != NULL);

  int optval = 1;
  #if defined(_WIN32) || defined(_WIN64)
    setsockopt(socket1->fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&optval, sizeof(optval));
  #elif defined(__linux__) || defined(__unix__)
    setsockopt(socket1->fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
  #endif
  #if defined(__linux__) || defined(__unix__)
    assert(socket1->fd != -1);
  #elif defined(_WIN32) || defined(_WIN64)
    assert(socket1->fd != INVALID_SOCKET);
  #endif
  assert(errno == 0);
  assert(strcmp(socket_errbuff, "") == 0);

  struct sockaddr_in addr1 = {0};
  addr1.sin_family = AF_INET;
  addr1.sin_port = htons(8080);
  addr1.sin_addr.s_addr = inet_addr("127.0.0.1");
  socklen_t addr1_len = sizeof(addr1);
  
  Socket_Bind(socket1, &addr1, addr1_len);
  assert(errno == 0);
  assert(memcmp(socket_errbuff, empty, 1024 * sizeof(char)) == 0);

  Socket_Listen(socket1, 1);
  assert(errno == 0);
  assert(memcmp(socket_errbuff, empty, 1024 * sizeof(char)) == 0);

  Socket* socket2 = Socket_Create();
  assert(socket2 != NULL);
  #if defined(__linux__) || defined(__unix__)
    assert(socket2->fd != -1);
  #elif defined(_WIN32) || defined(_WIN64)
    assert(socket2->fd != INVALID_SOCKET);
  #endif
  assert(errno == 0);
  assert(memcmp(socket_errbuff, empty, 1024 * sizeof(char)) == 0);
      
  Socket_Destroy(socket1);
  assert(errno == 0);
  assert(memcmp(socket_errbuff, empty, 1024 * sizeof(char)) == 0);

  Socket_Destroy(socket2);
  assert(errno == 0);
  assert(memcmp(socket_errbuff, empty, 1024 * sizeof(char)) == 0);

  Socket_DeInitializeLib();

  printf("All tests passed!\n");

  return 0;
}
