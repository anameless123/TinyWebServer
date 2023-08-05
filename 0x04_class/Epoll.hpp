#include <sys/epoll.h>
#include <vector>

#ifndef MAX_EVENTS
#define MAX_EVENTS 64
#endif
class Epoll
{
private:
    int epfd = 0;

public:
    epoll_event ev;
    Epoll();
    ~Epoll();
    void addFd(int fd, uint32_t flag = EPOLLIN | EPOLLET);
    int getfd();
    std::vector<epoll_event> poll();
};
