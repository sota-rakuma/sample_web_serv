#include "CGIResponseParser.hpp"

CGIResponseParser::CGIResponseParser() : _i(0), _content_type("") {
}

CGIResponseParser::~CGIResponseParser() {
}

int CGIResponseParser::parseExtensionField() {

	return 0;
}

int CGIResponseParser::parseClientLocation() {

	return 0;
}

int CGIResponseParser::parseResponseBody() {

	return 0;
}

int CGIResponseParser::parseOtherField() {

	return 0;
}

int CGIResponseParser::parseStatus() {
    
	return 0;
}

int CGIResponseParser::parseMediaType() {
	std::string media_type;
	size_t pos = _raw.find_first_of("\r\n", _i);
	if (pos == std::string::npos) {
		std::cout << "MediaType in parseMediaType invalid" << std::endl;
		return -1;
	} else if (_raw[pos] == '\r') {
		if (_raw[pos + 1] == '\n') {
			media_type = _raw.substr(_i, pos - _i);
			_i = pos + 2;
		}
	} else {
		media_type = _raw.substr(_i, pos - _i);
		_i = pos + 1;
	}
	size_t i = 0;
	pos = media_type.find("/", i);
	std::string type = media_type.substr(i, );
	
	return 0;
}

int CGIResponseParser::parseContentType() {
	std::string find_word = "Content-Type: ";
	size_t pos = _raw.find(find_word, _i);
	if (pos != _i) {
		std::cout << "find() in parseDocumentResponse failed" << std::endl;
		return -1;
	}
	_i = pos + find_word.length();
	if (parseMediaType() == -1) {
		std::cout << "" << std::endl;
		return -1;
	}
	return 0;
}

int CGIResponseParser::parseClientRedirDocResponse() {
	if (parseClientLocation() == -1) {
		std::cout << "parseClientLocation in parseClientRedirDocResponse failed" << std::endl;
		return -1;
	}
	if (parseStatus() == -1) {
		std::cout << "parseStatus in parseClientRedirDocResponse failed" << std::endl;
		return -1;
	}
	if (parseContentType() == -1) {
		std::cout << "parseContentType in parseClientRedirDocResponse failed" << std::endl;
		return -1;
	}
	if (parseOtherField() == -1) {
		std::cout << "parseOtherField in parseClientRedirDocResponse failed" << std::endl;
		return -1;
	}
	if (parseResponseBody() == -1) {
		std::cout << "parseResponseBody in parseClientRedirDocResponse failed" << std::endl;
		return -1;
	}
	return 0;
}

int CGIResponseParser::parseClientRedirResponse() {
	if (parseClientLocation() == -1) {
		std::cout << "parseClientLocation in parseClientRedirResponse failed" << std::endl;
		return -1;
	}
	if (parseExtensionField() == -1) {
		std::cout << "parseExtensionField in parseClientRedirResponse failed" << std::endl;
		return -1;
	}
	return 0;
}

int CGIResponseParser::parseLocalRedirResponse() {

	return 0;
}


int CGIResponseParser::parseDocumentResponse() {
	if (parseContentType() == -1) {
		std::cout << "parseContentType in parseDocumentResponse failed" << std::endl;
		return -1;
	}
	if (parseStatus() == -1) {
		std::cout << "parseStatus in parseDocumentResponse failed" << std::endl;
		return -1;
	}
	if (parseOtherField() == -1) {
		std::cout << "parseOtherField in parseDocumentResponse failed" << std::endl;
		return -1;
	}
	if (parseResponseBody() == -1) {
		std::cout << "parseResponseBody in parseDocumentResponse failed" << std::endl;
		return -1;
	}
	return 0;
}

int CGIResponseParser::getResponseType() const {
	std::string find_word = "Content-Type: ";
	size_t pos = _raw.find(find_word, _i);
	if (pos == _i)
		return DOCUMENT_RESPONSE;
	find_word = "Location: ";
	pos = _raw.find(find_word, _i);
	if (pos == _i) { // response_typeの判別方法を書く
		// if (~~~~)
		//     return LOCAL_REDIR_RESPONSE;
		// if (++++)
		//     return CLIENT_REDIR_RESPONSE;
		// if (!!!!)
		//     return CLIENT_REDIRDOC_RESPONSE;
	}
	return -1;
}

int CGIResponseParser::parse(const std::string &raw) {
	_raw = raw;
	_response_type = getResponseType();
	if (_response_type == -1) {
		std::cout << "response_type invalid" << std::endl;
		return -1;
	}
	switch (_response_type) {
		case DOCUMENT_RESPONSE:
			if (parseDocumentResponse() == -1)
				return -1;
			break;
		case LOCAL_REDIR_RESPONSE:
			if (parseLocalRedirResponse() == -1)
				return -1;
			break;
		case CLIENT_REDIR_RESPONSE:
			if (parseClientRedirResponse() == -1)
				return -1;
			break;
		case CLIENT_REDIRDOC_RESPONSE:
			if (parseClientRedirDocResponse() == -1)
				return -1;
			break;
		default:
			break; 
	}
	return 0;
}