#ifndef ACCEPTEDSOCKET_HPP
#define ACCEPTEDSOCKET_HPP

#include "IOEventHandler.hpp"
#include "httphandler/CGI.hpp"
#include "httphandler/File.hpp"
#include "../parser/Context.hpp"
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

class AcceptedSocket : public IOEventHandler, public IObserver
{
private:
	int _sockfd;
	size_t _chunk_size;
	std::string _buff;
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
	void processRequest();
	void processRequestLine(size_t);
	void processRequestHeader();
	void processRequestBody();
	void processChunkedBody();
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
	virtual ICommand *getHandler(int) const;
	AcceptedSocket &setInfo(const sockaddr_in &);
	AcceptedSocket &setFd(int);
	virtual void update(int);
	virtual int read();
	virtual int write();
};

#endif /* ACCEPTEDSOCKET_HPP */
