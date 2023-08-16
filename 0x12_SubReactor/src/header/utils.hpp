#pragma once
#include "Buffer.h"

#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>

using std::cin;
using std::cout;
using std::endl;

class Utils {
  public:
    static void errif(bool condition, const char *errmsg) {
        if (condition) {
            perror(errmsg);
            exit(EXIT_FAILURE);
        }
    }

    static void keepsend(int fd) {
        Buffer *sendBuffer = new Buffer();
        Buffer *readBuffer = new Buffer();
        while (true) {
            sendBuffer->getline();
            ssize_t wbytes = write(fd, sendBuffer->c_str(), sendBuffer->size());
            if (wbytes == 0)
                continue;
            else if (wbytes == -1) {
                errif(wbytes == -1, "Failed to write, socket already disconnected!");
                break;
            }
            char buf[1024];
            int already_read = 0;
            while (true) {
                memset(buf, 0, sizeof(buf));
                ssize_t read_bytes = read(fd, buf, sizeof(buf));
                if (read_bytes > 0) {
                    readBuffer->append(buf, read_bytes);
                    already_read += read_bytes;
                } else if (read_bytes == 0) {
                    cout << "server disconnected!" << endl;
                    exit(EXIT_FAILURE);
                }
                if (already_read >= sendBuffer->size()) {
                    cout << "message from server: " << readBuffer->c_str() << endl;
                    break;
                }
            }
            readBuffer->clear();
        }
        delete sendBuffer;
        delete readBuffer;
        close(fd);
    }
};
