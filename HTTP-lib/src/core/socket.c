#include "./socket.h"
#include "core/string.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <syscall.h>
#include <unistd.h>

u32 mkInet4Addr(string *address) {
  if (streq(*address, mkstring("localhost"))) {
    *address = mkstring("127.0.0.1");
  } else if (streq(*address, mkstring("0.0.0.0"))) {
    return 0;
  }

  string addrstr[4] = {0};
  strsplit(*address, '.', addrstr, 4);

  return str2usize(addrstr[3]) << 24 | str2usize(addrstr[2]) << 16 |
         str2usize(addrstr[1]) << 8 | str2usize(addrstr[0]);
}

Socket mkIPv4Socket(string *addr, u16 port, communication_protocol protocol) {
  Socket res = {0};

  i32 fd = syscall(SYS_socket, IPv4, protocol, 0);
  if (fd < 0) {
    return res;
  }

  /* Allow reuse of addr and port number */
  i32 opt = 1;
  if (syscall(SYS_setsockopt, fd, 1, 15, &opt, sizeof(opt)) < 0) {
    return res;
  }

  struct socket_addr address = {
      .ip_version = IPv4,
      .port = host2net(port),
      .address = mkInet4Addr(addr),
  };

  if (syscall(SYS_bind, fd, (struct {
                u16 ip_version;
                u8 socket_addr_data[];
              } *)&address,
              sizeof(address)) < 0) {
    return res;
  }

  res.fd = fd;
  res.protocol = protocol;
  res.addr = address;

  return res;
}

ConnectionStatus listen(Socket socket, u8 backlog) {
  if (socket.fd == 0 || syscall(SYS_listen, socket.fd, backlog) < 0) {
    return CONNECTION_ERR;
  }

  return CONNECTION_OK;
}

struct HandlerData {
  void (*fn)(Socket);
  Socket client;
};

void *handler_wrapper(void *handler_data) {
  struct HandlerData *data = (struct HandlerData *)handler_data;

  data->fn(data->client);

  printf("Handled request from %s:%d\n",
         addr2str(data->client.addr.address).literal,
         net2host(data->client.addr.port));

  shutdown(data->client);
  socket_close(data->client);

  free(data);
  return 0;
}

void accept(const Socket *server, void (*handler)(Socket)) {
  if (server->fd <= 0) {
    fprintf(stderr, "Server fd is invalid\n");
    return;
  }

  struct socket_addr client_addr = {0};
  usize client_size = sizeof(client_addr);
  i32 client_fd;

  if ((client_fd = syscall(SYS_accept, server->fd, &client_addr, &client_size)) < 0) {
    fprintf(stderr, "Socket accept failed\n");
    return;
  }

  Socket client = {.fd = client_fd, .protocol = server->protocol, .addr = client_addr};

  struct HandlerData *data = malloc(sizeof(struct HandlerData));
  data->fn = handler;
  data->client = client;

  pthread_t t;
  pthread_create(&t, 0, handler_wrapper, (void *)data);
}

void *recv(Socket client, usize count, i32 flags) {
  /* TODO: implement malloc! */
  char *msg = malloc(sizeof(u8) * count);
  i32 read_count = syscall(SYS_recvfrom, client.fd, msg, count, flags, 0, 0);

  return msg;
}

void send(Socket client, void *msg, usize size, i32 flags) {
  if (syscall(SYS_sendto, client.fd, msg, size, flags, 0, 0) < 0) {
    printf("\tFailed to send data to socket %s:%d\n",
           addr2str(client.addr.address).literal, net2host(client.addr.port));
  }
}

void shutdown(Socket client) {
  if (syscall(SYS_shutdown, client.fd, 1) < 0) {
    printf("\tFailed to shutdown clientet %s:%d\n",
           addr2str(client.addr.address).literal, net2host(client.addr.port));
  } else {
    printf("\tSocket %s:%d has been shutdown\n",
           addr2str(client.addr.address).literal, net2host(client.addr.port));
  }
}

void socket_close(Socket client) {
  if (syscall(SYS_close, client.fd) < 0) {
    printf("\tFailed to close client %s:%d\n",
           addr2str(client.addr.address).literal, net2host(client.addr.port));
  } else {
    printf("\tSocket %s:%d has been closed\n",
           addr2str(client.addr.address).literal, net2host(client.addr.port));
  }
}

u16 host2net(u16 port) { return (port >> 8) | (u16)(port << 8); }

u16 net2host(u16 port) { return (u16)(port << 8) | (port >> 8); }

string addr2str(u32 addr) {
  char *parts = malloc(sizeof(char) * (4 * 3 + 3 + 1));
  sprintf(parts, "%d.%d.%d.%d", (u8)addr, (u8)(addr >> 8), (u8)(addr >> 16),
          (u8)(addr >> 24));

  return mkstring(parts);
}
