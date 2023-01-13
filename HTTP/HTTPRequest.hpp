#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include "HTTPStatusCode.hpp"
#include <string>
#include <map>
#include <iostream>

enum HTTPMethod {
	GET,
	POST,
	DELETE,
};

class HTTPRequest
{
public:
	class RequestLine
	{
	private:
		HTTPMethod _method;
		// targetは権限とかのチェックを行っときたい
		std::string _target;
		std::string _version;
	public:
		RequestLine();
		RequestLine(
			const std::string &,
			const std::string &,
			const std::string &
		);
		RequestLine(
			HTTPMethod,
			const std::string &,
			const std::string &
		);
		RequestLine(
			const RequestLine &
		);
		~RequestLine();
		HTTPMethod getMethod() const;
		const std::string & getTarget() const;
		const std::string & getHTTPVersion() const;
		RequestLine & setMethod(
			HTTPMethod
		);
		RequestLine & setMethod(
			const std::string &
		);
		RequestLine & setTarget(
			const std::string &
		);
		RequestLine & setHTTPVersion(
			const std::string &
		);
		RequestLine & operator=(const RequestLine &);
	};
private:
	RequestLine _reql;
	std::map<std::string, std::string> _hf;
	std::string _body;
public:
	HTTPRequest();
	HTTPRequest(const HTTPRequest &);
	~HTTPRequest();
	const RequestLine & getRequestLine() const;
	const std::map<std::string, std::string> &
	getHeaderField() const;
	const std::string &getHeaderValue(
		const std::string &
	) const;
	const std::string & getMessageBody() const;
	HTTPRequest & setRequestLine(
		const std::string &,
		const std::string &,
		const std::string &
	);
	HTTPRequest & setHeaderField(
		const std::map<std::string, std::string> &
	);
	HTTPRequest & insertHeaderField(
		const std::pair<std::string, std::string> &
	);
	HTTPRequest & setMessageBody(
		const std::string &
	);
};

std::ostream &operator<<(
	std::ostream &,
	const HTTPRequest &
);




#endif /* HTTPREQUEST_HPP */
