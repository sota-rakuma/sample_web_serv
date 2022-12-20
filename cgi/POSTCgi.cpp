#include "POSTCgi.hpp"

void    POSTCgi::exec_cgi_method()
{
    execve("./cgi_scripts/perl_2.cgi", NULL, environ);
}