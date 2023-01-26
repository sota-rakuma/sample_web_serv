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
		// HTTPStatus _code;
		std::string _code; // CGIレスポンスパーサー内でパースした値をそのまま代入する??
		std::string _reason; // Status: 9999abcd\n  ←の時、_code = 9999、　_reason = abcd
	public:
		StatusLine();
		StatusLine(
			const std::string &,
			const std::string &,
			// HTTPStatus,
			const std::string &
		);
		StatusLine(const StatusLine &);
		~StatusLine();
		const std::string & getHTTPVersion() const;
		std::string getCode() const;
		// HTTPStatus getCode() const;
		const std::string & getReason() const;
		StatusLine & setHTTPVersion(const std::string &);
		StatusLine &setCode(std::string);
		// StatusLine &setCode(HTTPStatus);
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
		std::string
		// HTTPStatus
	);
	HTTPResponse & setStatusLine(
		const std::string &,
		// HTTPStatus,
		const std::string &,
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
