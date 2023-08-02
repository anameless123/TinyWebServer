#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
using namespace std;

int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    // 指针转换方式声明
    sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);
    bind(sockfd, (sockaddr *)&serv_addr, sizeof(serv_addr));
    /*
    // 显式拷贝声明
    sockaddr sock_addr;
    memcpy(&sock_addr.sa_data, &serv_addr.sin_addr, sizeof(serv_addr.sin_addr));
    memcpy(sock_addr.sa_data + sizeof(serv_addr.sin_addr), &serv_addr.sin_port, sizeof(serv_addr.sin_port));
    bind(sockfd, &sock_addr, sizeof(serv_addr));
    */
    listen(sockfd, SOMAXCONN);
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_len = sizeof(clnt_addr);
    bzero(&clnt_addr, sizeof(clnt_addr));
    int clnt_sockfd = accept(sockfd, (sockaddr *)&clnt_addr, &clnt_addr_len);
    cout << "new client fd" << clnt_sockfd << ", IP: "
         << inet_ntoa(clnt_addr.sin_addr) << ", Port: " << ntohs(clnt_addr.sin_port) << endl;
    return 0;
}