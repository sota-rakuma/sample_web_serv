```c++
class Server
{
private:
	EventMonitor _em;
	std::list<ICommand *> _commands;
public:
	void executeEvent();
};

// どっかでコマンドの発火が必要

class EventMonitor
{
private:
	std::vector<struct pollfd> _pv;
	std::map<int, IConnection *> _connections;
public:
	void pollEvent(std::list<ICommands *> & commands);
	void addSubject(int fd, IConnection * p);
};

/*
	cache的なのを持っていてもいいかもしれない。
	固定長の配列か何かにFILEとCGIを持っておいて、アクセス頻度と時間を組み合わせたものを基準にして、みたいな。
	or
	最後にアクセスされた時間
	か
	アクセス頻度
	だけをキーにする
*/

class IConnection
{
private:
	EventMonitor *_evm;
	Read *_r;
	Write *_w;
public:
	virtual void read() = 0;
	virtual void write() = 0;
	Command *getHnadler(int event) const
	{
		if (event & POLLOUT) {
			return _w;
		}
		return _r;
	}
};

// 各fdのclose と vectorからの削除

class ListenSocket : public IConnection
{
private:
	int socketfd;
	std::string _ip:
	std::string _port;
public:
	virtual void accept()
	{
		if (getaddrinfo(_ip, _port, &hint, &res) == -1) {
			// エラー処理
		}
		struct sockadd_in c_info;
		size_t len = sizeof(struct sockaddr_in);
		int fd = accept(_sockfd, &c_info, &len);
		if (fd == -1) {
			// エラー処理
		}
		evm->addSubject(fd, new Client(fd, c_info, this));
	};
	virtual void write(); // 何もしない
};

// 既に開かれてるfdに対してリクエストが来た時どうする？

class Client : public IConnection
{
private:
	static HTTPParser *_parser; // ?
	int sockfd;
	std::string _buff;
	HTTPRequest _req;
	HTTPResponse _res;
	ListenSocket *_ls;
	File *_f;
	CGI *_cgi; //CGIはin_fdとout_fdで一つずつ要素を作ってしまってもいいかもしれない。→ そしたら、メンバ変数にfdはいらない
public:
	Cient();
	void read(); リクエストを作成して、**FILE | CGIのインスタンスを追加して**、そのFILEのfdをpollvecに追加するまで。
	void write();
	void updata(const std::string &);
};

class File : public IConnection
{
private:
	std::string _name;
	int _fd;
	int _status;
	ssize_t _wb;
	std::string _buff;
	std::list<Client *> _oldobserver;
public:
	File(
		EventMonitor *evm,
		const std::string & name
	)
	:IConnection(evm),
	_name(name),
	_status(START)
	{
	};
	void read()
	{
		char buff[1024];
		if (_status == END) {
			return ;
		}
		ssize_t _rb = read(_fd, buff, 1024);
		if (rb == -1) {
			_status == ERROR;
			return ;
		} else if (rb == 0) {
			_status == END;
			return ;
		} else {
			_status == CONTINUE;
			buff[rb] = '\0';
		}
		_buff += buff;
	};
	// EWOULDBLOCKとかでも、もはやエラーとして扱ってしまっていいのか？
	// いや、一回一回pollイベントに戻れば問題ないか。
	void write()
	{
		ssize_t wb;
		wb = write(_fd, _buff, _buff.size());
		if (wb == -1) {
			_status = ERROR;
			return ;
		}
		_wb += wb;
		if (_wb < _buff.size()) {
			_status = CONTINUE;
			return ;
		} else {
			_status = END;
			notify();
		}
	};
};

class CGI : public IConnection
{
private:
	int in_fd;
	int out_fd;
	std::string _buff;
};

class Command
{
public:
	virtual void execute(int) = 0; // にすれば、メンバ変数にfdを持っていなくてもできる。
};

class Read : public Command
{
private:
	IConnection *_c;
public:
	virtual void exectue(int fd)
	{
		_c->read(fd);
	};
}

class Write : public Command
{
private:
	IConnetion *_c;
public:
	virtual void execute(int fd)
	{
		_c->write(fd);
	};
}
```

# コマンドにしたい処理
## ListenSocket
- クライアントからのConnect要求をAcceptして、Pollvecに通信用のソケットを追加する

## AcceptedSocket
- リクエスト
	- Chain of Responsibilityを採用 HandlerはCommandインターフェイスを採用
	(状態(HTTPステータスでもいいかも)でhandleするかどうかを判断する)

	1. クライアントからデータを受信する
	2. リクエストをパースする
		- SUCCESS: 3を行う
		- ERROR: 7を行う
	3. targetが存在するかどうかをチェックする
		- YES: 4を行う
		- NO: 7を行う
	4. targetがリダイレクトの設定がされているかどうかをチェックする
		- YES: 7を行う
		- NO: 5を行う
	5. targetがCGIかどうかをチェックする
		- YES: ReceiverにCGIオブジェクトを渡す
		- NO: targetがopenされてるかどうかをチェックする
			- YES: 既存のFileオブジェクトをReceiverに渡す
			- NO: 読み書き可の状態でファイルをopenして、FileオブジェクトをReceiverに渡す
	6.
