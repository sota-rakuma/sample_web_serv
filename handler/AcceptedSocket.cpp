#include "AcceptedSocket.hpp"
#include "httphandler/CGI.hpp"
#include "httphandler/File.hpp"
#include "../command/Get.hpp"
#include "../command/Post.hpp"
#include "../command/Delete.hpp"
#include <unistd.h>
#include <sstream>
#include <fcntl.h>

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

// socket fd, tmpfd, _receiver, はディープコピーをする必要がある？
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

/*
recv → リクエストラインパース
→ メソッドセット, targetセット(長さ確認)

recv → add buff → headerパース
→ configセット → targetの情報をリクエストにセット

POSTの場合のみ:
recv → リクエストのmessage bodyにセット
*/
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
	case EXECUTE_METHOD:
		addEvent();
	case CREATE_RESPONSE:
	case ERROR:
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
	_progress = ERROR;
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
		_progress = ERROR;
		return;
	}
	if (_parser_ctx.execParse(_buff) == -1 || validateRequest() == -1) {
		// エラーレスポンス
		_progress = ERROR;
		return;
	}
	try
	{
		_location = _config.tryGetLocation(_req.getRequestLine().getTarget());
	}
	catch(const std::exception& e)
	{
		_status = FORBIDDEN;
		_progress = ERROR;
		return;
	}
	return prepareEvent();
	if (_req.getRequestLine().getMethod() == POST) {
		_buff = "";
		//if (_req.getHeaderValue("Tranfer-Encoding").find("chunked") != std::string::npos) {
		//	_progress = RECEIVE_CHUNKED_SIZE;
		//} else {
		//	_progress = RECEIVE_REQUEST_BODY;
		//}
	} else {
		_progress = EXECUTE_METHOD;
		//_receiver = new File() or CGI();
		if (_req.getRequestLine().getMethod() == GET) {
			getCommandList()->push_back(new Get(_receiver));
		}
	}
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
		_config = _configfinder->getConfig(host);
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
		_status = NOT_IMPREMENTED;
		_progress = ERROR;
		return ;
	} else if (it->second == false) {
		_status = METHOD_NOT_ALLOWED;
		_progress = ERROR;
		return ;
	} else if (it->first == POST) {
		return preparePostEvent();
	}
	_progress = EXECUTE_METHOD;
}

void AcceptedSocket::preparePostEvent()
{
	try
	{
		const std::string & te = _req.tryGetHeaderValue("transfer-encoding");
		if (te != "chunked") {
			_status = NOT_IMPREMENTED;
			_progress = ERROR;
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
			_progress = ERROR;
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
		_progress = ERROR;
		return ;
	}
	_buff += raw.substr(0, _body_size - _buff.size());
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
				_buff += raw.substr(i, raw.size() - i);
				_body_size -= raw.size() - i;
				return ;
			}
			std::string chunked_body = raw.substr(i, index - i);
			if (chunked_body.size() != _body_size) {
				_status = BAD_REQUEST;
				_progress = ERROR;
				return ;
			}
			_buff += chunked_body;
			if (_config.getMaxBodySize() < _buff.size()) {
				_status = PAYLOAD_TOO_LARGE;
				_progress = ERROR;
				return ;
			}
			_progress = RECEIVE_CHUNKED_SIZE;
			_body_size = 0;
		}
		i = index + 2;
	}
}

// GET/POSTの中でファイルオープン
void AcceptedSocket::addEvent()
{
	const HTTPRequest::RequestLine & rl =  _req.getRequestLine();
	HTTPMethod *command;
	if (rl.getMethod() == "GET") {
		command = new Get();
	} else if (rl.getMethod() == "POST") {
		command = new Post();
	} else {
		command = new Delete();
	}
	if (_location.getCgiExtensions().size() > 0) {
		_receiver = new CGI(getSubject(),
						getCommandList(),
						command,
						rl.getTarget(),
						this);
	} else {
		_receiver = new File(getSubject(),
						getCommandList(),
						command,
						rl.getTarget(),
						this);
	}
	command->setReceiver(_receiver);
	getCommandList()->push_back(_receiver->getHTTPMethod());
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

/*
エラーレスポンスの場合

ステータスコードから、デフォルトエラーページを取得できた場合、現在持っている_receiverをdeleteして、そのファイルをreceiverに代入して、とってこれた内容をレスポンスのボディとして、レスポンスを作成する。

そうでない場合、それに対応するエラーレスポンスを埋め込んで、れすポンスを作成する(定型文があると、楽)
*/
// for test
void AcceptedSocket::createResponse(const std::string & body)
{
	std::cout << "######### TEST ##########" << std::endl;
	_buff = body;
	getSubject()->subscribe(_sockfd, POLLOUT, this);
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

	_receiver = new File (
		getSubject(),
		getCommandList(),
		current_dir,
		O_RDWR | O_CREAT | O_EXCL,
		S_IRWXU,
		this
	);

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
