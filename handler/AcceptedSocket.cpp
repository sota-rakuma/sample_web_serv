#include "AcceptedSocket.hpp"
#include "httphandler/CGI.hpp"
#include "httphandler/File.hpp"
#include <unistd.h>
#include <sstream>
#include "../command/Get.hpp"
#include "../command/Post.hpp"
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
//_parser_ctx(new RequestLineParser()),
_progress(RECEIVE_REQUEST_LINE),
_read(new Read(this)),
_write(new Write(this)),
_receiver(static_cast<HTTPMethodReceiver *>(NULL))
{
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
_read(new Read(this)),
_write(new Write(this)),
_receiver(static_cast<HTTPMethodReceiver *>(NULL))
{
	getSubject()->subscribe(_sockfd, POLLIN, this);
}

AcceptedSocket::~AcceptedSocket()
{
	delete _read;
	delete _write;
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
		getCommandList()->push_back(_write);
	}
	else if (event & POLLIN) {
		getCommandList()->push_back(_read);
	}
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

// \x0d\x0a → \r\n → CRLF
void AcceptedSocket::processRequest(const std::string & raw)
{
	if (_progress == RECEIVE_REQUEST_LINE) {
		processRequestLine(raw);
	} else if (_progress == RECEIVE_REQUEST_HEADER) {
		processRequestHeader(raw);
	} else {
		processChunkedBody(raw);
	}
}

void AcceptedSocket::processRequestLine(
	const std::string &raw
)
{
	size_t index = raw.find("\x0d\x0a");
	if (index == std::string::npos) {
		_buff += raw;
		return ;
	}
	//std::string raw = _buff.substr(0, index + 1);
	//_buff = _buff.substr(index + 1, _buff.size() - index);
	//std::vector<std::pair<Symbol, std::string>> tokens;
	// 現状 raw の tokenize 必須
	/*
		パースの方法によっては、
		生のデータをトークンせずに渡す方式に変更してもいい
	*/
	//if (_parser_ctx.execParse(tokens) == SUCCESS) {
		/*
			メソッドのセット
			targetの文字列セット
		*/
		// header の パース
		// _parser_ctx.transitionTo();
		// _buff = "";
	//}
	// BadRequest or HTTPVersion
	// createResponse();
}

void AcceptedSocket::processRequestHeader(
	const std::string & raw
)
{
	if (raw.find("\x0d\x0a\x0d\x0a") == std::string::npos) {
		_buff += raw;
		return ;
	}
	//std::vector<std::pair<Symbol, std::string>> tokens;
	// 現状 raw の tokenize 必須
	/*
		パースの方法によっては、
		生のデータをトークンせずに渡す方式に変更してもいい
	*/
	// Host なかったらパースエラー
	//if (_parser_ctx.execParse(tokens) == ERROR) {
		// BadRequest or HTTPVersion
		// createResponse();
	//}

	//_config = _configfinder->getConfig(
	//	_req.getHeaderValue("Host"));
	/*
		targetのルーティング
		targetの情報設定
		_receiver = new File();
			or
		_receiver = new CGI();
	*/
	if (_req.getRequestLine().getMethod() == POST) {
		_buff = "";
		if (_req.getHeaderValue("Tranfer-Encoding").find("chunked") != std::string::npos) {
			_progress = RECEIVE_CHUNKED_SIZE;
		} else {
			_progress = RECEIVE_REQUEST_BODY;
		}
	} else {
		_progress = EXECUTE_METHOD;
		//_receiver = new File() or CGI();
		if (_req.getRequestLine().getMethod() == GET) {
			getCommandList()->push_back(new Get(_receiver));
		}
	}
}

void AcceptedSocket::processRequestBody(
	const std::string & raw
)
{
	if (_progress == RECEIVE_CHUNKED_SIZE) {
		processChunkedBody(raw);
	} else {
		std::stringstream ss;
		// パースの段階で処理される想定
		//ss << _req.getHeaderValue("Content-Length");
		//ss >> _body_size;
		/*
		if (_conf.getMaxBodySize() < _body_size) {
			// PAYLOAD_TOO_MUCH;
		}
		*/
		_buff += raw.substr(0, _body_size - _buff.size());
		if (_body_size == _buff.size()) {
			_progress == EXECUTE_METHOD;
			return ;
		}
	}
}

void AcceptedSocket::processChunkedBody(
	const std::string & raw
)
{
	for (size_t i = 0; i < raw.size();)
	{
		size_t index = raw.find("\x0d\x0a");
		if (index == std::string::npos) {
			index = raw.size() - 1;
		}

		if (_progress == RECEIVE_CHUNKED_SIZE) {
			std::stringstream ss;
			ss << std::dec << std::hex << raw.substr(i, index);
			ss >> _body_size;
			if (ss) {
				// エラーレスポンス
				std::cerr << "BAD_REQUEST" << std::endl;
			}
			if (_body_size == 0) {
				_progress = EXECUTE_METHOD;
				return ;
			}
			_progress = RECEIVE_REQUEST_BODY;
		}
		else if (_progress == RECEIVE_REQUEST_BODY)
		{
			std::string chunked_body = raw.substr(i, index);
			if (chunked_body.size() != _body_size) {
				// エラーレスポンス
				std::cerr << "BAD_REQUEST" << std::endl;
			}
			_buff += chunked_body;
			if (_config.getMaxBodySize() < _buff.size()) {
				// エラーレスポンス
				std::cerr << "PAYLOAD_TOO_LARGE" << std::endl;
			}
			_progress = RECEIVE_CHUNKED_SIZE;
		}
		i = index + 2;
	}
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
		getCommandList()->push_back(new Get(_receiver));
	} else if (_req.getRequestLine().getMethod() == POST) {
		getCommandList()->push_back(new Post(_receiver));
	}
}
