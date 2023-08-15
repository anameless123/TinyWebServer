server:
	g++ server.cpp -pthread src/Acceptor.cpp src/Buffer.cpp src/Channel.cpp \
	src/Connection.cpp src/Epoll.cpp src/EventLoop.cpp src/Server.cpp src/Socket.cpp \
	src/ThreadPool.cpp \
	-o server &&\
	g++ client.cpp src/Buffer.cpp src/Socket.cpp -o client
clean:
	rm server && rm client

threadTest:
	g++ -pthread src/ThreadPool.cpp ThreadPoolTest.cpp -o ThreadPoolTest