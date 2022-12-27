#ifndef GETCGI_HPP
#define GETCGI_HPP

#include "CgiMethod.hpp"

class GETCgi : public CgiMethod
{
    public:
        ~GETCgi() {delete this;};
        void exec_cgi_method();
};

#endif