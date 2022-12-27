#include "GETCgi.hpp"
// #include "CgiMethod.hpp"

void    GETCgi::exec_cgi_method()
{
    // char path_c[1000];
    // if (getcwd(path_c, 1000) == NULL)
    //  perror_and_exit("getcwd");
    // std::string path = path_c;
    // std::cout << path << std::endl;
    // std::string file_path = path + "/cgi_scripts/perl.cgi";
    // char *a = "/Users/mhida/42/lv5/webserv_pra/cgi/cgi_scripts/perl.cgi";
    // char *args[2];
    // args[0] = (char *)malloc(100);
    // args[1] = NULL;
    // strcpy(args[0], file_path.c_str());

    // std::cout << "check" << std::endl;
    execve("./cgi_scripts/perl.cgi", NULL, environ);
    // std::exit(0); // for debug
}