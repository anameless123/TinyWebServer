#include "Socket.h"
#include "utils.hpp"
int main() {
  Socket *clnt_sock = new Socket;
  InetAddress serv_addr(AF_INET, "127.0.0.1", 1234);
  clnt_sock->connect(serv_addr);
  Utils::keepsend(clnt_sock->getFd());
  return 0;
}
