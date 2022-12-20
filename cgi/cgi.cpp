#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <csignal>
#include "CgiMethod.hpp"
#include "GETCgi.hpp"
#include "POSTCgi.hpp"
#include "DELETECgi.hpp"

int child_pid = 0;
int delete_flag = 0;

void    perror_and_exit(std::string str)
{
    std::perror(str.c_str());
    std::exit(1);
}

static void    timer_handler(int pid)
{
    std::cout << "time is over, child will be killed" << std::endl;
    if (kill(child_pid, SIGKILL) == -1)
        perror_and_exit("kill");
}

static void    start_method(CgiMethod *method)
{
    int pipe_fd[2];
    pid_t   pid;
    pid_t   w_pid;
    int status;

    if (std::signal(SIGALRM, timer_handler) == SIG_ERR)
        perror_and_exit("std::signal");
    if (pipe(pipe_fd) == -1)
        perror_and_exit("pipe");
    pid = fork();
    child_pid = pid;
    if (pid < 0)
        perror_and_exit("fork");
    else if (pid == 0)
    {
        if (close(pipe_fd[0]) == -1)
            perror_and_exit("close");
        if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
            perror_and_exit("dup2");
        // if (setenv("PATH_INFO", PATH_INFO_VALUE, 1) == -1)
        //     perror_and_exit("set_env");
        char *QUERY_STRING_VALUE = MY_QUERY_STRING_VALUE;
        // GETの場合はリクエストターゲットから取得、POSTの場合はリクエストボディにクエリストリングが設定されているのでそこから取得
        if (setenv("QUERY_STRING", QUERY_STRING_VALUE, 1) == -1)
            perror_and_exit("set_env");
        // sleep(5); // for debug
        method->exec_cgi_method();
        // 日本語が来た場合文字化けする。
    }
    if (close(pipe_fd[1]) == -1)
        perror_and_exit("close");
    alarm(3);
    // w_pid = waitpid(pid, &status, WNOHANG);
    w_pid = waitpid(pid, &status, 0); // WNOHANGの場合、time_handlerで終了したかどうかの判断が難しい?
    if (WIFEXITED(status) != 1)
    {
        std::cout << "Child process might have been killed." << std::endl;
        std::exit(1);
    }
    char buf[1000];
    if (read(pipe_fd[0], buf, 1000) == -1)
        perror_and_exit("read");
    std::cout << buf << std::endl;
}

static CgiMethod   *get_cgi_method()
{
    if (MY_METHOD == GET)
    {
        CgiMethod   *method = new GETCgi();
        return (method);
    }
    else if (MY_METHOD == POST)
    {
        CgiMethod   *method = new POSTCgi();
        return (method);
    }
    else if (MY_METHOD == DELETE) // 例外としてDELETEのみ子プロセスの処理へ移らない
    {
        CgiMethod   *method = new DELETECgi();
        method->exec_cgi_method();
        delete_flag = 1;
        return (method);
    }
}

void    cgi()
{
    CgiMethod   *method = get_cgi_method();
    if (delete_flag)
        return;
    start_method(method);
}

int main() // for debug
{
    cgi();
    return (0);
}
