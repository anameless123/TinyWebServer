#include "src/header/Server.h"
#include "src/header/EventLoop.h"

int main()
{
    EventLoop *loop = new EventLoop;
    Server *server = new Server(loop);
    loop->loop();
    
    delete loop;
    delete server;
    return 0;
}