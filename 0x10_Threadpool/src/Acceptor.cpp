#include "header/Acceptor.h"
#include "header/Channel.h"
#include "header/Server.h"
#include "header/Socket.h"

Acceptor::Acceptor(EventLoop* _loop)
    : loop(_loop) {
    serv_sock = new Socket;
    addr = new InetAddress(AF_INET, "127.0.0.1", 8888);
    serv_sock->bind(*addr);
    serv_sock->listen();
    serv_sock->setnonblocking();

    acceptChannel = new Channel(loop, serv_sock->getFd());
    std::function<void()> cb = std::bind(&Acceptor::acceptConnection, this);
    acceptChannel->setCallback(cb);
    acceptChannel->enableReading();
}
Acceptor::~Acceptor() {
    delete serv_sock;
    delete acceptChannel;
}

void Acceptor::acceptConnection() {
    InetAddress clnt_addr;
    Socket *clnt_sock = new Socket(serv_sock->accept(&clnt_addr), clnt_addr);
    clnt_sock->setnonblocking();
    newConnectionCallback(clnt_sock);
}

void Acceptor::setNewConnectionCallback(std::function<void(Socket*)> _cb) {
    newConnectionCallback = _cb;
}