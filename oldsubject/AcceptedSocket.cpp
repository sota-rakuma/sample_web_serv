#include "AcceptedSocket.hpp"
#include <unistd.h>

AcceptedSocket::AcceptedSocket()
:EventHandler()
{
}

AcceptedSocket::AcceptedSocket(
	IObserver *oldobserver,
	int sockfd,
	const sockaddr_in &info,
	ServerConfigFinder *configfinder
)
:EventHandler(oldobserver),
_sockfd(sockfd),
_info(info),
_configfinder(configfinder),
_config(),
//_parser_ctx(new RequestLineParser()),
_progress(RECEIVE_REQUEST_LINE),
_read(new Read(this)),
_write(new Write(this))
{
}

// socket fd, tmpfd, _receiver, はディープコピーをする必要がある？
AcceptedSocket::AcceptedSocket(const AcceptedSocket &another)
:EventHandler(another.getObserver()),
_sockfd(another._sockfd),
//_tmpfd(another._tmpfd),
_configfinder(another._configfinder),
_config(another._config),
_parser_ctx(another._parser_ctx),
_info(another._info),
_progress(another._progress),
_read(new Read(this)),
_write(new Write(this))
{
}

AcceptedSocket::~AcceptedSocket()
{
	delete _read;
	delete _write;
	close(_sockfd);
	//if (_tmpfd != -1) {
	//	::close(_tmpfd);
	//}
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

void AcceptedSocket::notify(
	int fd,
	int event,
	EventHandler *oldsubject
)
{
	getObserver()->update(fd, event, oldsubject);
}

ICommand *AcceptedSocket::getHandler(int event) const
{
	// 要例外処理
	if (event == IN) {
		return _read;
	} else if (event == OUT) {
		return _write;
	}
	return NULL;
}

/*
recv → リクエストラインパース
→ メソッドセット, targetセット(長さ確認)

recv → add buff → headerパース
→ configセット → targetの情報をリクエストにセット

POSTの場合のみ:
recv → リクエストのmessage bodyにセット

execMethod:
eventlist.add(_receiver.getHandler(_req.getMethod()));
*/
// チャンクの時の処理も必要
int AcceptedSocket::receive()
{
	char buff[BUFFSIZE];

	ssize_t nb = ::recv(_sockfd, buff, BUFFSIZE, 0);
	if (nb == -1) {
		notify(_sockfd, REMOVE, this);
	} else if (nb == 0) {
		// ソケットストリームが正しく閉じられた場合
		notify(_sockfd, REMOVE, this);
		// ソケットが閉じられて、file/cgiが存在する可能性は
		return 0;
	}
	buff[nb] = '\0';
	_buff += buff;
	processRequest();
	return 1;
}

// \x0d\x0a → \r\n → CRLF
void AcceptedSocket::processRequest()
{
	size_t index = _buff.find_first_of("\x0d\x0a");
	if (_progress == RECEIVE_REQUEST_LINE &&
		index != std::string::npos)
	{
		processRequestLine(index);
	} else if (_progress == RECEIVE_REQUEST_HEADER &&
		_buff.find_first_of("\x0d\x0a\x0d\x0a") != std::string::npos)
	{
		processRequestHeader();
	} else if (_progress == RECEIVE_REQUEST_BODY) {
		/**/
	}
}

void AcceptedSocket::processRequestLine(size_t index)
{
	std::string raw = _buff.substr(0, index + 1);
	_buff = _buff.substr(index + 1, _buff.size() - index);
	std::vector<std::pair<Symbol, std::string>> tokens;
	// 現状 raw の tokenize 必須
	/*
		パースの方法によっては、
		生のデータをトークンせずに渡す方式に変更してもいい
	*/
	if (_parser_ctx.execParse(tokens) == SUCCESS) {
		/*
			メソッドのセット
			targetの文字列セット
		*/
		// header の パーサ
		// _parser_ctx.transitionTo();
	}
	// BadRequest or HTTPVersion
	// createResponse();
}

void AcceptedSocket::processRequestHeader()
{
	if (BUFFSIZE < _buff.size()) {
		//return createResponse(REQUEST_HEADER_FIELD_TOO_LARGE);
	}
	std::vector<std::pair<Symbol, std::string>> tokens;
	// 現状 raw の tokenize 必須
	/*
	パースの方法によっては、
	生のデータをトークンせずに渡す方式に変更してもいい
	*/
	if (_parser_ctx.execParse(tokens) == SUCCESS) {
		_config = _configfinder->getConfig(
			_req.getHeaderValue("Host"));
		/*
			targetのルーティング
			targetの情報設定
			_receiver = new File();
				or
			_receiver = new CGI();
		*/
		if (_req.getRequestLine().getMethod() == POST) {
			_progress = RECEIVE_REQUEST_BODY;
		} else {
			_progress = EXECUTE_METHOD;
		}
	}
	// BadRequest or HTTPVersion
	// createResponse();
}


/*
・chunk:→デコードしたものを_buffにaddしていく
・raw: →Content-Lengthまでデータを_buffに足していく
・common:
	bufferのサイズを超えたら、tmpfileをopen
	一番最後にtmpfileのサイズをカウントして、maxを超えていたら、413
*/
void AcceptedSocket::processRequestBody()
{
}

//int AcceptedSocket::send()
//{
//	ssize_t nb = ::send(_sockfd, _)
//}