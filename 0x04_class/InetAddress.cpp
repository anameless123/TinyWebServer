#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include "InetAddress.hpp"

InetAddress::InetAddress()
{
    std::memset(&siaddr, 0, sizeof(siaddr));
}
InetAddress::InetAddress(const uint16_t &type, const char *IP, const short &port)
{
    siaddr.sin_family = type;
    siaddr.sin_port = htons(port);
    siaddr.sin_addr.s_addr = inet_addr(IP);
}

InetAddress::~InetAddress() {}

sockaddr *InetAddress::generilize()
{
    return (sockaddr *)&siaddr;
}

uint32_t InetAddress::size()
{
    return sizeof(siaddr);
}

uint16_t InetAddress::getv()
{
    return siaddr.sin_family;
}