#include "EventLoop.h"
#include "Channel.h"
#include "Epoll.h"
#include "ThreadPool.h"

EventLoop::EventLoop() : ep(nullptr), quit(false) { ep = new Epoll(); }
EventLoop::~EventLoop() { delete ep; }

void EventLoop::loop() {
  while (!quit) {
    std::vector<Channel *> channels = std::move(ep->pollChannel());
    for (auto ch : channels) ch->handleEvent();
  }
}

void EventLoop::updateChannel(Channel *ch) { ep->updateChannel(ch); }
