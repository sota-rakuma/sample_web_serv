#ifndef CGIMETHOD_HPP
#define CGIMETHOD_HPP

#include <string>
#include <unistd.h>
#include <iostream>

#define MY_CGI_PERL "/Users/mhida/42/lv5/webserv_pra/cgi/cgi_scripts/perl.cgi" //
#define MY_CGI_PERL_2 "/Users/mhida/42/lv5/webserv_pra/cgi/cgi_scripts/perl_2.cgi" //
#define MY_QUERY_STRING_VALUE "value=post_value&value_2=post_value_2" //
#define GET 10
#define POST 11
#define DELETE 12
#define MY_METHOD POST

extern char **environ;

class CgiMethod
{
    public:
        virtual ~CgiMethod() {};
        virtual void exec_cgi_method() = 0;
};

void    perror_and_exit(std::string str);
void    cgi();


#endif