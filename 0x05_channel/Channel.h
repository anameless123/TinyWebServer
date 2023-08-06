#pragma once
#include "../utils.hpp"

class Epoll;

class Channel
{
private:
    Epoll *ep;
    int fd;
    uint32_t events;
    uint32_t revents;
    bool inEpoll;

public:
    Channel(Epoll *epo, int fd);
    ~Channel();
    
    void enableReading();

    int getFd();
    uint32_t getEvents();
    void setRevents(uint32_t evts);
    bool getInEpoll();
    void setInEpoll();
};