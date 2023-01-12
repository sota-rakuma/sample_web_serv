#ifndef ACCEPTEDSOCKET_HPP
#define ACCEPTEDSOCKET_HPP

#include "../parser/Context.hpp"
#include "../HTTP/HTTPRequest.hpp"
#include "../HTTP/HTTPResponse.hpp"
#include "../handler/EventHandler.hpp"
#include "../handler/HTTPMethodReceiver.hpp"
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
	RECEIVE_REQUEST_BODY,
	EXECUTE_METHOD,
	SEND_STATUS_LINE,
	SEND_RESPONSE_HEADER,
	SEND_RESPONSE_BODY,
};

class AcceptedSocket : public EventHandler
{
private:
	int _sockfd;
	//class DataStore {
	//private:
	//	int _tmpfd;
	//	size_t len;
	//	std::string _buff;
	//public:
	//	DataStore();
	//	DataStore(const DataStore &);
	//	~DataStore();
	//	int getTmpFd() const;
	//	size_t getDataLength() const;
	//	const std::string & getBuff() const;
	//	DataStore & addBuff(const std::string &);
	//	DataStore & addBuff(const char *);
	//}	_datastore;
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
public:
	AcceptedSocket();
	AcceptedSocket(
		IObserver *,
		int,
		const sockaddr_in &,
		ServerConfigFinder *
	);
	AcceptedSocket(const AcceptedSocket &);
	virtual ~AcceptedSocket();
	virtual void notify(int, int, EventHandler *);
	virtual ICommand *getHandler(int) const;
	AcceptedSocket &setInfo(const sockaddr_in &);
	AcceptedSocket &setFd(int);
	int receive();
	int send();
};

#endif /* ACCEPTEDSOCKET_HPP */
