#include "../sockets/sockets.h"

typedef struct Server {
  Socket* server_sock;
  struct sockaddr_in addr;
  socklen_t addr_len;
  Socket** client_socks;
  size_t num_clients;
  size_t max_clients;
} Server;

Server* Server_Create(int port);

void Server_Destroy(Server* server);

void Server_Accept(Server* server);
