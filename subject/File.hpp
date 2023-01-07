#ifndef FILE_HPP
#define FILE_HPP


#include "EventHandler.hpp"
#include "../command/Read.hpp"
#include "../command/Write.hpp"
#include <string>
#include <stdexcept>

class File : public EventHandler
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
	std::string _path;
	Read *_read;
	Write *_write;
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
	virtual ICommand *getHandler(int) const;
};

#endif /* FILE_HPP */
