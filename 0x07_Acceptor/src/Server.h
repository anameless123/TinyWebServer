#pragma once
class EventLoop;
class Socket;
class InetAddress;
class Acceptor;

class Server
{
private:
    EventLoop *loop;
    Acceptor *acceptor;
public:
    Server();
    Server(EventLoop*);
    ~Server();
    
    void handleReadEvent(int);
    void newConnection(Socket *serv_sock);
};