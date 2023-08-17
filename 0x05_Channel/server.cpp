#include "Socket.hpp"
#include "Epoll.h"
#include "Channel.h"

int main()
{
    Socket *serv_sock = new Socket;
    InetAddress serv_addr(AF_INET, "127.0.0.1", 8888);
    serv_sock->bind(serv_addr);
    serv_sock->listen();
    Utils::setnonblocking(serv_sock->getFd());

    Epoll *myepoll = new Epoll;
    Channel *serv_ch = new Channel(myepoll, serv_sock->getFd());
    serv_ch->enableReading();

    InetAddress clnt_addr;
    while (true)
    {
        std::vector<Channel *> channels = std::move(myepoll->pollChannel());
        for (auto ch : channels)
        {
            if (ch->getFd() == serv_sock->getFd())
            {
                int clnt_sockfd = serv_sock->accept(clnt_addr);
                Utils::setnonblocking(clnt_sockfd);
                Channel *clnt_ch = new Channel(myepoll, clnt_sockfd);
                clnt_ch->enableReading();
            }
            else if (ch->getEvents() & EPOLLIN)
            {
                Utils::handleEvent(ch->getFd());
            }
            else
            {
                cout << "something else in channel" << endl;
            }
        }
    }
    delete serv_sock;
    return 0;
}