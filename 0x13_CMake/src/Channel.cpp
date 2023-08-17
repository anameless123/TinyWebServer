#include "Channel.h"
#include <sys/epoll.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include "EventLoop.h"

Channel::Channel(EventLoop *lp, int fd) : loop(lp), fd(fd), events(0), ready(0), inEpoll(false) {
  printf("new channel\n");
}
Channel::~Channel() {
  if (fd != -1) {
    close(fd);
    fd = -1;
  }
}
void Channel::handleEvent() {
  if (ready & (EPOLLIN | EPOLLPRI)) {
    readcallback();
  }
  if (ready & (EPOLLOUT)) {
    writecallback();
  }
  // callback();
}
void Channel::enableRead() {
  events = EPOLLIN | EPOLLPRI;
  loop->updateChannel(this);
}

void Channel::useET() {
  events |= EPOLLET;
  loop->updateChannel(this);
}
int Channel::getFd() { return fd; }

uint32_t Channel::getEvents() { return events; }

uint32_t Channel::getReady() { return ready; }

bool Channel::getInEpoll() { return inEpoll; }

void Channel::setInEpoll(bool _in) { inEpoll = _in; }

void Channel::setReady(uint32_t _evts) { ready = _evts; }

void Channel::setReadCallback(std::function<void()> _cb) { readcallback = _cb; }

void Channel::setWriteCallback(std::function<void()> _cb) { writecallback = _cb; }
