#include "Server.h"
#include "Acceptor.h"
#include "EventLoop.h"
#include "Socket.h"
#include "Connection.h"
#include <functional>

#ifndef READ_BUFFER
#define READ_BUFFER 1024
#endif
Server::Server() : loop(nullptr), acceptor(nullptr) {
}
// 注册服务器socket，其事件就是新连接，绑定新连接方法
Server::Server(EventLoop *lp) : loop(lp), acceptor(nullptr) {
    acceptor = new Acceptor(loop);
    std::function<void(Socket *)> cb = std::bind(&Server::newConnection, this, std::placeholders::_1);
    acceptor->setNewConnectionCallback(cb);
}

Server::~Server() {
    if (loop != nullptr)
        delete loop;
}

// 新连接注册监听Channel
void Server::newConnection(Socket *clnt_sock) {
    Connection *conn = new Connection(loop, clnt_sock);
    std::function<void(Socket*)> cb = std::bind(&Server::deleteConnection, this, std::placeholders::_1);
    conn->setDeleteConnectionCallback(cb);
    connections[clnt_sock->getFd()] = conn;
}

void Server::deleteConnection(Socket *serv_sock) {
    Connection *conn = connections[serv_sock->getFd()];
    connections.erase(serv_sock->getFd());
    delete conn;
}