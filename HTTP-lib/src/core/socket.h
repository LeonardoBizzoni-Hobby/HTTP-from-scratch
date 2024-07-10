#include "type.h"

typedef enum {
  IPv4 = 2,
  IPv6 = 10,
} ip_version;

typedef enum {
  TCP = 1,
  UDP = 2,
} communication_protocol;

struct socket_addr {
  u16 ip_version;
  u16 port;
  u32 address;
  u8 zeros[8];
};

typedef struct {
  i32 fd;
  communication_protocol protocol;
  struct socket_addr addr;
} Socket;

typedef enum {
  CONNECTION_ERR,
  CONNECTION_OK,
} ConnectionStatus;

u32 mkInet4Addr(string *address);

Socket mkIPv4Socket(string *addr, u16 port, communication_protocol protocol);

ConnectionStatus listen(Socket socket, u8 backlog);
void accept(const Socket *server, void (*handler)(Socket client_socket));

void *recv(Socket client, usize count, i32 flags);
void send(Socket client, void *msg, usize size, i32 flags);
void shutdown(Socket sock);
void socket_close(Socket sock);

u16 host2net(u16 port);
u16 net2host(u16 port);
string addr2str(u32 addr);
