#include "Socket.h"
#include "utils.hpp"

using std::cout;
using std::endl;

Socket::Socket() {
  addr = InetAddress(-1, "0.0.0.0", 1);
  sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    perror("Failed to initialize socket!");
    exit(EXIT_FAILURE);
  } else {
    cout << "socket initialize succeed!" << endl;
  }
}
Socket::Socket(int fd, InetAddress _addr) : sockfd(fd) {
  addr = _addr;
  Utils::errif(fd == -1, "Failed to construct Socket by fd");
}

Socket::~Socket() {
  if (sockfd != -1) {
    close(sockfd);
    sockfd = -1;
  }
}
void Socket::bind(InetAddress _addr) {
  addr = _addr;
  if (::bind(sockfd, addr.generilize(), addr.size()) == -1) {
    perror("Failed to bind!");
    exit(EXIT_FAILURE);
  } else {
    cout << "Server bind at: " << addr.getIP() << ":" << addr.getPort() << endl;
  }
}

void Socket::connect(InetAddress _addr) {
  addr = _addr;
  if (::connect(sockfd, addr.generilize(), addr.size()) == -1) {
    perror("Failed to connect!");
    exit(EXIT_FAILURE);
  } else {
    cout << "Connect to: " << addr.getIP() << ":" << addr.getPort() << endl;
  }
}

void Socket::listen() {
  if (::listen(sockfd, MaxCon) == -1) {
    perror("Failed to listen!");
    exit(EXIT_FAILURE);
  } else {
    cout << "listening..." << endl;
  }
}

int Socket::accept(InetAddress *clntaddr) {
  socklen_t size = clntaddr->size();
  int res = ::accept(sockfd, clntaddr->generilize(), &size);
  if (size != clntaddr->size()) {
    std::cout << "Failed to accept, address is cutdown" << std::endl;
    res = -1;
  }
  if (res != -1) cout << "Client: " << clntaddr->getIP() << ":" << clntaddr->getPort() << " accepted" << endl;
  return res;
}

void Socket::setnonblocking() { fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFL, 0) | O_NONBLOCK); }

int Socket::getFd() { return sockfd; }

void Socket::printIP() { cout << addr.getIP(); }

void Socket::printPort() { cout << addr.getPort(); }
