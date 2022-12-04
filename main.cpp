#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include<unistd.h>
#include <string.h>

int main(void)
{
	addrinfo hints, *addr;

	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == -1) {
		perror("socket");
		std::exit(1);
	}

	memset(&hints, 0, sizeof(addrinfo));
	hints.ai_family = AF_INET; // IPv4
	hints.ai_socktype = SOCK_STREAM; // TCP
	if (getaddrinfo("127.0.0.1", "8080", &hints, &addr) != 0) {
		perror("getaddrinfo");
		std::exit(1);
	}

	if (bind(fd, addr->ai_addr, addr->ai_addrlen) == -1) {
		perror("bind");
		std::exit(1);
	}

	if (listen(fd, 50) == -1) {
		perror("listen");
		std::exit(1);
	}

	addrinfo peer_addr;
	socklen_t len = sizeof(addrinfo);
	int s_fd = accept(fd, (sockaddr *)&peer_addr, &len);
	if (s_fd == -1) {
		perror("accept");
		std::exit(1);
	}
	std::cout << "connect !" << std::endl;

	char buf[256];
	memset(buf, 0, sizeof(buf));
	while (read(s_fd, buf, 256) > 0) {
		if (strncmp(buf, "EOF", 3) == 0) {
			break ;
		}
		std::cout << buf;
		memset(buf, 0, sizeof(buf));
	}

	freeaddrinfo(addr);
	close(fd);
	std::cout << "connection closed" << std::endl;
	return 0;
}

//#include <poll.h>
//#include <vector>

//int main(void)
//{
//	addrinfo hints, *addr;

//	int fd = socket(AF_INET, SOCK_STREAM, 0);
//	if (fd == -1) {
//		perror("socket");
//		std::exit(1);
//	}

//	memset(&hints, 0, sizeof(addrinfo));
//	hints.ai_family = AF_INET;
//	hints.ai_socktype = SOCK_STREAM;
//	if (getaddrinfo("127.0.0.1", "8080", &hints, &addr) != 0) {
//		perror("getaddrinfo");
//		std::exit(1);
//	}

//	if (bind(fd, addr->ai_addr, addr->ai_addrlen) == -1) {
//		perror("bind");
//		std::exit(1);
//	}

//	std::vector<pollfd> pollvec;

//	if (listen(fd, 50) == -1) {
//		perror("listen");
//		std::exit(1);
//	}

//	pollvec.push_back((pollfd){fd, POLL_IN, 0});

//	while (1)
//	{
//		int pollnum = poll(pollvec.data(), pollvec.size(), 60);
//		if (pollnum < 0)
//		{
//			perror("poll");
//			break;
//		}
//		for (size_t i = 0; i < pollnum;)
//		{

//		}
//	}

//	addrinfo peer_addr;
//	socklen_t len = sizeof(addrinfo);
//	int s_fd = accept(fd, (sockaddr *)&peer_addr, &len);
//	if (s_fd == -1) {
//		perror("accept");
//		std::exit(1);
//	}
//	std::cout << "connect !" << std::endl;

//	char buf[256];
//	memset(buf, 0, sizeof(buf));
//	while (read(s_fd, buf, 256) > 0) {
//		if (strncmp(buf, "EOF", 3) == 0) {
//			break ;
//		}
//		std::cout << buf;
//		memset(buf, 0, sizeof(buf));
//	}

//	freeaddrinfo(addr);
//	close(fd);
//	std::cout << "connection closed" << std::endl;
//	return 0;
//}