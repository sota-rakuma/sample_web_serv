#ifndef POSTCGI_HPP
#define POSTCGI_HPP

#include "CgiMethod.hpp"

class POSTCgi : public CgiMethod
{
    public:
        ~POSTCgi() {delete this;};
        void exec_cgi_method();
};

#endif