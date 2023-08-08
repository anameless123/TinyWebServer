#pragma once
#include <sys/socket.h>
#include "InetAddress.hpp"
#include "utils.hpp"

class Socket
{
private:
    int sockfd = 0;

public:
    // IP ver, default = IPv4;
    int IP = AF_INET;
    // Max Connection, default = 128;
    int MaxCon = 128;

    Socket();
    Socket(__socket_type tran, uint16_t inet);
    ~Socket();
    void bind(InetAddress addr);
    void listen();
    void connect(InetAddress addr);
    int accept(InetAddress &addr);
    int getfd();
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
    close(sockfd);
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
            cout << "socket bind succeed!" << endl;
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
            cout << "connect succeed!" << endl;
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

int Socket::accept(InetAddress &clntaddr)
{
    socklen_t size = clntaddr.size();
    int res = ::accept(sockfd, clntaddr.generilize(), &size);
    if (size != clntaddr.size())
    {
        std::cout << "Failed to accept, address is cutdown" << std::endl;
        res = -1;
    }
    if (res != -1)
        cout << "accepted " << res << endl;
    return res;
}
int Socket::getfd()
{
    return sockfd;
}
