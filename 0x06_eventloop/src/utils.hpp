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
};
