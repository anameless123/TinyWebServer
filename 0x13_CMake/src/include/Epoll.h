#pragma once
#include <sys/epoll.h>
#include <vector>

#ifndef MAX_EVENTS
#define MAX_EVENTS 64
#endif

class Channel;

class Epoll {
 private:
  int epfd = 0;
  epoll_event *evs;

 public:
  Epoll();
  ~Epoll();
  // 0x04 version
  // void addFd(int fd, uint32_t flag = EPOLLIN | EPOLLET);
  // std::vector<epoll_event> poll();
  std::vector<Channel *> pollChannel();
  void updateChannel(Channel *channel);
};
