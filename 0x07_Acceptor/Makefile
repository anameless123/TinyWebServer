server:
	g++ server.cpp src/Socket.cpp src/Acceptor.cpp src/Channel.cpp src/Epoll.cpp src/EventLoop.cpp src/Server.cpp -o server &&\
	g++ client.cpp src/Socket.cpp -o client
clean:
	rm server && rm client