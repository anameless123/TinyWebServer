#pragma once
#include <sys/socket.h>
#include "InetAddress.hpp"
#include "utils.hpp"

class Socket
{
private:
    int sockfd = 0;
    // IP ver, default = IPv4;
    int IP = AF_INET;
    // Max Connection, default = 128;
    int MaxCon = 128;

public:
    Socket();
    Socket(int sockfd);
    Socket(__socket_type tran, uint16_t inet);
    ~Socket();
    void bind(InetAddress addr);
    void listen();
    void connect(InetAddress addr);
    int accept(InetAddress *addr);
    void setnonblocking();
    int getFd();
};
Socket::Socket()
{
    sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("Failed to initialize socket!");
        exit(EXIT_FAILURE);
    }
    else
        cout << "socket initialize succeed!" << endl;
}
Socket::Socket(int fd) : sockfd(fd)
{
    Utils::errif(fd == -1, "Failed to copy construct Socket by fd");
}

Socket::Socket(__socket_type tran, uint16_t inet)
{
    if (sockfd = ::socket(IP, tran, 0) == -1)
    {
        perror("Failed to initialize socket!");
        exit(EXIT_FAILURE);
    }
    else
        cout << "socket initialize succeed!" << endl;
}
Socket::~Socket()
{
    if (sockfd != -1)
    {
        close(sockfd);
        sockfd = -1;
    }
}
void Socket::bind(InetAddress addr)
{
    if (addr.getv() == IP)
    {
        if (::bind(sockfd, addr.generilize(), addr.size()) == -1)
        {
            perror("Failed to bind!");
            exit(EXIT_FAILURE);
        }
        else
            cout << "Server bind at: " << inet_ntoa(addr.siaddr.sin_addr)
                 << ":" << ntohs(addr.siaddr.sin_port) << endl;
    }
    else
    {
        std::cout << "address ipVersion != socket ipVersion" << std::endl;
        exit(1);
    }
}

void Socket::connect(InetAddress addr)
{
    if (addr.getv() == IP)
    {
        if (::connect(sockfd, addr.generilize(), addr.size()) == -1)
        {
            perror("Failed to connect!");
            exit(EXIT_FAILURE);
        }
        else
            cout << "Connect to: " << inet_ntoa(addr.siaddr.sin_addr)
                 << ":" << ntohs(addr.siaddr.sin_port) << endl;
    }
    else
    {
        cout << "address ipVersion != socket ipVersion" << endl;
        exit(1);
    }
}

void Socket::listen()
{
    if (::listen(sockfd, MaxCon) == -1)
    {
        perror("Failed to listen!");
        exit(EXIT_FAILURE);
    }
    else
        cout << "listening..." << endl;
}

int Socket::accept(InetAddress *clntaddr)
{
    socklen_t size = clntaddr->size();
    int res = ::accept(sockfd, clntaddr->generilize(), &size);
    if (size != clntaddr->size())
    {
        std::cout << "Failed to accept, address is cutdown" << std::endl;
        res = -1;
    }
    if (res != -1)
        cout << "Client accepted in Fd: " << res << ", " << inet_ntoa(clntaddr->siaddr.sin_addr)
             << ":" << ntohs(clntaddr->siaddr.sin_port) << endl;
    return res;
}

int Socket::getFd()
{
    return sockfd;
}

void Socket::setnonblocking()
{
    fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFL, 0) | O_NONBLOCK);
}