#include "AcceptedSocket.hpp"
#include "httphandler/HTTPMethodReceiver.hpp"
#include "httphandler/CGI.hpp"
#include "httphandler/File.hpp"
#include "../command/Get.hpp"
#include "../command/Post.hpp"
#include "../command/Delete.hpp"
#include "../parser/CGIResponseParser.hpp"
#include "../utils/Template.hpp"
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
	std::map<std::string, ServerConfig> * confs
)
:IOEventHandler(subject, commands),
_sockfd(sockfd),
_body_size(0),
_buff(""),
_info(info),
_confs(confs),
_config(confs->begin()->second),
_progress(RECEIVE_REQUEST_LINE),
_receiver(static_cast<HTTPMethodReceiver *>(NULL)),
_is_cgi(false)
{
	_parser_ctx.setParser(new HTTPRequestParser(&this->_req));
	getSubject()->subscribe(_sockfd, POLLIN, this, getMilliTime() + (TIMEOUT * 1000));
}

AcceptedSocket::AcceptedSocket(const AcceptedSocket &another)
:IOEventHandler(another.getSubject(), another.getCommandList()),
_sockfd(another._sockfd),
_body_size(0),
_buff(""),
_confs(another._confs),
_config(another._config),
_parser_ctx(another._parser_ctx),
_info(another._info),
_progress(another._progress),
_receiver(static_cast<HTTPMethodReceiver *>(NULL)),
_is_cgi(another._is_cgi)
{
	getSubject()->subscribe(_sockfd, POLLIN, this, getMilliTime() + (TIMEOUT * 1000));
}

AcceptedSocket::~AcceptedSocket()
{
	if (_receiver != static_cast<HTTPMethodReceiver *>(NULL)) {
		delete _receiver;
	}
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

	if (event == EV_TIMEOUT) {
		createErrorResponse(REQUEST_TIMEOUT);
	} else if (event & POLLOUT) {
		getCommandList()->push_back(getWriteCommand());
	}
	else if (event & POLLIN) {
		getCommandList()->push_back(getReadCommand());
	}
}

int AcceptedSocket::read()
{
	char buff[BUFFSIZE];

	ssize_t nb = ::recv(_sockfd, buff, BUFFSIZE - 1, 0);
	if (nb == -1) {
		getSubject()->unsubscribe(_sockfd, false);
		return -1;
	} else if (nb == 0) {
		getSubject()->unsubscribe(_sockfd, false);
		return 0;
	}
	buff[nb] = '\0';
	processRequest(buff);
	return 1;
}

void AcceptedSocket::processRequest(const std::string & raw)
{
	for (size_t index = 0; index < raw.size();) {
		if (_progress == RECEIVE_REQUEST_LINE)
			index = processRequestLine(raw);
		if (_progress == RECEIVE_REQUEST_HEADER)
			index = processRequestHeader(raw, index);
		if (_progress == RECEIVE_CHUNKED_SIZE)
			index = processChunkedBody(raw, index);
		if (_progress == RECEIVE_CHUNKED_BODY)
			index = processChunkedBody(raw, index);
		if (_progress == RECEIVE_REQUEST_BODY)
			index = processRequestBody(raw, index);
		if (_progress == EXECUTE_METHOD) {
			addCommand(_receiver->getHTTPMethod());
			break;
		}
	}
}

size_t AcceptedSocket::processRequestLine(
	const std::string &raw
)
{
	size_t not_crlf = raw.find_first_not_of("\r\n");
	size_t crlf = raw.find("\r\n");

	if (not_crlf != std::string::npos) {
		if (crlf == std::string::npos) {
			_buff += raw.substr(not_crlf);
			return raw.size();
		}
		_buff += raw.substr(not_crlf, crlf - not_crlf);
	} else if (_buff.size() == 0) {
		return raw.size();
	}
	if (_parser_ctx.execParse(_buff) == 0) {
		_progress = RECEIVE_REQUEST_HEADER;
		_buff.clear();
		return crlf + 2;
	}
	createErrorResponse(BAD_REQUEST);
	return raw.size();
}

