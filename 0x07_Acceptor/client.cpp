#include "src/Socket.h"
#include "src/InetAddress.hpp"
int main()
{
    Socket *clnt_sock = new Socket;
    InetAddress serv_addr(AF_INET, "127.0.0.1", 8888);

    clnt_sock->connect(serv_addr);
    Utils::keepsend(clnt_sock->getFd());

    return 0;
}