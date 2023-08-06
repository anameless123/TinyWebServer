#pragma once
#include <sys/epoll.h>
#include <vector>
#include "../utils.hpp"

#ifndef MAX_EVENTS
#define MAX_EVENTS 64
#endif
class Epoll
{
private:
    int epfd = 0;

public:
    epoll_event *evs;
    Epoll();
    ~Epoll();
    void addFd(int fd, uint32_t flag = EPOLLIN | EPOLLET);
    int getfd();
    std::vector<epoll_event> poll();
};

Epoll::Epoll()
{
    epfd = epoll_create1(0);
    if (epfd == -1)
    {
        perror("Failed to create epoll! ");
        exit(EXIT_FAILURE);
    }
    else
    {
        evs = new epoll_event[MAX_EVENTS];
        memset(evs, 0, sizeof(*evs) * MAX_EVENTS);
        cout << "create epoll:" << epfd << endl;
    }
}
Epoll::~Epoll()
{
    if (epfd != -1)
        close(epfd);
    delete[] evs;
}
void Epoll::addFd(int fd, uint32_t flag)
{
    epoll_event ev;
    ev.data.fd = fd;
    ev.events = flag;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1)
    {
        perror("Failed to add to epoll! ");
        exit(EXIT_FAILURE);
    }
    else
        cout << "add epoll:" << fd << endl;
}

int Epoll::getfd()
{
    return epfd;
}

std::vector<epoll_event> Epoll::poll()
{
    int nfds = epoll_wait(epfd, evs, MAX_EVENTS, -1);
    // return std::vector<epoll_event> (evs, evs+sizeof(evs)/sizeof(epoll_event));
    std::vector<epoll_event> eventv;
    for (int i = 0; i < nfds; ++i)
        eventv.emplace_back(std::move(evs[i]));
    return eventv;
}
