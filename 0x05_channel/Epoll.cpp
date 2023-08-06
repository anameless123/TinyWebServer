#include "Epoll.h"
#include "../utils.hpp"
#include "Channel.h"

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

int Epoll::getFd()
{
    return epfd;
}

std::vector<epoll_event> Epoll::poll()
{
    int nfds = epoll_wait(epfd, evs, MAX_EVENTS, -1);
    Utils::errif(nfds == -1, "Failed to poll!");
    // return std::vector<epoll_event> (evs, evs+sizeof(evs)/sizeof(epoll_event));
    std::vector<epoll_event> eventv;
    for (int i = 0; i < nfds; ++i)
        eventv.emplace_back(std::move(evs[i]));
    return eventv;
}

std::vector<Channel *> Epoll::pollChannel()
{
    int nfds = epoll_wait(epfd, evs, MAX_EVENTS, -1);
    Utils::errif(nfds == -1, "Failed to poll Channel!");
    // return std::vector<epoll_event> (evs, evs+sizeof(evs)/sizeof(epoll_event));
    std::vector<Channel*> channelv;
    for (int i = 0; i < nfds; ++i)
    {
        Channel *ch = (Channel*)evs[i].data.ptr;
        ch->setRevents(evs[i].events);
        channelv.emplace_back(ch);
    }
    return channelv;
}

void Epoll::updateChannel(Channel *channel)
{
    int fd = channel->getFd();
    epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.data.ptr = channel;
    ev.events = channel->getEvents();
    if (!channel->getInEpoll())
    {
        Utils::errif(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1, "Failed to add epoll");
        channel->setInEpoll();
    }
    else
    {
        Utils::errif(epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev) == -1, "Failed to mod epoll");
    }
}