```c++
	//FILE
	if (_req.getMethod() == "GET") {
		notify(filefd, POLLIN, new File());
	} else if (_req.getMethod() == "POST") {
		notify(filefd, POLLOUT, new File());
	} else if (_req.getMethod() == "DELETE") {
		_eventlist.add(DELETE(_req.getTarget()));
	}
	// CGI
	if (_req.getMethod() == "GET") {
		// PATHINFO. QUERYSTRINGの処理
		notify(infd, POLLIN, new File());
	} else if (_req.getMethod() == "POST") {
		// PATHINFO. QUERYSTRINGの処理
		notify(infd, POLLOUT, new File());
		notify(outfd, POLLIN, new File());
	} else if (_req.getMethod() == "DELETE") {
		_eventlist.add(DELETE(_req.getTarget()));
	}
	// 本当は全部

	_evnetlist.add(_handler.getHandler(_req.getMethod()))ってしたい。


	if (file) {
		_receiver = new File(...);
	} else if (cgi) {
		_receiver = new CGI(...);
	}
	_evenlist.add(_receiver.getHandler(_req.getMethod()));
```
	6. HTTPメソッドを読み取って、該当するメンバ変数に4で渡したReceiverを渡して、evnelistに追加依頼を通達する
- レスポンス
	1. レスポンスを送信する
	2.ソケットを閉じて、pollvecにfdの削除依頼を通達する
	__ここからの処理は後回し__
	2. keep-aliveがonになってるかどうかを確認する
		...
	n.ソケットを閉じて、pollvecにfdの削除依頼を通達する


```c++
// (GET & FILE) & (READ & FILE)
int File::httpGet()
{
	if (is_exist == false) {
		notify(fd, IN, this);
		return 1;
	}
	char buff[1024];
	ssize_t nb = read(fd, buff, 1024);
	if (nb < 0) {
		return -1;
	} else if (nb == 0) {
		for (std::list<AcceptedSocket *>::iterator it = _as.begin();
			it != _as.end();
			it++)
		{
			it->createResponse(200, _header, _buff);
		}
		notify(fd, DELETE, this);
		return 0;
	}
	buff[nb] = '\0';
	_buff += buff;
	return 1;
}

// (GET & CGI) & (READ & CGI)
int CGI::httpGet()
{
	if (is_exist == false) {
		// writeの後に呼ばれても二回実行されることはない。
		// dup → pipe → fork → close
		//→ execve(子プロセス)
		//→ notify(fd, IN, this); 親プロセス
		executeCGI(IN); // POSTならnotify(out_fd, OUT, this);も
		return 1;
	}
	char buff[1024];
	ssize_t nb = read(fd, buff, 1024);
	if (nb < 0) {
		return -1;
	} else if (nb == 0) {
		// CGIレスポンスのパース
		if (_res.kind() == LOCAL_REDIRECT) {
			_as->route(_location);
		} else {
			_as->createResponse(200, _header,_buff);
		}
		notify(fd, DELETE, this);
		return 0;
	}
	buff[nb] = '\0';
	_buff += buff;
	return 1;
}

```

## File
- GET
	1. fdとPOLLINフラグをPollvecに追加する
	2. イベントが通知されたら、fdからファイルの読み込みを開始して、bufferにデータを溜め込む
	3. エラー or EOFに達するまで2を繰り返す
	4. AcceptedSocketにレスポンスの作成依頼を通達する
	5. EOFに達したら、ファイルを閉じて、レスポンス作成
	6. update(acceptedFD, OUT, NULL);
	5. Pollvecを持つクラスとFileオブジェクトを持つクラスに削除要求をする

- POST
	1. fdとPOLLOUTフラグをPollvecに追加する
	2. イベントが通知されたら、fdからファイルの書き込みを開始する
	3. もし、ファイルの書き込みがサイズ分行われなかったら、2を再度行う
	4. エラー or bufferサイズ分書き込みが終わるまで、2,3を繰り返す
	5. 書き込んだバイト数がbufferサイズと等しくなったら、ファイルを閉じて、Pollvecを持つクラスとFileオブジェクトを持つクラスに削除要求をする
- DELETE
	1. ファイルの削除を行う
## CGI
- GET
	1. CGIの実行環境を作成して、親プロセスの入力側のfdとPOLLINフラグをPollvecに追加する
	2. イベントが通知されたら、CGIレスポンスをパースする
- POST
	1. CGIの実行環境を作成して、親プロセスの入力用のfdとPOLLIN, 出力用のfdとPOLLOUTをPollvecに追加
	2. イベントが通知されたら、そのfdにデータを送信
	3. イベントが通知されたら、CGIレスポンスをパースする
- DELETE
	1. 特に何もしない

