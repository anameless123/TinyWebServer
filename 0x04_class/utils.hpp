#pragma once

#include <iostream>
#include <unistd.h>
#include <cstring>
#include <fcntl.h>

using std::cin;
using std::cout;
using std::endl;

class Utils
{
public:
    static void errif(bool condition, const char *errmsg)
    {
        if (condition)
        {
            perror(errmsg);
            exit(EXIT_FAILURE);
        }
    }

    static void keepsend(int fd)
    {
        char buf[1024];
        while (true)
        {
            memset(buf, 0, sizeof(buf));
            cin >> buf;
            ssize_t wbytes = write(fd, buf, sizeof(buf));
            cout << wbytes << endl;

            if (wbytes == -1)
            {
                errif(wbytes == -1, "Failed to write, socket already disconnected!");
                break;
            }
            ssize_t read_bytes = read(fd, buf, sizeof(buf));

            if (read_bytes > 0)
            {
                cout << "message from server: " << buf << endl;
            }
            else if (read_bytes == 0)
            {
                cout << "server socket disconnected" << endl;
                break;
            }
            else if (read_bytes == -1)
            {
                close(fd);
                errif(true, "socket read error");
            }
        }
    }

    static void setnonblocking(const int &sockfd)
    {
        fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFL, 0) | O_NONBLOCK);
    }

    static void handleEvent(const int &evfd)
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
};
