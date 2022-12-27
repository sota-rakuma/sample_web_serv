#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "SocketInfo.hpp"

/**
 * @brief
 * acceptの後のコネクション確立後にオブジェクトが作られる
 */
class Connection : public SocketInfo
{
private:
	std::string _rbuff;
	std::string _wbuff;
public:
	Connection();
	Connection(const SocketInfo &);
	Connection(int, const sockaddr_in &);
	Connection(int, const sockaddr_in &, const std::string &);
	Connection(int, const sockaddr_in &, const std::string &, const std::string &);
	Connection(const Connection&);
	~Connection();
	const std::string &getRbuffer() const;
	const std::string &getWbuffer() const;
	int getRequest();
	int createResponse();
	int sendResponse();
};

#endif /* CONNECTION_HPP */
