#include "Channel.h"
#include "EventLoop.h"

Channel::Channel(EventLoop* lp, int fd)
    : loop(lp), fd(fd), events(0), revents(0), inEpoll(false) {
}
Channel::~Channel() {}

void Channel::enableReading() {
    events = EPOLLIN | EPOLLET;
    loop->updateChannel(this);
}

int Channel::getFd() {
    return fd;
}

uint32_t Channel::getEvents() {
    return events;
}

uint32_t Channel::getRevents() {
    return revents;
}

bool Channel::getInEpoll() {
    return inEpoll;
}

void Channel::setInEpoll() {
    inEpoll = true;
}

void Channel::setRevents(uint32_t evts) {
    revents = evts;
}

void Channel::setCallback(std::function<void()> cb) {
    callback = cb;
}

void Channel::handleEvent() {
    callback();
}