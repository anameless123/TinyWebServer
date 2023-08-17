#pragma once
#include "Buffer.h"

#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>

class Utils {
  public:
    static void errif(bool condition, const char *errmsg) {
        if (condition) {
            perror(errmsg);
            exit(EXIT_FAILURE);
        }
    }

    static void Keepsend(int fd) {
        Buffer *send_Buffer = new Buffer();
        Buffer *read_Buffer = new Buffer();
        while (true) {
            send_Buffer->getline();
            ssize_t wbytes = write(fd, send_Buffer->c_str(), send_Buffer->size());
            if (wbytes == 0) {
                continue;
            }
            if (wbytes == -1) {
                errif(wbytes == -1, "Failed to write, socket already disconnected!");
                break;
            }
            char buf[1024];
            int already_read = 0;
            while (true) {
                memset(buf, 0, sizeof(buf));
                ssize_t read_bytes = read(fd, buf, sizeof(buf));
                if (read_bytes > 0) {
                    read_Buffer->append(buf, read_bytes);
                    already_read += read_bytes;
                } else if (read_bytes == 0) {
                    std::cout << "server disconnected!" << std::endl;
                    exit(EXIT_FAILURE);
                }
                if (already_read >= send_Buffer->size()) {
                    std::cout << "message from server: " << read_Buffer->c_str() << std::endl;
                    break;
                }
            }
            read_Buffer->clear();
        }
        delete send_Buffer;
        delete read_Buffer;
        close(fd);
    }
};
