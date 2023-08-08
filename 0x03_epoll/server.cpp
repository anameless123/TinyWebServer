#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include "utils.hpp"

using namespace std;

#define MAX_EVENTS 64

int main()
{
    // socket初始化
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);
    Utils::errif(bind(sockfd, (sockaddr *)&serv_addr, sizeof(serv_addr)) == -1, "Failed to bind");
    Utils::errif(listen(sockfd, SOMAXCONN) == -1, "Failed to listen");
    
    // epoll初始化
    int epfd = epoll_create1(0);
    Utils::errif(epfd == -1, "Failed to create epoll! ");
    epoll_event evs[MAX_EVENTS], ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = sockfd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);
    
    // 事件处理
    int nfds = 0;
    sockaddr_in clnt_addr;
    memset(&clnt_addr, 0, sizeof(clnt_addr));
    socklen_t clnt_len = sizeof(clnt_addr);
    while (true)
    {
        nfds = epoll_wait(epfd, evs, MAX_EVENTS, -1);
        for (int i = 0; i < nfds; ++i)
        {
            // 建立连接请求
            if (evs[i].data.fd == sockfd)
            {
                int clnt_sockfd = accept(sockfd, (sockaddr *)&clnt_addr, &clnt_len);
                ev.data.fd = clnt_sockfd;
                ev.events = EPOLLIN | EPOLLOUT;

                Utils::setnonblocking(clnt_sockfd);
                epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sockfd, &ev);
            }
            else if (evs[i].events & EPOLLIN)
            {
                Utils::handleEvent(evs[i].data.fd);
            }
        }
    }
}