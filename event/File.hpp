#ifndef FILE_HPP
#define FILE_HPP

#include "Reciever.hpp"
#include <string>

class File : public Reciever
{
private:
	std::string _buff;
public:
	File();
	File(
		const std::string &
	);
	File(const File &);
	~File();
	EventHandler::Status specialRead(int fd);
	EventHandler::Status specialWrite(int fd);
	const std::string &getBuff() const;
	const std::string &setBuff(
		const std::string &
	);
};

#endif /* FILE_HPP */
