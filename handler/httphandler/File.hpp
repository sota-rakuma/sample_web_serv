#ifndef FILE_HPP
#define FILE_HPP

#include "HTTPMethodReceiver.hpp"
#include "../../command/Read.hpp"
#include "../../command/Write.hpp"
#include "../../HTTP/HTTPStatusCode.hpp"
#include <string>
#include <stdexcept>
#include <list>
#include <dirent.h>

class File : public HTTPMethodReceiver, public IObserver
{
public:
	class FileError : public std::runtime_error
	{
	public:
		FileError();
		FileError(const std::string &);
		virtual ~FileError() throw();
	};
private:
	int _fd;
	size_t _nb;
	bool _autoindex;
	std::string _index_file;
private:
	int processAutoindex();
	bool createFile();
	bool appendFile();
public:
	File();
	File(
		ISubject *,
		std::list<ICommand *> *,
		AcceptedSocket *,
		bool,
		const std::string &
	);
	File(
		ISubject *,
		std::list<ICommand *> *,
		HTTPMethod *,
		const std::string &,
		AcceptedSocket *,
		bool,
		const std::string &
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
};

#endif /* FILE_HPP */
