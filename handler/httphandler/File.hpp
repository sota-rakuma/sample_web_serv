#ifndef FILE_HPP
#define FILE_HPP

#include "HTTPMethodReceiver.hpp"
#include "../AcceptedSocket.hpp"
#include "../../command/Read.hpp"
#include "../../command/Write.hpp"
#include <string>
#include <stdexcept>
#include <list>

#ifndef BUFSIZE
#define BUFSIZE 1024
#endif

class File : public HTTPMethodReceiver, public IObserver
{
public:
	class FileError : public std::runtime_error
	{
	public:
		FileError();
		FileError(const std::string &);
		~FileError();
	};
private:
	int _fd;
	bool _is_exist;
	std::string _path;
	std::string _buff;
	Read *_read;
	Write *_write;
	std::list<AcceptedSocket *> _as;
public:
	File();
	File(
		ISubject *,
		std::list<ICommand *> *,
		const std::string &,
		int oflag
	);
	File(
		ISubject *,
		std::list<ICommand *> *,
		const std::string &,
		int oflag,
		int mode
	);
	File(const File &);
	virtual ~File();
	virtual void update(int);
	virtual int read();
	virtual int write();
	virtual int httpGet();
	virtual int httpPost();
	virtual int httpDelete();
	int getFd() const;
	const std::string & getPath() const;
};

#endif /* FILE_HPP */
