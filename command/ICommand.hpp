#ifndef ICOMMAND_HPP
#define ICOMMAND_HPP

#include <cstddef>

class ICommand
{
public:
	ICommand();
	virtual ~ICommand();
	virtual int execute() = 0;
};

#endif /* ICOMMAND_HPP */
