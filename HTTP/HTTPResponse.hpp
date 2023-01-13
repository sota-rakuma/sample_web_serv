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
		HTTPStatus _code;
		std::string _reason;
	public:
		StatusLine();
		StatusLine(
			const std::string &,
			HTTPStatus,
			const std::string &
		);
		StatusLine(const StatusLine &);
		~StatusLine();
		const std::string & getHTTPVersion() const;
		HTTPStatus getCode() const;
		const std::string & getReason() const;
		StatusLine & setHTTPVersion(const std::string &);
		StatusLine &setCode(HTTPStatus);
		StatusLine & setReason(const std::string &);
		StatusLine & operator=(const StatusLine &);
	};
private:
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
	HTTPResponse & setStatusLine(
		const std::string &,
		HTTPStatus,
		const std::string &
	);
	HTTPResponse & setHeaderField(
		const std::map<std::string, std::string> &
	);
	HTTPResponse & insertHeaderField(
		const std::pair<std::string, std::string> &
	);
	HTTPResponse & setMessageBody(
		const std::string &
	);
};

std::ostream &operator<<(
	std::ostream &,
	const HTTPResponse &
);

#endif /* HTTPRESPONSE_HPP */
