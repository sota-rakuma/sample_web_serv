#ifndef FILE_HPP
#define FILE_HPP

#include "HTTPMethodReciever.hpp"
#include "../subject/AcceptedSocket.hpp"
#include "../command/Get.hpp"
#include "../command/Post.hpp"
#include "../command/Delete.hpp"
#include <string>
#include <stdexcept>
#include <list>

#ifndef BUFSIZE
#define BUFSIZE 1024
#endif

class File : public HTTPMethodReciever, public ISubject
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
	Get *_get;
	Post *_post;
	Delete *_delete;
	std::list<AcceptedSocket *> _as;
public:
	File();
	File(
		const std::string &,
		int oflag
	);
	File(
		const std::string &,
		int oflag,
		int mode
	);
	File(const File &);
	virtual ~File();
	virtual int httpGet();
	virtual int httpPost();
	virtual int httpDelete();
	virtual void notify(int, int, ISubject *);
};

#endif /* FILE_HPP */
