#ifndef ACCEPTEDSOCKET_HPP
#define ACCEPTEDSOCKET_HPP

#include "IOEventHandler.hpp"
#include "../parser/Context.hpp"
#include "httphandler/HTTPMethodReceiver.hpp"
#include "../HTTP/HTTPRequest.hpp"
#include "../HTTP/HTTPResponse.hpp"
#include "../command/Read.hpp"
#include "../command/Write.hpp"
#include "../config/ServerConfigFinder.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#ifndef BUFFSIZE
#define BUFFSIZE 8196
#endif

enum Progress
{
	RECEIVE_REQUEST_LINE,
	RECEIVE_REQUEST_HEADER,
	RECEIVE_CHUNKED_SIZE,
	RECEIVE_REQUEST_BODY,
	EXECUTE_METHOD,
	SEND_STATUS_LINE,
	SEND_RESPONSE_HEADER,
	SEND_RESPONSE_BODY,
};

class File;
class CGI;

class AcceptedSocket : public IOEventHandler, public IObserver
{
private:
	int _sockfd;
	size_t _body_size;
	std::string _buff;
	size_t _nb;
	sockaddr_in _info;
	ServerConfigFinder *_configfinder;
	ServerConfig _config;
	HTTPStatus _status;
	Progress _progress;
	HTTPRequest _req;
	HTTPResponse _res;
	Read *_read;
	Write *_write;
	Context _parser_ctx;
	HTTPMethodReceiver *_receiver;
	// finderではなく、header解析時にconfigをセットしてしまっても構わない
	// error-pageの内容は持っていてもいいのか？
private:
	void processRequest(const std::string &);
	void processRequestLine(
		const std::string &
	);
	void processRequestHeader(
		const std::string &
	);
	void processRequestBody(
		const std::string &
	);
	void processChunkedBody(
		const std::string &
	);
public:
	AcceptedSocket();
	AcceptedSocket(
		ISubject *,
		std::list<ICommand *> *,
		int,
		const sockaddr_in &,
		ServerConfigFinder *
	);
	AcceptedSocket(const AcceptedSocket &);
	virtual ~AcceptedSocket();
	AcceptedSocket &setInfo(const sockaddr_in &);
	AcceptedSocket &setFd(int);
	virtual void update(int);
	virtual int read();
	virtual int write();
	void processCGIResponse(const std::string &);
	void createResponse(const std::string &);
	void processTest();
};

#endif /* ACCEPTEDSOCKET_HPP */
