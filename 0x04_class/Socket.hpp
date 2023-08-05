#include <string>
#include "InetAddress.hpp"

using std::string;
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
