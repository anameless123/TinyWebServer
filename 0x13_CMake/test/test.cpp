#include <unistd.h>
#include <cstring>
#include <functional>
#include <iostream>
#include "Buffer.h"
#include "InetAddress.hpp"
#include "Socket.h"
#include "ThreadPool.h"
#include "utils.hpp"

using std::cout;
using std::endl;
using std::stoi;

void OneClient(int msgs, int wait) {
  Socket *sock = new Socket;
  InetAddress addr(AF_INET, "127.0.0.1", 1234);
  sock->connect(addr);

  int sockfd = sock->getFd();

  Buffer *send_Buffer = new Buffer;
  Buffer *read_Buffer = new Buffer;

  sleep(wait);
  int count = 0;
  while (count < msgs) {
    send_Buffer->setBuf("Im client!");
    ssize_t write_bytes = write(sockfd, send_Buffer->c_str(), send_Buffer->size());
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
        read_Buffer->append(buf, read_bytes);
        already_read += read_bytes;
      } else if (read_bytes == 0) {
        cout << "server disconnected!" << endl;
        exit(EXIT_SUCCESS);
      }
      if (already_read >= send_Buffer->size()) {
        cout << "count: " << count++ << "message from server" << read_Buffer->c_str() << endl;
        break;
      }
    }
    read_Buffer->clear();
  }
  delete sock;
  delete send_Buffer;
  delete read_Buffer;
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
  std::function<void()> func = std::bind(OneClient, msgs, wait);
  for (int i = 0; i < threads; ++i) {
    pool->add(func);
  }
  delete pool;
  return 0;
}
