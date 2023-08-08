#pragma once

#include <arpa/inet.h>
#include <cstring>
#include <iostream>

class InetAddress
{
public:
    sockaddr_in siaddr;

    InetAddress()
    {
        std::memset(&siaddr, 0, sizeof(siaddr));
    }
    InetAddress(int type, const char *IP, uint16_t port)
    {
        if (inet_pton(AF_INET, IP, &siaddr.sin_addr) <= 0)
        {
            // IP地址不合法,抛出异常或返回错误
            std::cout << "IP not legal!" << std::endl;
            exit(1);
        }
        else
        {
            siaddr.sin_family = type;

            siaddr.sin_family = type;
            siaddr.sin_port = htons(port);
        }
    }
    sockaddr *generilize()
    {
        return (sockaddr *)&siaddr;
    }
    uint16_t getv()
    {
        return siaddr.sin_family;
    }
    uint32_t size()
    {
        return sizeof(siaddr);
    }
    ~InetAddress() {}
};
