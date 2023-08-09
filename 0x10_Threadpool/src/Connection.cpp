#include "header/Connection.h"
#include "header/Buffer.h"
#include "header/Channel.h"
#include "header/EventLoop.h"
#include "header/Socket.h"
#include "header/utils.hpp"
#include <unistd.h>

Connection::Connection(EventLoop *_loop, Socket *_sock)
    : loop(_loop), sock(_sock), channel(nullptr) {
    channel = new Channel(loop, sock->getFd());
    std::function<void()> cb = std::bind(&Connection::echo, this, sock->getFd());
    channel->setCallback(cb);
    channel->enableReading();
    readBuffer = new Buffer;
}
Connection::~Connection() {
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
            readBuffer->append(buf, rbytes);
        } else if (rbytes == -1 && errno == EINTR) { // 客户端正常中断、继续读取
            cout << "...reading..." << endl;
            continue;
        } else if (rbytes == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) { // 非阻塞IO，这个条件表示数据全部读取完毕
            // 该fd上数据读取完毕
            cout << "...finish reading" << endl;
            cout << "message from clinet: ";
            sock->printIP();
            cout << ":";
            sock->printPort();
            cout << endl
                 << readBuffer->c_str() << endl;
            Utils::errif(write(fd, readBuffer->c_str(), readBuffer->size()) == -1, "socket write error");
            readBuffer->clear();
            break;
        } else if (rbytes == 0) { // EOF事件，一般表示客户端断开连接
            cout << "Client fd:" << fd << " already disconnected !" << endl;
            deleteConnectionCallback(sock); // 关闭socket会自动将文件描述符从epoll树上移除
            break;
        } // 剩下的bytes_read == -1的情况表示其他错误，这里没有处理
    }
}

void Connection::setDeleteConnectionCallback(std::function<void(Socket *)> _cb) {
    deleteConnectionCallback = _cb;
}
