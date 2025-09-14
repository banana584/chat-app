#include "../sockets/sockets.h"

typedef struct Server {
  Socket* server_sock;
  struct sockaddr_in addr;
  socklen_t addr_len;
  Socket** client_socks;
} Server;

Server* Server_Create(int port);

void Server_Destroy(Server* server);
