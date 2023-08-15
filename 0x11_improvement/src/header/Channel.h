#pragma once
#include <functional>

class EventLoop;

class Channel
{
private:
    EventLoop *loop;
    int fd;
    uint32_t events;
    uint32_t revents;
    bool inEpoll;
    std::function<void()> callback;

public:
    Channel(EventLoop *loop, int fd);
    ~Channel();

    void handleEvent();
    void enableReading();

    int getFd();
    uint32_t getEvents();
    uint32_t getRevents();
    void setRevents(uint32_t evts);
    bool getInEpoll();
    void setInEpoll();

    void setCallback(std::function<void()>);
};