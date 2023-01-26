#include "AcceptedSocket.hpp"
#include "httphandler/HTTPMethodReceiver.hpp"
#include "httphandler/CGI.hpp"
#include "httphandler/File.hpp"
#include "../command/Get.hpp"
#include "../command/Post.hpp"
#include "../command/Delete.hpp"
#include <unistd.h>
#include <sstream>
#include <fcntl.h>
#include <arpa/inet.h>
#include <cstdio>
#include <ctime>

AcceptedSocket::AcceptedSocket()
:IOEventHandler()
{
}

AcceptedSocket::AcceptedSocket(
	ISubject *subject,
	std::list<ICommand *> *commands,
	int sockfd,
	const sockaddr_in &info,
	ServerConfigFinder *configfinder
)
:IOEventHandler(subject, commands),
_sockfd(sockfd),
_body_size(0),
_buff(""),
_nb(0),
_info(info),
_configfinder(configfinder),
_config(),
_progress(RECEIVE_REQUEST_LINE),
_receiver(static_cast<HTTPMethodReceiver *>(NULL))
{
	_parser_ctx = new HTTPRequestParser(&this->_req);
	getSubject()->subscribe(_sockfd, POLLIN, this);
}

AcceptedSocket::AcceptedSocket(const AcceptedSocket &another)
:IOEventHandler(another.getSubject(), another.getCommandList()),
_sockfd(another._sockfd),
_body_size(0),
_buff(""),
_nb(0),
_configfinder(another._configfinder),
_config(another._config),
_parser_ctx(another._parser_ctx),
_info(another._info),
_progress(another._progress),
_receiver(static_cast<HTTPMethodReceiver *>(NULL))
{
	getSubject()->subscribe(_sockfd, POLLIN, this);
}

AcceptedSocket::~AcceptedSocket()
{
	delete _receiver;
	::close(_sockfd);
}

AcceptedSocket &AcceptedSocket::setInfo(const sockaddr_in & info)
{
	_info = info;
	return *this;
}

AcceptedSocket &AcceptedSocket::setFd(int sockfd)
{
	_sockfd = sockfd;
	return *this;
}

AcceptedSocket &AcceptedSocket::setStatus(HTTPStatus status)
{
	_status = status;
	return *this;
}

void AcceptedSocket::update(int event)
{
	if (event & (POLLERR | POLLHUP | POLLNVAL)) {
		getSubject()->unsubscribe(_sockfd, false);
		return ;
	}

	if (event & POLLOUT) {
		getCommandList()->push_back(getWriteCommand());
	}
	else if (event & POLLIN) {
		getCommandList()->push_back(getReadCommand());
	}
}

int AcceptedSocket::read()
{
	char buff[BUFFSIZE];

	ssize_t nb = ::recv(_sockfd, buff, BUFFSIZE, 0);
	if (nb == -1) {
		getSubject()->unsubscribe(_sockfd, false);
		return -1;
	} else if (nb == 0) {
		// ソケットストリームが正しく閉じられた場合
		getSubject()->unsubscribe(_sockfd, false);
		return 0;
	}
	buff[nb] = '\0';
	processRequest(buff);
	//processTest();
	return 1;
}

void AcceptedSocket::processRequest(const std::string & raw)
{
	switch (_progress)
	{
	case RECEIVE_REQUEST_LINE:
		processRequestLine(raw);
	case RECEIVE_REQUEST_HEADER:
		processRequestHeader(raw);
	case RECEIVE_CHUNKED_SIZE:
		processChunkedBody(raw);
	case RECEIVE_CHUNKED_BODY:
		processChunkedBody(raw);
	case RECEIVE_REQUEST_BODY:
		processRequestBody(raw);
	case CREATE_RESPONSE:
		createResponse();
	default:
		break;
	}
}

void AcceptedSocket::processRequestLine(
	const std::string &raw
)
{
	size_t index = raw.find("\r\n");
	if (index == std::string::npos) {
		_buff += raw;
		return ;
	}
	_buff += raw.substr(0, index);
	if (_parser_ctx.execParse(_buff) == 0) {
		_buff = raw.substr(index + 2);
		_progress = RECEIVE_REQUEST_HEADER;
		return ;
	}
	_status = BAD_REQUEST;
	// エラーレスポンス
	_progress = CREATE_RESPONSE;
}

