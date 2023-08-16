#include "src/header/Buffer.h"
#include "src/header/InetAddress.hpp"
#include "src/header/Socket.h"
#include "src/header/ThreadPool.h"
#include "src/header/utils.hpp"
#include <cstring>
#include <functional>
#include <iostream>
#include <unistd.h>

using namespace std;

void oneClient(int msgs, int wait) {
    Socket *sock = new Socket;
    InetAddress addr(AF_INET, "127.0.0.1", 1234);
    sock->connect(addr);

    int sockfd = sock->getFd();

    Buffer *sendBuffer = new Buffer;
    Buffer *readBuffer = new Buffer;

    sleep(wait);
    int count = 0;
    while (count < msgs) {
        sendBuffer->setBuf("Im client!");
        ssize_t write_bytes = write(sockfd, sendBuffer->c_str(), sendBuffer->size());
        if (write_bytes == -1) {
            printf("socket already disconnected!");
            break;
        }
        int already_read = 0;
        char buf[1024];
        while (true) {
            memset(buf, 0, sizeof(buf));
            ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
            if (read_bytes > 0) {
                readBuffer->append(buf, read_bytes);
                already_read += read_bytes;
            } else if (read_bytes == 0) {
                cout << "server disconnected!" << endl;
                exit(EXIT_SUCCESS);
            }
            if (already_read >= sendBuffer->size()) {
                cout << "count: " << count++ << "message from server" << readBuffer->c_str() << endl;
                break;
            }
        }
        readBuffer->clear();
    }
    delete sock;
}

int main(int argc, char *argv[]) {
    int threads = 100;
    int msgs = 100;
    int wait = 0;
    int o;
    const char *optstring = "t:m:w:";
    while ((o = getopt(argc, argv, optstring)) != -1) {
        switch (o) {
        case 't':
            threads = stoi(optarg);
            cout << "threads = " << threads << endl;
            break;
        case 'm':
            msgs = stoi(optarg);
            cout << "msgs = " << msgs << endl;
            break;
        case 'w':
            if (optarg) wait = stoi(optarg);
            cout << "wait = " << wait << endl;
            break;
        case '?':
            cout << "error optopt: " << optopt << endl;
            cout << "error opterr: " << opterr << endl;
            exit(EXIT_FAILURE);
        }
    }

    ThreadPool *pool = new ThreadPool;
    std::function<void()> func = std::bind(oneClient, msgs, wait);
    for (int i = 0; i < threads; ++i) {
        pool->add(func);
    }
    delete pool;
    return 0;
}