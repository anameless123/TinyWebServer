#include "header/Server.h"
#include "header/Acceptor.h"
#include "header/Connection.h"
#include "header/EventLoop.h"
#include "header/Socket.h"
#include "header/ThreadPool.h"
#include <functional>
#include <unistd.h>

#ifndef READ_BUFFER
#define READ_BUFFER 1024
#endif
// 注册服务器socket，其事件就是新连接，绑定新连接方法
Server::Server(EventLoop *_loop) : mainReactor(_loop), acceptor(nullptr) {
    acceptor = new Acceptor(mainReactor);
    std::function<void(Socket *)> cb = std::bind(&Server::newConnection, this, std::placeholders::_1);
    acceptor->setNewConnectionCallback(cb);

    int size = std::thread::hardware_concurrency();
    thpool = new ThreadPool(size);
    // 一个线程对应一个eventloop
    for (int i = 0; i < size; ++i) {
        subReactors.emplace_back(new EventLoop());
    }

    for (int i = 0; i < size; ++i) {
        // bind将对象subReactors[i]（地址）与成员函数loop（地址）绑定，使其可以像普通函数一样调用
        std::function<void()> sub_loop = std::bind(&EventLoop::loop, subReactors[i]);
        thpool->add(sub_loop);
    }
}

Server::~Server() {
    delete acceptor;
    delete thpool;
}

// 新连接注册监听Channel，注册到subReactor中
void Server::newConnection(Socket *clnt_sock) {
    if (clnt_sock->getFd() != -1) {
        int random = clnt_sock->getFd() % subReactors.size();
        Connection *conn = new Connection(subReactors[random], clnt_sock);
        std::function<void(int)> cb = std::bind(&Server::deleteConnection, this, std::placeholders::_1);
        conn->setDeleteConnectionCallback(cb);
        connections[clnt_sock->getFd()] = conn;
    }
}

void Server::deleteConnection(int sockfd) {
    if (sockfd != -1) {
        auto it = connections.find(sockfd);
        if (it != connections.end()) {
            Connection *conn = connections[sockfd];
            connections.erase(sockfd);
            delete conn;
        }
    }
}