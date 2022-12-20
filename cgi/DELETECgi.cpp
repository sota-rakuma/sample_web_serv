#include "DELETECgi.hpp"
#include "CgiMethod.hpp"

void    DELETECgi::exec_cgi_method()
{
    if (access(MY_CGI_PERL, F_OK) == -1)
        perror_and_exit("access"); // ステータスコードどうする？？
    if (access(MY_CGI_PERL, W_OK) == -1)
        perror_and_exit("access"); // ステータスコードどうする？？
    if (unlink(MY_CGI_PERL) == -1)
        perror_and_exit("unlink");
    // ここでステータスコード200設定する
}