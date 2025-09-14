#include "../../include/server/server.h"

Server* Server_Create(int port) {
  Server* server = (Server*)malloc(sizeof(Server));

  server->server_sock = Socket_Create();

  server->addr.sin_family = AF_INET;
  server->addr.sin_port = htons(port);
  server->addr.sin_addr.s_addr = INADDR_ANY;
  server->addr_len = sizeof(server->addr);

  server->client_socks = NULL;
  server->num_clients = 0;
  server->max_clients = 0;

  Socket_Bind(server->server_sock, &server->addr, server->addr_len);

  Socket_Listen(server->server_sock, 1); // Temporary 1, will change later.

  return server;
}

void Server_Destroy(Server* server) {
  if (!server) {
    return;
  }

  if (server->client_socks) {
    for (int i = 0; i < server->num_clients; i++) {
      Socket_Destroy(server->client_socks[i]);
    }
    free(server->client_socks);
  }

  Socket_Destroy(server->server_sock);

  free(server);
}

void Server_Accept(Server* server) {
  Socket* client_sock = Socket_Accept(server->server_sock);

  if (server->num_clients + 1 > server->max_clients) {
    if (!server->client_socks) {
      server->client_socks = (Socket**)malloc(sizeof(Socket*) * 1);
      server->max_clients = 1;
    } else {
      server->client_socks = (Socket**)realloc(server->client_socks, sizeof(Socket*) * server->max_clients * 2);
      server->max_clients *= 2;
    }
  }

  server->client_socks[server->num_clients] = client_sock;
  server->num_clients++;
}
