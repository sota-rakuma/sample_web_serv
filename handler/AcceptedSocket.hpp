#ifndef ACCEPTEDSOCKET_HPP
#define ACCEPTEDSOCKET_HPP

#include "IOEventHandler.hpp"
#include "../parser/Context.hpp"
#include "../parser/HTTPRequestParser.hpp"
#include "../config/ServerConfig.hpp"
#include "../HTTP/HTTPRequest.hpp"
#include "../HTTP/HTTPResponse.hpp"
#include "../command/Read.hpp"
#include "../command/Write.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#ifndef BUFFSIZE
#define BUFFSIZE 8192
#endif

enum Progress
{
	RECEIVE_REQUEST_LINE,
	RECEIVE_REQUEST_HEADER,
	RECEIVE_CHUNKED_SIZE,
	RECEIVE_CHUNKED_BODY,
	RECEIVE_REQUEST_BODY,
	EXECUTE_METHOD,
	CREATE_RESPONSE,
	SEND_STATUS_LINE,
	SEND_RESPONSE_HEADER,
	SEND_RESPONSE_BODY,
	ERROR,
	END,
};

class HTTPMethodReceiver;

class AcceptedSocket : public IOEventHandler, public IObserver
{
private:
	int _sockfd;
	size_t _body_size;
	std::string _buff;
	sockaddr_in _info;
	std::map<std::string, ServerConfig> * _confs;
	ServerConfig _config;
	ServerConfig::Location _location;
	HTTPStatus _status;
	Progress _progress;
	HTTPRequest _req;
	HTTPResponse _res;
	Context _parser_ctx;
	HTTPMethodReceiver *_receiver;
	bool _is_cgi;
private:
	void processRequest(const std::string &);
	size_t processRequestLine(
		const std::string &
	);
	size_t processRequestHeader(
		const std::string &,
		size_t
	);
	size_t processRequestBody(
		const std::string &,
		size_t
	);
	size_t processChunkedBody(
		const std::string &,
		size_t
	);
	int validateRequest();
	int processObsFold();
	bool prepareEvent();
	bool checMethodAllowed(
		const std::string &
	);
	bool preparePostEvent(
		std::string &
	);
	void setMethodReceiver();
	bool setHTTPMethod(
		const std::string &,
		std::string &
	);
	void translatePath(
		std::string &,
		const std::string &
	);
	bool isCGI() const;
	bool prepareCGI();
	void createGeneralHeader();
	void createRedirectResponse(
		HTTPStatus
	);
	void createResponseTemplate();
	void processResponse();
	void internalRedirect(
		const std::string &,
		std::string &,
		const std::string &
	);
	void internalRedirect(
		const std::string &,
		std::string &
	);
	int processCGIResponse();
public:
	AcceptedSocket();
	AcceptedSocket(
		ISubject *,
		std::list<ICommand *> *,
		int,
		const sockaddr_in &,
		std::map<std::string, ServerConfig> *
	);
	AcceptedSocket(const AcceptedSocket &);
	virtual ~AcceptedSocket();
	AcceptedSocket &setInfo(const sockaddr_in &);
	AcceptedSocket &setFd(int);
	AcceptedSocket &setStatus(HTTPStatus);
	virtual void update(int);
	virtual int read();
	virtual int write();
	void createErrorResponse(
		HTTPStatus
	);
	void createNormalResponse();
};

#endif /* ACCEPTEDSOCKET_HPP */
