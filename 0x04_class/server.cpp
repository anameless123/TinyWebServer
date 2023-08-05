#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <cstring>
#include "Socket.hpp"
#include "Epoll.hpp"
#include "../utils.hpp"

int main()
{
    Socket *serv_sock = new Socket;
    InetAddress serv_addr(AF_INET, "127.0.0.1", 8888);
    serv_sock->bind(serv_addr);
    serv_sock->listen();

    Epoll *myepoll = new Epoll;
    myepoll->addFd(serv_sock->getfd());

    InetAddress clnt_addr;
    while (true)
    {
        std::vector<epoll_event> events = myepoll->poll();
        for (auto e : events)
        {
            if (e.data.fd == serv_sock->getfd())
            {
                int clnt_sockfd = serv_sock->accept(clnt_addr);
                setnonblocking(clnt_sockfd);
                myepoll->addFd(clnt_sockfd, EPOLLIN | EPOLLOUT);
            }
            else if (e.events & EPOLLIN)
            {
                handleEvent(e.data.fd);
            }
        }
    }
}