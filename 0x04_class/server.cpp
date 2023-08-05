#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <cstring>
#include "InetAddress.hpp"
#include "Socket.hpp"
#include "Epoll.hpp"
#include "../utils.hpp"

class Server
{
private:
public:
    Server();
    ~Server();

    static void setnonblocking(const int &);
    static void handleEvent(const int &);
};

void Server::setnonblocking(const int &sockfd)
{
    fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFL, 0) | O_NONBLOCK);
}

void Server::handleEvent(const int &evfd)
{
    char buf[1024];
    memset(buf, 0, sizeof(buf));
    ssize_t rbytes = 0;
    while (true)
    { // 由于使用非阻塞IO，需要不断读取，直到全部读取完毕
        rbytes = read(evfd, buf, sizeof(buf));
        if (rbytes > 0)
        {
            std::cout << "message from clinet fd: " << evfd << ": " << buf << std::endl;
            // 原样返回
            write(evfd, buf, sizeof(buf));
        }
        else if (rbytes == -1 && errno == EINTR)
        { // 客户端正常中断、继续读取
            continue;
        }
        else if (rbytes == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK)))
        { // 非阻塞IO，这个条件表示数据全部读取完毕
            // 该fd上数据读取完毕
            break;
        }
        else if (rbytes == 0)
        { // EOF事件，一般表示客户端断开连接
            std::cout << "Client fd:" << evfd << " disconnected !" << std::endl;
            close(evfd); // 关闭socket会自动将文件描述符从epoll树上移除
            break;
        } // 剩下的bytes_read == -1的情况表示其他错误，这里没有处理
    }
}

int main()
{
    Socket *serv_sock = new Socket;
    InetAddress serv_addr(AF_INET, "127.0.0.1", 8888);
    serv_sock->bind(serv_addr);
    serv_sock->listen();

    Epoll *myepoll = new Epoll;
    myepoll->addFd(serv_sock->getfd());

    InetAddress clnt_addr;
    while (true)
    {
        std::vector<epoll_event> events = myepoll->poll();
        for (auto e : events)
        {
            if (e.data.fd == serv_sock->getfd())
            {
                int clnt_sockfd = serv_sock->accept(clnt_addr);
                Server::setnonblocking(clnt_sockfd);
                myepoll->addFd(clnt_sockfd, EPOLLIN | EPOLLOUT);
            }
            else if(e.events & EPOLLIN)
            {
                Server::handleEvent(e.data.fd);
            }
        }
    }
}