void AcceptedSocket::processRequestHeader(
	const std::string & raw
)
{
	if (raw.find("\r\n\r\n") == std::string::npos) {
		_buff += raw;
		return ;
	}
	if (_buff.size() > BUFFSIZE) {
		_status = REQUEST_HEADER_FIELD_TOO_LARGE;
		// エラーレスポンス
		_progress = CREATE_RESPONSE;
		return;
	}
	if (_parser_ctx.execParse(_buff) == -1 || validateRequest() == -1) {
		// エラーレスポンス
		_progress = CREATE_RESPONSE;
		return;
	}
	try
	{
		_location = _config.tryGetLocation(_req.getRequestLine().getPath());
	}
	catch(const std::exception& e)
	{
		_status = FORBIDDEN;
		_progress = CREATE_RESPONSE;
		return;
	}
	return prepareEvent();
}

int AcceptedSocket::validateRequest()
{
	const HTTPRequest::RequestLine & rl = _req.getRequestLine();
	if (rl.getHTTPVersion() != "HTTP/1.1") {
		_status = HTTP_VERSION_NOT_SUPPORTED;
		return -1;
	}
	try
	{
		const std::string & host = _req.tryGetHeaderValue("host");
		size_t colon = host.find(':');
		if (colon != std::string::npos) {
			_config = _configfinder->getConfig(host.substr(0, colon));
		} else {
			_config = _configfinder->getConfig(host);
		}
	}
	catch(const std::exception& e)
	{
		_status = BAD_REQUEST;
		return -1;
	}
	if (_req.hasObsFold() == true) {
		return processObsFold();
	}
	return 0;
}

int AcceptedSocket::processObsFold()
{
	try
	{
		if (_req.tryGetHeaderValue("content-type") != "message/http")
		{
			_status = BAD_REQUEST;
			return -1;
		}
	}
	catch(const std::exception& e)
	{
		_status = BAD_REQUEST;
		return -1;
	}
	_status = UNSUPPORTED_MEDIA_TYPE;
	return -1;
}

void AcceptedSocket::prepareEvent()
{
	if (_location.getReturn().first != 0) {
		_status = static_cast<HTTPStatus>(_location.getReturn().first);
		_progress = CREATE_RESPONSE;
		return;
	}
	const std::string & method = _req.getRequestLine().getMethod();
	const std::map<std::string, bool>::const_iterator it = \
		_location.getAllowedMethod().find(method);
	if (it == _location.getAllowedMethod().end()) {
		_status = NOT_IMPLEMENTED;
		_progress = CREATE_RESPONSE;
		return ;
	} else if (it->second == false) {
		_status = METHOD_NOT_ALLOWED;
		_progress = CREATE_RESPONSE;
		return ;
	}
	return addEvent();
}

void AcceptedSocket::addEvent()
{
	if (isCGI() == true) {
		if (prepareCGI() == false) {
			_progress = CREATE_RESPONSE;
			return ;
		}
		_receiver = new CGI(getSubject(),
							getCommandList(),
							_location.getCgiExtensions(),
							this);
	} else {
		_receiver = new File(getSubject(), getCommandList(),
						this,
						_location.getAutoIndex() == "on",
						_location.getIndexFile());
	}
	const HTTPRequest::RequestLine & rl = _req.getRequestLine();
	if (rl.getMethod() == POST) {
		_receiver->setHTTPMethod(new Post(_receiver));
		preparePostEvent();
	} else {
		std::string path = rl.getPath();
		if (_location.getAlias() != "default") {
			path.replace(0,
			_location.getPath().size(),
			_location.getAlias());
		}
		_receiver->setPath(path);
		if (rl.getMethod() == GET) {
			_receiver->setHTTPMethod(new Get(_receiver));
		} else {
			_receiver->setHTTPMethod(new Delete(_receiver));
		}
	}
	addCommand(_receiver->getHTTPMethod());
}

void AcceptedSocket::preparePostEvent()
{
	std::string path = _req.getRequestLine().getPath();
	if (_location.getUploadPlace() != "default")
	{
		path.replace(0,
				_location.getPath().size(),
				_location.getUploadPlace());
	} else if (_location.getAlias() != "default") {
		path.replace(0,
				_location.getPath().size(),
				_location.getAlias());
	}
	_receiver->setPath(path);
	try
	{
		const std::string & te = _req.tryGetHeaderValue("transfer-encoding");
		if (te != "chunked") {
			_status = NOT_IMPLEMENTED;
			_progress = CREATE_RESPONSE;
			return ;
		}
		_progress = RECEIVE_CHUNKED_SIZE;
		return ;
	}
	catch(const std::exception& e){}
	try
	{
		const std::string & te = _req.tryGetHeaderValue("content-length");
		std::stringstream ss;
		ss << te;
		ss >> _body_size;
		if (ss) {
			_status = BAD_REQUEST;
			_progress = CREATE_RESPONSE;
			return ;
		}
	}
	catch(const std::exception& e){
		_body_size = 0;
	}
	_progress = RECEIVE_REQUEST_BODY;
}

