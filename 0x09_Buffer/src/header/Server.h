#pragma once
#include <map>

class EventLoop;
class Acceptor;
class Connection;
class Socket;

class Server
{
private:
    EventLoop *loop;
    Acceptor *acceptor;
    std::map<int, Connection*> connections;
public:
    Server();
    Server(EventLoop*);
    ~Server();
    
    void handleReadEvent(int);
    void newConnection(Socket *serv_sock);
    void deleteConnection(Socket *sock);
};