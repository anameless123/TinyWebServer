#pragma once
class EventLoop;
class Socket;
class InetAddress;

class Server
{
private:
    EventLoop *loop;
public:
    Server();
    Server(EventLoop*);
    Server(EventLoop*, InetAddress);
    ~Server();
    void handleReadEvent(int);
    void newConnection(Socket *serv_sock);
};