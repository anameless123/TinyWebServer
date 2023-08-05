#include <sys/epoll.h>
#include <unistd.h>
#include <vector>
#include "Epoll.hpp"
#include "../utils.hpp"

Epoll::Epoll()
{
    epfd = epoll_create1(0);
    errif(epfd == -1, "Failed to create epoll! ");
}
Epoll::~Epoll()
{
    close(epfd);
}
void Epoll::addFd(int fd, uint32_t flag)
{
    ev.data.fd = fd;
    ev.events = flag;
    errif(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1, "Failed to add to epoll!");
}

int Epoll::getfd()
{
    return epfd;
}

std::vector<epoll_event> Epoll::poll()
{
    epoll_event evs[MAX_EVENTS];
    epoll_wait(epfd, evs, MAX_EVENTS, -1);
    // return std::vector<epoll_event> (evs, evs+sizeof(evs)/sizeof(epoll_event));
    return std::vector<epoll_event>(std::begin(evs), std::end(evs));
}