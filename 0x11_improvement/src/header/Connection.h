#pragma once
#include <functional>

class Socket;
class EventLoop;
class Channel;
class Buffer;

class Connection {
  private:
    EventLoop *loop;
    Socket *sock;
    Channel *channel;
    std::function<void(Socket *)> deleteConnectionCallback;
    Buffer *readBuffer;

  public:
    Connection(EventLoop *_loop, Socket *_sock);
    ~Connection();

    void setDeleteConnectionCallback(std::function<void(Socket *)> _cb);

    void echo(int sockfd);
    void send(int sockfd);
};