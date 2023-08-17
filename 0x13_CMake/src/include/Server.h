#pragma once
#include <map>
#include <vector>

class EventLoop;
class Acceptor;
class Connection;
class Socket;
class ThreadPool;

class Server {
 private:
  EventLoop *mainReactor;
  Acceptor *acceptor;
  std::map<int, Connection *> connections;
  std::vector<EventLoop *> subReactors;
  ThreadPool *thpool;

 public:
  Server();
  explicit Server(EventLoop *);
  ~Server();

  void handleReadEvent(int);
  void newConnection(Socket *serv_sock);
  void deleteConnection(int sockfd);
};
