#include "Socket.h"

Socket::Socket() {
    addr = InetAddress(-1, "0.0.0.0", 1);
    sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Failed to initialize socket!");
        exit(EXIT_FAILURE);
    } else
        cout << "socket initialize succeed!" << endl;
}
Socket::Socket(int fd)
    : sockfd(fd) {
    Utils::errif(fd == -1, "Failed to copy construct Socket by fd");
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
    } else
        cout << "Server bind at: " << inet_ntoa(addr.siaddr.sin_addr)
             << ":" << ntohs(addr.siaddr.sin_port) << endl;
}

void Socket::connect(InetAddress _addr) {
    addr = _addr;
    if (::connect(sockfd, addr.generilize(), addr.size()) == -1) {
        perror("Failed to connect!");
        exit(EXIT_FAILURE);
    } else
        cout << "Connect to: " << inet_ntoa(addr.siaddr.sin_addr)
             << ":" << ntohs(addr.siaddr.sin_port) << endl;
}

void Socket::listen() {
    if (::listen(sockfd, MaxCon) == -1) {
        perror("Failed to listen!");
        exit(EXIT_FAILURE);
    } else
        cout << "listening..." << endl;
}

int Socket::accept(InetAddress *clntaddr) {
    socklen_t size = clntaddr->size();
    int res = ::accept(sockfd, clntaddr->generilize(), &size);
    if (size != clntaddr->size()) {
        std::cout << "Failed to accept, address is cutdown" << std::endl;
        res = -1;
    }
    if (res != -1)
        cout << "Client accepted in Fd: " << res << ", " << inet_ntoa(clntaddr->siaddr.sin_addr)
             << ":" << ntohs(clntaddr->siaddr.sin_port) << endl;
    return res;
}

int Socket::getFd() {
    return sockfd;
}

void Socket::setnonblocking() {
    fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFL, 0) | O_NONBLOCK);
}