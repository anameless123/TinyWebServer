#include "Server.h"
#include <functional>
#include "Acceptor.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Socket.h"
#include "InetAddress.hpp"

#ifndef READ_BUFFER
#define READ_BUFFER 1024
#endif
Server::Server()
    : loop(nullptr), acceptor(nullptr) {
}
// 注册服务器socket，其事件就是新连接，绑定新连接方法
Server::Server(EventLoop* lp)
    : loop(lp), acceptor(nullptr) {
    acceptor = new Acceptor(loop);
    std::function<void(Socket*)> cb = std::bind(&Server::newConnection, this, std::placeholders::_1);
    acceptor->setNewConnectionCallback(cb);
}

Server::~Server() {
    if (loop != nullptr)
        delete loop;
}

void Server::handleReadEvent(int evfd) {
    char buf[1024];
    memset(buf, 0, sizeof(buf));
    ssize_t rbytes = 0;
    while (true) {  // 由于使用非阻塞IO，需要不断读取，直到全部读取完毕
        rbytes = read(evfd, buf, sizeof(buf));
        if (rbytes > 0) {
            std::cout << "message from clinet fd: " << evfd << ": " << buf << std::endl;
            // 原样返回
            write(evfd, buf, sizeof(buf));
        } else if (rbytes == -1 && errno == EINTR) {  // 客户端正常中断、继续读取
            continue;
        } else if (rbytes == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {  // 非阻塞IO，这个条件表示数据全部读取完毕
            // 该fd上数据读取完毕
            break;
        } else if (rbytes == 0) {  // EOF事件，一般表示客户端断开连接
            std::cout << "Client fd:" << evfd << " disconnected !" << std::endl;
            close(evfd);  // 关闭socket会自动将文件描述符从epoll树上移除
            break;
        }  // 剩下的bytes_read == -1的情况表示其他错误，这里没有处理
    }
}

// 新连接注册监听Channel，读事件就绑定读方法
void Server::newConnection(Socket* serv_sock) {
    InetAddress clnt_addr;
    Socket* clnt_sock = new Socket(serv_sock->accept(&clnt_addr));
    clnt_sock->setnonblocking();
    Channel* clnt_channel = new Channel(loop, clnt_sock->getFd());

    std::function<void()> cb = std::bind(&Server::handleReadEvent, this, clnt_sock->getFd());
    clnt_channel->setCallback(cb);
    clnt_channel->enableReading();
}