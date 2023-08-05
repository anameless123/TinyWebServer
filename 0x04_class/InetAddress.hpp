#include <arpa/inet.h>

class InetAddress
{
private:
    sockaddr_in siaddr;

public:
    InetAddress();
    InetAddress(const uint16_t &type, const char *IP, const short &port);
    sockaddr *generilize();
    uint16_t getv();
    uint32_t size();
    ~InetAddress();
};