void AcceptedSocket::processRequestBody(
	const std::string & raw
)
{
	if (_config.getMaxBodySize() < _body_size) {
		_status = PAYLOAD_TOO_LARGE;
		_progress = CREATE_RESPONSE;
		return ;
	}
	_receiver->addContent(raw.substr(0, _body_size - _buff.size()));
	if (_body_size == _buff.size()) {
		_progress == EXECUTE_METHOD;
		return ;
	}
}

void AcceptedSocket::processChunkedBody(
	const std::string & raw
)
{
	for (size_t i = 0; i < raw.size();)
	{
		size_t index = raw.find("\r\n", i);
		size_t tmp;
		if (_progress == RECEIVE_CHUNKED_SIZE) {
			if (index == std::string::npos) {
				index = raw.size();
			}
			std::stringstream ss;
			ss << std::dec << std::hex << raw.substr(i, index - i);
			ss >> tmp;
			_body_size += tmp;
			if (_body_size == 0) {
				_progress = EXECUTE_METHOD;
				return ;
			}
			_progress = RECEIVE_CHUNKED_BODY;
		} else if (_progress == RECEIVE_CHUNKED_BODY) {
			if (index == std::string::npos) {
				_receiver->addContent(raw.substr(i, raw.size() - i));
				_body_size -= raw.size() - i;
				return ;
			}
			std::string chunked_body = raw.substr(i, index - i);
			if (chunked_body.size() != _body_size) {
				_status = BAD_REQUEST;
				_progress = CREATE_RESPONSE;
				return ;
			}
			_receiver->addContent(chunked_body);
			if (_config.getMaxBodySize() < _receiver->getContent().size()) {
				_status = PAYLOAD_TOO_LARGE;
				_progress = CREATE_RESPONSE;
				return ;
			}
			_progress = RECEIVE_CHUNKED_SIZE;
			_body_size = 0;
		}
		i = index + 2;
	}
}

bool AcceptedSocket::isCGI() const
{
	return _location.getCgiExtensions().size() > 0;
}

bool AcceptedSocket::prepareCGI()
{
	std::stringstream ss;
	std::string len;
	ss << _buff.size();
	ss >> len;
	if (setenv("CONTENT_LENGTH", len.c_str(), 1) == -1) {
		_status = INTERNAL_SERVER_ERROR;
		return false;
	}
	if (setenv("SERVER_NAME", _config.getServerName().c_str(), 1) == -1 ||
		setenv("SERVER_PORT", _config.getListen().c_str(), 1) == -1 ||
		setenv("REMOTE_ADDR", inet_ntoa(_info.sin_addr), 1) == -1)
	{
		_status = INTERNAL_SERVER_ERROR;
		return false;
	}
	//_parser_ctx.transitionTo(new CGIResponseParser(&_req));
}

// for test
int AcceptedSocket::write()
{
	ssize_t nb = ::send(_sockfd, _buff.c_str(), _buff.size(), 0);
	if (nb == -1) {
		getSubject()->unsubscribe(_sockfd, false);
		return -1;
	}
	if (nb < _nb + _buff.size()) {
		_nb += nb;
		_buff = _buff.substr(0, nb);
		return 1;
	}
	getSubject()->unsubscribe(_sockfd, false);
	return 0;
}

// for test
void AcceptedSocket::processCGIResponse(
	const std::string & cgi_res
)
{
	createResponse(cgi_res);
}

// for test
void AcceptedSocket::createResponse(const std::string & body)
{
	std::cout << "######### TEST ##########" << std::endl;
	_buff = body;
	getSubject()->subscribe(_sockfd, POLLOUT, this);
}

