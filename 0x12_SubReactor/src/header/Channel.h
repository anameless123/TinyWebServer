#pragma once
#include <functional>

class EventLoop;

class Channel
{
private:
    EventLoop *loop;
    int fd;
    uint32_t events;
    uint32_t ready;
    bool inEpoll;
    std::function<void()> readcallback;
    std::function<void()> writecallback;

public:
    Channel(EventLoop *loop, int fd);
    ~Channel();

    void handleEvent();
    void enableRead();

    int getFd();
    uint32_t getEvents();
    uint32_t getReady();
    void useET();
    bool getInEpoll();
    void setInEpoll(bool _in = true);

    void setReady(uint32_t);
    void setReadCallback(std::function<void()>);
    void setWriteCallback(std::function<void()>);
};