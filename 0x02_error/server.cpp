#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include "utils.hpp"
using namespace std;

int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    Utils::errif(sockfd == -1, "Failed to create socket");
    // 指针转换方式声明
    sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);
    Utils::errif(bind(sockfd, (sockaddr *)&serv_addr, sizeof(serv_addr)) == -1, "socket bind error");
    /*
    // 显式拷贝声明
    sockaddr sock_addr;
    memcpy(&sock_addr.sa_data, &serv_addr.sin_addr, sizeof(serv_addr.sin_addr));
    memcpy(sock_addr.sa_data + sizeof(serv_addr.sin_addr), &serv_addr.sin_port, sizeof(serv_addr.sin_port));
    bind(sockfd, &sock_addr, sizeof(serv_addr));
    */
    Utils::errif(listen(sockfd, SOMAXCONN) == -1, "socket listen error");

    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_len = sizeof(clnt_addr);
    bzero(&clnt_addr, sizeof(clnt_addr));

    int clnt_sockfd = accept(sockfd, (sockaddr *)&clnt_addr, &clnt_addr_len);
    Utils::errif(clnt_sockfd == -1, "socket accept error");
    cout << "new client fd" << clnt_sockfd << ", IP: "
         << inet_ntoa(clnt_addr.sin_addr) << ", Port: " << ntohs(clnt_addr.sin_port) << endl;

    char buf[1024];
    while (true)
    {
        memset(buf, 0, sizeof(buf));
        ssize_t read_bytes = read(clnt_sockfd, buf, sizeof(buf));
        if (read_bytes > 0)
        {
            cout << "message from clinet fd: " << clnt_sockfd << ": " << buf << endl;
            write(clnt_sockfd, buf, sizeof(buf));
        }
        else if (read_bytes == 0)
        {
            cout << "client fd: " << clnt_sockfd << " disconnected" << endl;
            close(clnt_sockfd);
            break;
        }
        else if (read_bytes == -1)
        {
            close(clnt_sockfd);
            Utils::errif(true, "socket read error");
        }
    }

    return 0;
}