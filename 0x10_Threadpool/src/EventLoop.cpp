#include "header/EventLoop.h"
#include "header/Channel.h"
#include "header/Epoll.h"
#include "header/ThreadPool.h"

EventLoop::EventLoop() : ep(nullptr), threadpool(nullptr), quit(false) {
    ep = new Epoll();
    threadpool = new ThreadPool;
}
EventLoop::~EventLoop() {
    delete ep;
    delete threadpool;
}

void EventLoop::loop() {
    while (!quit) {
        std::vector<Channel *> channels = std::move(ep->pollChannel());
        for (auto ch : channels)
            ch->handleEvent();
    }
}

void EventLoop::updateChannel(Channel *ch) {
    ep->updateChannel(ch);
}

void EventLoop::addThread(std::function<void()> func){
    threadpool->add(func);
}