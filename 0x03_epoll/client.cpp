#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include "../utils.hpp"

using namespace std;

int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);

    errif(connect(sockfd, (sockaddr *)&serv_addr, sizeof(serv_addr)) == -1, "Failed to connect !");
    char buf[1024];
    cout << "connect succeed!" << endl;

    while (true)
    {
        memset(buf, 0, sizeof(buf));
        cin >> buf;
        ssize_t wbytes = write(sockfd, buf, sizeof(buf));
        if (wbytes == -1)
        {
            errif(wbytes == -1, "Failed to write, socket already disconnected!");
            break;
        }
        ssize_t read_bytes = read(sockfd, buf, sizeof(buf));

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
            close(sockfd);
            errif(true, "socket read error");
        }
    }

    return 0;
}