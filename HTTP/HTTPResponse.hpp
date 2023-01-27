#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include "HTTPStatusCode.hpp"
#include <string>
#include <map>

class HTTPResponse
{
public:
	class StatusLine
	{
	private:
		std::string _version;
		std::string _code;
		std::string _reason;
	public:
		StatusLine();
		StatusLine(
			const std::string &,
			const std::string &,
			const std::string &
		);
		StatusLine(const StatusLine &);
		~StatusLine();
		const std::string & getHTTPVersion() const;
		const std::string & getCode() const;
		const std::string & getReason() const;
		StatusLine & setHTTPVersion(const std::string &);
		StatusLine &setCode(const std::string &);
		StatusLine & setReason(const std::string &);
		StatusLine & operator=(const StatusLine &);
	};
private:
	static std::map<HTTPStatus, const std::string &> _err_msg;
	StatusLine _statl;
	std::map<std::string, std::string> _hf;
	std::string _body;
public:
	HTTPResponse();
	HTTPResponse(const HTTPResponse &);
	~HTTPResponse();
	const StatusLine & getStatusLine() const;
	const std::map<std::string, std::string> &
	getHeaderField() const;
	const std::string & getHeaderValue(
		const std::string &
	) const;
	const std::string & getMessageBody() const;
	HTTPResponse & setStatusCode(
		HTTPStatus
	);
	HTTPResponse & setStatusCode(
		const std::string &
	);
	HTTPResponse & setStatusCode(
		const std::string &,
		const std::string &
	);
	HTTPResponse & setStatusLine(
		const std::string &,
		const std::string &,
		const std::string &
	);
	HTTPResponse & setHeaderField(
		const std::map<std::string, std::string> &
	);
	HTTPResponse & insertHeaderField(
		const std::pair<std::string, std::string> &
	);
	HTTPResponse & insertHeaderField(
		const std::string &,
		const std::string &
	);
	HTTPResponse & setMessageBody(
		const std::string &
	);
	HTTPResponse & addMessageBody(
		const std::string &
	);
};

std::ostream &operator<<(
	std::ostream &,
	const HTTPResponse::StatusLine &
);

std::ostream &operator<<(
	std::ostream &,
	const std::map<std::string, std::string> &
);

std::ostream &operator<<(
	std::ostream &,
	const HTTPResponse &
);

#endif /* HTTPRESPONSE_HPP */