size_t AcceptedSocket::processRequestHeader(
	const std::string & raw,
	size_t index
)
{
	if (raw.size() <= index) {
		return index;
	}

	size_t crlf2 = raw.find("\r\n\r\n");
	if (crlf2 == std::string::npos) {
		size_t crlf = raw.rfind("\r\n");
		size_t bufflen = _buff.size();
		if (crlf != 0 ||
			(0 < bufflen && bufflen < 2) ||
			(bufflen > 2 && _buff.rfind("\r\n") != bufflen - 2))
		{
			_buff += raw;
			return raw.size();
		}
		crlf2 = crlf + 2;
		_buff += "\r\n";
	} else {
		crlf2 += 4;
		_buff += raw.substr(index, crlf2);
	}

	if (_buff.size() > BUFFSIZE) {
		createErrorResponse(REQUEST_HEADER_FIELD_TOO_LARGE);
		return raw.size();
	}
	if (_parser_ctx.execParse(_buff) == -1) {
		createErrorResponse(BAD_REQUEST);
		return raw.size();
	}
	if (validateRequest() == -1) {
		return raw.size();
	}
	try
	{
		_location = _config.tryGetLocation(_req.getRequestLine().getPath());
	}
	catch(const std::exception& e)
	{
		createErrorResponse(FORBIDDEN);
		return raw.size();
	}
	if (prepareEvent() == false) {
		return raw.size();
	}
	return crlf2;
}

int AcceptedSocket::validateRequest()
{
	const HTTPRequest::RequestLine & rl = _req.getRequestLine();
	if (rl.getHTTPVersion() != "HTTP/1.1") {
		createErrorResponse( HTTP_VERSION_NOT_SUPPORTED);
		return -1;
	}
	if (_req.getHeaderField().find("expect") != _req.getHeaderField().end()) {
		createErrorResponse(EXPECTAION_FAILED);
		return -1;
	}
	try
	{
		const std::string & host = _req.tryGetHeaderValue("host");
		size_t colon = host.find(':');
		std::map<std::string, ServerConfig>::iterator conf_it;
		if (colon != std::string::npos) {
			conf_it = _confs->find(host.substr(0, colon));
		} else {
			conf_it = _confs->find(host);
		}
		if (conf_it != _confs->end()) {
			_config = conf_it->second;
		}
	}
	catch(const std::exception& e)
	{
		createErrorResponse(BAD_REQUEST);
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
			createErrorResponse( BAD_REQUEST);
			return -1;
		}
	}
	catch(const std::exception& e)
	{
		createErrorResponse(BAD_REQUEST);
		return -1;
	}
	createErrorResponse(UNSUPPORTED_MEDIA_TYPE);
	return -1;
}

bool AcceptedSocket::prepareEvent()
{
	if (_location.getReturn().first != 0) {
		createRedirectResponse(
			static_cast<HTTPStatus>(_location.getReturn().first)
		);
		return false;
	}
	const std::string & method = _req.getRequestLine().getMethod();
	const std::map<std::string, bool>::const_iterator it = \
		_location.getAllowedMethod().find(method);
	if (it == _location.getAllowedMethod().end()) {
		createErrorResponse(NOT_IMPLEMENTED);
		return false;
	} else if (it->second == false) {
		createErrorResponse(METHOD_NOT_ALLOWED);
		return false;
	}
	if (_location.getCgiExtensions().size() > 0) {
		if (prepareCGI() == false) {
			return false;
		}
		_is_cgi = true;
		_receiver = new CGI(getSubject(),
							getCommandList(),
							this,
							_location.getCgiExtensions());
	} else {
		_receiver = new File(getSubject(), getCommandList(),
						this,
						_location.getAutoIndex(),
						_location.getIndexFile());
	}
	if (_req.getRequestLine().getQuery().size() > 0) {
		_receiver->setQuery(_req.getRequestLine().getQuery());
	}
	return setHTTPMethod();
}

static void replacePath(
	std::string & dest,
	const std::string & target,
	const std::string & src)
{
	if (dest.size() < target.size()) {
		return ;
	}
	size_t len = target.size();
	if (dest[target.size()] == '/') {
		len += 1;
	}
	dest.replace(0, len, src);
}

