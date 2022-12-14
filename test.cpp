#include <unistd.h>
#include <sys/types.h>
#include <iostream>
#include <sys/wait.h>
#include <poll.h>
#include <vector>
#include <signal.h>


void childProc()
{
	std::cout << "aaaaaaaaaaaaa" << std::endl;
}

void parentProc(
	std::vector<pollfd> &pollvec
)
{
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
			char buf[256];
			read(pollvec[i].fd, buf, 256);
			std::cout << buf << std::endl;
			pollnum--;
		}
	}
}

void signalHandler(int num)
{
	int stat;
	if (waitpid(num, &stat, WNOHANG) == -1) {
		perror("waitpid");
		std::exit(1);
	}
	if (WIFEXITED(stat) == false) {
		std::cout << "time out" << std::endl;
		kill(num, SIGKILL);
		return ;
	}
	std::cout << "child has already ended" << std::endl;
}

void setSignal()
{
	alarm(1);
	struct sigaction sa;
	memset(&sa, 0, sizeof(struct sigaction));
	sa.__sigaction_u.__sa_handler = signalHandler;
	sigaction(SIGALRM, &sa, NULL);
}

int main(void)
{
	int newin = dup(STDIN_FILENO);
	int newout = dup(STDOUT_FILENO);
	std::vector<pollfd> test;
	test.push_back((pollfd){newin, POLL_IN, 0});
	//test.push_back((pollfd){newout, POLL_OUT, 0});

	int pipefd[] = {newin, newout};
	if (pipe(pipefd)) {
		perror("pipe");
		std::exit(1);
	}

	pid_t pid = fork();
	if (pid != 0)
	{
		close(newout);
		setSignal();
		parentProc(test);
	} else {
		close(newin);
		childProc();
		close(newout);
		std::cout << "end of child" << std::endl;
	}
	return 0;
}