#ifndef CGI_HPP
#define CGI_HPP

#include "EventHandler.hpp"
#include "../command/Read.hpp"
#include "../command/Write.hpp"
#include <string>

class CGI : public EventHandler
{
private:
	int _in_fd;
	int _out_fd;
	std::string _path;
	Read *_read;
	Write *_write;
public:
	CGI();
	CGI(
		const std::string &
	);
	CGI(const CGI &);
	virtual ~CGI();
	virtual void notify(int, int, EventHandler *);
	virtual ICommand *getHandler(int) const;
};

#endif /* CGI_HPP */
