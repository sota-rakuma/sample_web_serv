#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include "HTTPStatusCode.hpp"
#include <string>
#include <map>
#include <iostream>

#define GET "GET"
#define POST "POST"
#define DELETE "DELETE"

class HTTPRequest
{
public:
	class RequestLine
	{
	private:
		std::string _method;
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
			const RequestLine &
		);
		~RequestLine();
		const std::string & getMethod() const;
		const std::string & getTarget() const;
		const std::string & getHTTPVersion() const;
		RequestLine & setMethod(
			const std::string &
		);
		RequestLine & setTarget(
			const std::string &
		);
		void normalizeTarget(
			std::string &
		);
		void removeDotSegment(
			std::string &
		);
		void percentDocode(
			std::string &
		);
		RequestLine & setHTTPVersion(
			const std::string &
		);
		RequestLine & operator=(const RequestLine &);
	};
private:
	RequestLine _reql;
	std::map<std::string, std::string> _hf;
	bool _obs_fold;
	std::string _body;
public:
	HTTPRequest();
	HTTPRequest(const HTTPRequest &);
	~HTTPRequest();
	const RequestLine & getRequestLine() const;
	const std::map<std::string, std::string> &
	getHeaderField() const;
	const std::string &tryGetHeaderValue(
		const std::string &
	) const;
	const std::string & getMessageBody() const;
	bool hasObsFold() const;
	HTTPRequest & setRequestLine(
		const std::string &,
		const std::string &,
		const std::string &
	);
	HTTPRequest & setHeaderField(
		const std::map<std::string, std::string> &
	);
	HTTPRequest & insertHeaderField(
		const std::string &,
		const std::string &
	);
	HTTPRequest & setMessageBody(
		const std::string &
	);
	HTTPRequest & setObsFold(
		bool
	);
};

std::ostream &operator<<(
	std::ostream &,
	const HTTPRequest &
);




#endif /* HTTPREQUEST_HPP */
