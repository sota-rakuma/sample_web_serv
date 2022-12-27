#ifndef DELETECGI_HPP
#define DELETECGI_HPP

#include "CgiMethod.hpp"

class DELETECgi : public CgiMethod
{
    public:
        ~DELETECgi() {delete this;};
        void exec_cgi_method();
};

#endif