bool AcceptedSocket::setHTTPMethod()
{
	const HTTPRequest::RequestLine & rl = _req.getRequestLine();
	if (rl.getMethod() == POST) {
		_receiver->setHTTPMethod(new Post(_receiver));
		return preparePostEvent();
	}

	std::string path = rl.getPath();
	if (_location.getAlias().size() > 0) {
		replacePath(path, _location.getPath(), _location.getAlias());
	}
	_receiver->setPath(path);
	if (rl.getMethod() == GET) {
		_receiver->setHTTPMethod(new Get(_receiver));
	} else {
		_receiver->setHTTPMethod(new Delete(_receiver));
	}
	_progress = EXECUTE_METHOD;
	return true;
}

bool AcceptedSocket::preparePostEvent()
{
	std::string path = _req.getRequestLine().getPath();
	if (_location.getUploadPlace().size() > 0)
	{
		replacePath(path, _location.getPath(), _location.getUploadPlace());
	} else if (_location.getAlias().size() > 0) {
		replacePath(path, _location.getPath(), _location.getAlias());
	}
	_receiver->setPath(path);
	try
	{
		const std::string & te = _req.tryGetHeaderValue("transfer-encoding");
		if (te != "chunked") {
			createErrorResponse(NOT_IMPLEMENTED);
			return false;
		}
		_progress = RECEIVE_CHUNKED_SIZE;
		return true;
	}
	catch(const std::exception& e){}
	try
	{
		const std::string & te = _req.tryGetHeaderValue("content-length");
		for (size_t i = 0; i < te.size(); i++) {
			if (isDigit(te[i]) == false) {
				createErrorResponse(BAD_REQUEST);
				return false;
			}
		}
		std::stringstream ss;
		ss << te;
		ss >> _body_size;
	}
	catch(const std::exception& e){
		_body_size = 0;
	}
	_progress = RECEIVE_REQUEST_BODY;
	_buff.clear();
	return true;
}

size_t AcceptedSocket::processRequestBody(
	const std::string & raw,
	size_t index
)
{
	if (raw.size() <= index) {
		return index;
	}
	if (_config.getMaxBodySize() < _body_size) {
		createErrorResponse(PAYLOAD_TOO_LARGE);
		return 0;
	}
	_receiver->addContent(raw.substr(
								index,
								_body_size - _receiver->getContent().size()));
	if (_body_size == _receiver->getContent().size()) {
		_progress = EXECUTE_METHOD;
		return 0;
	}
	return raw.size();
}

size_t AcceptedSocket::processChunkedBody(
	const std::string & raw,
	size_t index
)
{
	for (size_t i = index; i < raw.size();)
	{
		size_t crlf = raw.find("\r\n", i);
		size_t tmp = 0;
		if (_progress == RECEIVE_CHUNKED_SIZE) {
			if (crlf == std::string::npos) {
				_buff = raw.substr(i);
				crlf = raw.size();
			}
			size_t len = crlf;
			size_t semcolon = raw.find(';', i);
			bool f = true;
			if (semcolon != std::string::npos) {
				len = semcolon;
			}
			for (size_t j = i; j < len; j++) {
				if (isHexDig(raw[j]) == false) {
					f = false;
				}
			}
			if (f == false || len == 0) {
				createErrorResponse(BAD_REQUEST);
				return 0;
			}
			std::stringstream ss;
			ss << std::dec << std::hex << raw.substr(i, len - i);
			ss >> tmp;
			_body_size += tmp;
			if (_body_size == 0) {
				_progress = EXECUTE_METHOD;
				return 0;
			}
			_progress = RECEIVE_CHUNKED_BODY;
		} else if (_progress == RECEIVE_CHUNKED_BODY) {
			if (crlf == std::string::npos) {
				_receiver->addContent(raw.substr(i, raw.size() - i));
				if (_body_size < raw.size() - i) {
					createErrorResponse(BAD_REQUEST);
				} else {
					_body_size -= raw.size() - i;
				}
				return raw.size();
			}
			std::string chunked_body = raw.substr(i, crlf - i);
			if (chunked_body.size() != _body_size) {
				createErrorResponse(BAD_REQUEST);
				_progress = CREATE_RESPONSE;
				return 0;
			}
			_receiver->addContent(chunked_body);
			if (_config.getMaxBodySize() < _receiver->getContent().size()) {
				createErrorResponse(PAYLOAD_TOO_LARGE);
				return 0;
			}
			_progress = RECEIVE_CHUNKED_SIZE;
			_body_size = 0;
		}
		i = crlf + 2;
	}
	return raw.size();
}

