#ifndef CGI_HPP
#define CGI_HPP

#include "HTTPMethodReciever.hpp"
#include "../subject/AcceptedSocket.hpp"
#include "../command/Get.hpp"
#include "../command/Post.hpp"
#include "../command/Delete.hpp"
#include <string>
#include <stdexcept>

#define GET "GET" // デバッグ用
#define POST "POST" // デバッグ用
#define DELETE "DELETE" // デバッグ用

class CGI : public HTTPMethodReciever, public ISubject
{
private:
	int _in_fd;
	int _out_fd;
	std::string _buff;
	size_t _nb;
	std::string _path;

	bool _is_exist;
	Get *_get;
	Post *_post;
	Delete *_delete;
public:
	CGI();
	CGI(
		const std::string &
	);
	CGI(const CGI &);
	virtual ~CGI();
	virtual int httpGet();
	virtual int httpPost();
	virtual int httpDelete();
	virtual void notify(int, int, ISubject *);
	virtual ICommand *getHandler(int) const;
	// void executeCGI(int, int); //
	void setMetaVariables(std::string);
	void executeCGI(std::string);
};

#endif /* CGI_HPP */
