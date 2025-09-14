#include "../sockets/sockets.h"

typedef struct Node {
  struct Node* next;
  int id;
  Socket* data;
} Node;

typedef struct Server {
  Socket* server_sock;
  struct sockaddr_in addr;
  socklen_t addr_len;
  Node* client_socks_head;
  size_t num_clients;
} Server;

Server* Server_Create(int port);

void Server_Destroy(Server* server);

void Server_Accept(Server* server);

void Server_Disconnect(Server* server, int idx);

int Server_Send(Server* server, int idx, Message* message);

Message* Server_Recv(Server* server, int idx);

int Server_Sendall(Server* server, Message* message);
