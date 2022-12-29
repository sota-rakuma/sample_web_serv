#ifndef EVENTHANDLER_HPP
#define EVENTHANDLER_HPP

#include <cstddef>
#include <stdexcept>

class EventHandler
{
public:
	enum Status {
		ERROR,
		SUCCESS,
	};
private:
	int _fd;
public:
	EventHandler();
	EventHandler(int fd);
	virtual ~EventHandler();
	virtual Status execute() = 0;
	int getFd() const;
	void setFd(int fd);
};


#endif /* EVENTHANDLER_HPP */