bool AcceptedSocket::isCGI() const
{
	return _is_cgi;
}

bool AcceptedSocket::prepareCGI()
{
	std::stringstream ss;
	std::string len;
	ss << _buff.size();
	ss >> len;
	if (setenv("CONTENT_LENGTH", len.c_str(), 1) == -1) {
		createErrorResponse(INTERNAL_SERVER_ERROR);
		return false;
	}
	if (setenv("SERVER_NAME", _config.getServerName().c_str(), 1) == -1 ||
		setenv("SERVER_PORT", _config.getListen().c_str(), 1) == -1 ||
		setenv("REMOTE_ADDR", inet_ntoa(_info.sin_addr), 1) == -1)
	{
		createErrorResponse(INTERNAL_SERVER_ERROR);
		return false;
	}
	_parser_ctx.transitionTo(new CGIResponseParser(&_res));
	return true;
}

int AcceptedSocket::write()
{
	ssize_t nb = ::send(_sockfd, _buff.c_str(), _buff.size(), 0);
	if (nb == -1) {
		getSubject()->unsubscribe(_sockfd, false);
		return -1;
	}
	if (nb < _buff.size()) {
		_buff = _buff.substr(nb + 1);
		return 1;
	}
	if (_progress == END) {
		if (_res.getHeaderValue("Connection") == "keep-alive") {
			_buff.clear();
			_req.clear();
			_res.clear();
			_parser_ctx.transitionTo(new HTTPRequestParser(&_req));
			delete _receiver;
			_receiver = static_cast<HTTPMethodReceiver *>(NULL);
			_progress = RECEIVE_REQUEST_LINE;
			getSubject()->subscribe(_sockfd, POLLIN, this, getMilliTime() + (TIMEOUT * 1000));
		} else {
			getSubject()->unsubscribe(_sockfd, false);
		}
	} else {
		processResponse();
	}
	return 0;
}

void AcceptedSocket::createNormalResponse()
{
	int ret;
	_buff.clear();
	if (isCGI() == true) {
		processCGIResponse();
	} else {
		int status = static_cast<int>(_status);
		if (_status == CREATED) {
			_res.insertHeaderField(
				"Location",
				_req.getRequestLine().getPath());
		}
		_res.setStatusCode(_status);
		_res.setMessageBody(_receiver->getContent());
	}
	createGeneralHeader();
	if (_res.getHeaderField().find("Transfer-Encoding") == _res.getHeaderField().end()) {
		std::stringstream ss;
		ss << _res.getMessageBody().size();
		_res.insertHeaderField("Content-Length", ss.str());
	}
	std::map<std::string, std::string>::const_iterator ct = _req.getHeaderField().find("content-type");
	if (ct != _req.getHeaderField().end())
	{
		_res.insertHeaderField("Content-Type", ct->second);
	}
	_progress = SEND_STATUS_LINE;
	processResponse();
}

void AcceptedSocket::processCGIResponse()
{
	std::cout << "receiver BUFF: " << _receiver->getContent() << std::endl;
	int ret = _parser_ctx.execParse(_receiver->getContent());
	if (ret == LOCAL_REDIR_RESPONSE) {
		HTTPMethodReceiver * cgi = new CGI(
			getSubject(),
			getCommandList(),
			this,
			_location.getCgiExtensions()
		);
		internalRedirect(cgi,
				_req.getRequestLine().getMethod(),
				_res.getHeaderValue("Location"));
		return ;
	}
	if (ret == ERROR) {
		_buff.clear();
		createErrorResponse(INTERNAL_SERVER_ERROR);
	}
}

static void getGMTTime(
	std::string & date
)
{
	time_t now;
	struct tm *ptm;
	char buf[128];
	std::ostringstream os;
	const static std::string day[7] = {"Sun",
									"Mon",
									"Tue",
									"Wed",
									"Thu",
									"Fri",
									"Sat"};
	now = time(NULL);
	ptm = gmtime(&now);
	os << day[ptm->tm_wday] << ", "
	<< ptm->tm_mday << " " << ptm->tm_mon + 1 << " " << ptm->tm_year + 1900
	<< " " << ptm->tm_hour << ":" << ptm->tm_min << ":" << ptm->tm_sec << " GMT";
	date = os.str();
}

