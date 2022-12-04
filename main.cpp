#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include<unistd.h>
#include <string.h>
#include <poll.h>
#include <vector>
#include <fcntl.h>

int initSocket(addrinfo *addr)
{
	addrinfo hints;

	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == -1) {
		perror("socket");
		std::exit(1);
	}
	int flag = fcntl(fd, F_GETFL);
	if (fcntl(fd, F_SETFL, flag | O_NONBLOCK) == -1) {
		perror("fcntl");
		return 1;
	}

	memset(&hints, 0, sizeof(addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
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

	return fd;
}

void communicate(
	std::vector<pollfd> & pollvec,
	size_t index
)
{
	std::cout << "index: " << index << std::endl;
	if (index == 0) {
		addrinfo peer_addr;
		socklen_t len = sizeof(addrinfo);
		int s_fd = accept(pollvec[index].fd, (sockaddr *)&peer_addr, &len);
		if (s_fd == -1) {
			perror("accept");
		}
		pollvec.push_back((pollfd){s_fd, POLLIN, 0});
		std::cout << "connect !" << std::endl;
	} else {
		std::cout << "client" << index << " send" << std::endl;
		char buf[256];
		memset(buf, 0, sizeof(buf));
		if (read(pollvec[index].fd, buf, 256) < 0) {
			perror("read");
		}
		if (strncmp(buf, "EOF", 3) == 0) {
			close(pollvec[index].fd);
			pollvec.erase(pollvec.begin() + index);
			return ;
		}
		std::cout << buf;
		memset(buf, 0, sizeof(buf));
	}
}

int main(void)
{
	addrinfo *addr;
	int fd = initSocket(addr);

	std::vector<pollfd> pollvec;
	pollvec.push_back((pollfd){fd, POLLIN, 0});

	while (1)
	{
		int pollnum = poll(pollvec.data(), pollvec.size(), 60);
		if (pollnum < 0) {
			perror("poll");
			break;
		} else if (pollnum == 0) {
			continue;
		}
		for (size_t i = 0; i < pollvec.size() && pollnum > 0;i++)
		{
			if (pollvec[i].revents ^ POLL_IN) {
				if (pollvec[i].revents & POLL_ERR ||
					pollvec[i].revents & POLL_HUP) {
					std::cerr << "クライアントが閉じてる" << std::endl;
				}
				continue;
			}
			communicate(pollvec, i);
			pollnum--;
		}
	}

	freeaddrinfo(addr);
	close(fd);
	std::cout << "connection closed" << std::endl;
	return 0;
}