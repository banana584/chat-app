#include "../../include/server/server.h"

Server* Server_Create(int port) {
  Server* server = (Server*)malloc(sizeof(Server));

  server->server_sock = Socket_Create();

  server->addr.sin_family = AF_INET;
  server->addr.sin_port = htons(port);
  server->addr.sin_addr.s_addr = INADDR_ANY;
  server->addr_len = sizeof(server->addr);

  server->client_socks_head = NULL;
  server->num_clients = 0;

  Socket_Bind(server->server_sock, &server->addr, server->addr_len);

  Socket_Listen(server->server_sock, 1); // Temporary 1, will change later.

  return server;
}

void Server_Destroy(Server* server) {
  if (!server) {
    return;
  }

  if (server->client_socks_head) {
    Node* current = server->client_socks_head;
    Node* tmp = NULL;
    while (current) {
      Socket_Destroy(current->data);
      tmp = current->next;
      free(current);
      current = tmp;
    }
  }

  Socket_Destroy(server->server_sock);

  free(server);
}

void Server_Accept(Server* server) {
  Socket* client_sock = Socket_Accept(server->server_sock);

  Node* node = (Node*)malloc(sizeof(Node));
  node->data = client_sock;
  node->id = server->num_clients + 1;
  node->next = server->client_socks_head;
  server->client_socks_head = node;
  server->num_clients++;
}

void Server_Disconnect(Server* server, int idx) {
  Node* prev = NULL;
  Node* current = server->client_socks_head;
  while (current) {
    if (current->id == idx) {
      break;
    }
    prev = current;
    current = current->next;
  }

  if (!current) {
    return;
  }

  Socket_Destroy(current->data);
  if (!prev) {
    server->client_socks_head = NULL;
  } else {
    prev->next = current->next;
  }
  free(current);
}

int Server_Send(Server* server, int idx, Message* message) {
  Node* current = server->client_socks_head;
  while (current) {
    if (current->id == idx) {
      break;
    }
    current = current->next;
  }

  if (!current) {
    return -1;
  }

  return Socket_Send(current->data, message);
}

Message* Server_Recv(Server* server, int idx) {
  Node* current = server->client_socks_head;
  while (current) {
    if (current->id == idx) {
      break;
    }
    current = current->next;
  }

  if (!current) {
    return NULL;
  }

  return Socket_Recieve(current->data);
}

int Server_Sendall(Server* server, Message* message) {
  Node* current = server->client_socks_head;
  while (current) {
    Socket_Send(current->data, message);
    current = current->next;
  }

  return 0;
}
