#include "Channel.h"
#include "Epoll.h"

Channel::Channel(Epoll *epo, int fd) : ep(epo), fd(fd), events(0), revents(0), inEpoll(false)
{
}

Channel::~Channel() {}
void Channel::enableReading()
{
    events = EPOLLIN | EPOLLET;
    ep->updateChannel(this);
}

int Channel::getFd()
{
    return fd;
}

uint32_t Channel::getEvents()
{
    return events;
}

bool Channel::getInEpoll()
{
    return inEpoll;
}

void Channel::setInEpoll()
{
    inEpoll = true;
}

void Channel::setRevents(uint32_t evts)
{
    revents = evts;
}