/*
・✅Data
・✅Server
・Location(3xx or 201)
・Transfer-Encoding(Content-Length)
・✅Connection
・[content-type]
*/
void AcceptedSocket::createResponse()
{
	createGeneralHeader();
	int status = static_cast<int>(_status);
	if (400 <= status && status < 600) {
		createErrorResponse();
	} else if (300 <= status && status < 400) {
		createRedirectResponse();
	} else {
		if (_status == CREATED) {
			_res.insertHeaderField(
				"Location",
				_req.getRequestLine().getPath());
		}
		_res.setMessageBody(_receiver->getContent());
	}
	if (_res.getHeaderField().find("Transfer-Encoding") == _res.getHeaderField().end()) {
		std::stringstream ss;
		ss << _res.getMessageBody().size();
		_res.insertHeaderField("Content-Lenght", ss.str());
	}

	getSubject()->subscribe(_sockfd, POLLOUT, this);
}

static void getGMTTime(
	std::string & date
)
{
	time_t now;
	struct tm *ptm;
	char buf[128];
	std::stringstream ss;
	static const std::string day[6] = {"Sun",
									"Mod",
									"Tue",
									"Wed",
									"Thu"
									"Fri"
									"Sat"};

	now = time(NULL);
	ptm = gmtime(&now);
	strftime(buf, sizeof(buf), "%Y/%m/%d %H:%M:%S %Z", ptm);
	ss << day[ptm->tm_wday] << ", "
	<< ptm->tm_mday << " " << ptm->tm_mon << " " << ptm->tm_year
	<< " " << ptm->tm_hour << ":" << ptm->tm_min << ":" << ptm->tm_sec << " GMT";
	ss >> date;
}

void AcceptedSocket::createGeneralHeader()
{
	std::string date;
	getGMTTime(date);
	_res.insertHeaderField("Data", date);
	_res.insertHeaderField("Server", "42Webserv");
	try
	{
		_res.insertHeaderField("Connection",
					 			_req.tryGetHeaderValue("connection"));
	}
	catch(const std::exception& e)
	{
		_res.insertHeaderField("Connection", "keep-alive");
	}
}

/*
<html>
<head><title>301 Moved Permanently</title></head>
<body>
<center><h1>301 Moved Permanently</h1></center>
<hr><center>nginx/1.23.3</center>
</body>
</html>
*/
void AcceptedSocket::createRedirectResponse()
{
	_res.setStatusCode(_status);
	_res.insertHeaderField("Location", _location.getReturn().second);
	createResponseTemplate();
}

void AcceptedSocket::createErrorResponse()
{
}

void AcceptedSocket::createResponseTemplate()
{
	const std::string & comment = _res.getStatusLine().getCode() + " " + _res.getStatusLine().getReason();
	_res.addMessageBody("<html>\n<head><title>");
	_res.addMessageBody(comment);
	_res.addMessageBody("</title></head>\n<body>\n<center><h1>");
	_res.addMessageBody(comment);
	_res.addMessageBody("</h1></center>\n<hr><center>42WebServ</center>\n</body>\n</html>");
	_res.insertHeaderField("Content-Type", "text/html");
}

// for test
void AcceptedSocket::processTest()
{
	char cd[1024];
	getcwd(cd, 1024);
	std::string current_dir = cd;
	//std::string test_file = "handler";
	//size_t index = current_dir.find(test_file);
	//current_dir = current_dir.substr(
	//	0,
	//	index);
	//current_dir += "/html/test.html";
	//current_dir += "/exp/cgi_scripts/perl_post.cgi";
	current_dir += "/sample";

	//setenv("CONTENT_LENGTH", "24", 1);

	//setenv("QUERY_STRING", "value=aaaa&value_2=bbbb", 1);

	//_req.setRequestLine("GET", "", "");
	_req.setRequestLine("POST", "", "value=aaaa&value_2=bbbb");

	std::cout << current_dir << std::endl;

	//_receiver = new File(
	//	getSubject(),
	//	getCommandList(),
	//	current_dir,
	//	O_CLOEXEC,
	//	this
	//);

	//_receiver = new File (
	//	getSubject(),
	//	getCommandList(),
	//	current_dir,
	//	O_RDWR | O_CREAT | O_EXCL,
	//	S_IWGRP,
	//	this
	//);

	//_receiver = new CGI(getSubject(),
	//					getCommandList(),
	//					current_dir,
	//					this);
	_progress = EXECUTE_METHOD;
	if (_req.getRequestLine().getMethod() == GET) {
		_receiver->setHTTPMethod(new Get(_receiver));
	} else if (_req.getRequestLine().getMethod() == POST) {
		_receiver->setHTTPMethod(new Post(_receiver));
	}
	getCommandList()->push_back(_receiver->getHTTPMethod());
}
