server:
	g++ client.cpp -o client && \
	g++ server.cpp src/Epoll.cpp src/Channel.cpp src/EventLoop.cpp src/Server.cpp -o server
clean:
	rm server && rm client