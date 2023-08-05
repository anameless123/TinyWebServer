server:
	g++ client.cpp Socket.cpp -o client && \
	g++ server.cpp Epoll.cpp InetAddress.cpp Socket.cpp -o server
clean:
	rm server && rm client