void AcceptedSocket::createGeneralHeader()
{
	std::string date;

	getGMTTime(date);
	_res.insertHeaderField("Date", date);
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

void AcceptedSocket::createRedirectResponse(
	HTTPStatus status
)
{
	_res.setStatusCode(status);
	_res.insertHeaderField("Location", _location.getReturn().second);
	createGeneralHeader();
	createResponseTemplate();
}

void AcceptedSocket::createErrorResponse(
	HTTPStatus status
)
{
	_res.setStatusCode(status);
	std::map<int, std::string>::const_iterator ep = _config.getDefaultErrorPage().find(static_cast<int>(status));
	_res.insertHeaderField("Connection", "close");
	if (ep != _config.getDefaultErrorPage().end())
	{
		_res.insertHeaderField("Content-Type", "text/html");
		HTTPMethodReceiver *hr = new File(getSubject(), getCommandList(), this,
							_location.getAutoIndex(),
							_location.getIndexFile());
		internalRedirect(hr, GET, ep->second);
		_config.eraseDefaultErrorPage(ep->first);
		return ;
	}
	createGeneralHeader();
	createResponseTemplate();
	_progress = SEND_STATUS_LINE;
	processResponse();
}

void AcceptedSocket::createResponseTemplate()
{
	const std::string & statusline = _res.getStatusLine().getCode() + " " + _res.getStatusLine().getReason();
	_res.addMessageBody("<html>\n<head><title>");
	_res.addMessageBody(statusline);
	_res.addMessageBody("</title></head>\n<body>\n<center><h1>");
	_res.addMessageBody(statusline);
	_res.addMessageBody("</h1></center>\n<hr><center>42WebServ</center>\n</body>\n</html>");
	_res.insertHeaderField("Content-Type", "text/html");
	std::stringstream ss;
	ss << _res.getMessageBody().size();
	_res.insertHeaderField("Content-Length", ss.str());
}

void AcceptedSocket::processResponse()
{
	std::ostringstream os;
	_buff.clear();
	if (_progress == SEND_STATUS_LINE) {
		os << _res.getStatusLine();
		_buff = os.str();
		_progress = SEND_RESPONSE_HEADER;
	} else if (_progress == SEND_RESPONSE_HEADER) {
		os << _res.getHeaderField();
		_buff = os.str();
		_progress = SEND_RESPONSE_BODY;
	} else {
		_buff = _res.getMessageBody();
		_progress = END;
	}
	getSubject()->subscribe(_sockfd, POLLOUT | POLLIN, this, 0);
}

void AcceptedSocket::internalRedirect(
	HTTPMethodReceiver * receiver,
	const std::string & method_name,
	const std::string & path,
	const std::string & query
)
{
	delete _receiver;
	_receiver = receiver;
	_receiver->setPath(path);
	_receiver->setQuery(query);
	HTTPMethod * method = static_cast<HTTPMethod *>(NULL);
	if (method_name == GET) {
		method = new Get(receiver);
	} else if (method_name == POST) {
		method = new Post(receiver);
	} else {
		method = new Delete(receiver);
	}
	_receiver->setHTTPMethod(method);
	addCommand(method);
}

void AcceptedSocket::internalRedirect(
	HTTPMethodReceiver * receiver,
	const std::string & method_name,
	const std::string & path
)
{
	delete _receiver;
	_receiver = receiver;
	size_t q = path.find('?');
	if (q != std::string::npos) {
		_receiver->setPath(path.substr(0, q));
		_receiver->setQuery(path.substr(q + 1));
	} else {
		_receiver->setPath(path);
	}
	HTTPMethod * method = static_cast<HTTPMethod *>(NULL);
	if (method_name == GET) {
		method = new Get(receiver);
	} else if (method_name == POST) {
		method = new Post(receiver);
	} else {
		method = new Delete(receiver);
	}
	_receiver->setHTTPMethod(method);
	addCommand(method);
}