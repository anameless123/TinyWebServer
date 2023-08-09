#include "Connection.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Socket.h"

#include <cstring>
#include <unistd.h>

Connection::Connection(EventLoop *_loop, Socket *_sock)
    : loop(_loop), sock(_sock), channel(nullptr) {
    channel = new Channel(loop, sock->getFd());
    std::function<void()> cb = std::bind(&Connection::echo, this, sock->getFd());
    channel->setCallback(cb);
    channel->enableReading();
}
Connection::~Connection(){
    delete channel;
    delete sock;
}
void Connection::echo(int fd) {
    char buf[1024];
    memset(buf, 0, sizeof(buf));
    ssize_t rbytes = 0;
    while (true) { // 由于使用非阻塞IO，需要不断读取，直到全部读取完毕
        rbytes = read(fd, buf, sizeof(buf));
        if (rbytes > 0) {
            std::cout << "message from clinet fd: " << fd << ": " << buf << std::endl;
            // 原样返回
            write(fd, buf, sizeof(buf));
        } else if (rbytes == -1 && errno == EINTR) { // 客户端正常中断、继续读取
            continue;
        } else if (rbytes == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) { // 非阻塞IO，这个条件表示数据全部读取完毕
            // 该fd上数据读取完毕
            break;
        } else if (rbytes == 0) { // EOF事件，一般表示客户端断开连接
            std::cout << "Client fd:" << fd << " already disconnected !" << std::endl;
            deleteConnectionCallback(sock); // 关闭socket会自动将文件描述符从epoll树上移除
            break;
        } // 剩下的bytes_read == -1的情况表示其他错误，这里没有处理
    }
}

void Connection::setDeleteConnectionCallback(std::function<void(Socket *)> _cb) {
    deleteConnectionCallback = _cb;
}
