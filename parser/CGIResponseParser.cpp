#include "CGIResponseParser.hpp"

CGIResponseParser::CGIResponseParser() : _i(0) {
	_http_res = new HTTPResponse();
}

CGIResponseParser::~CGIResponseParser() {
	delete _http_res;
}

int CGIResponseParser::treatNL(std::string &str, size_t &pos, size_t &i) {
	pos = _raw.find_first_of("\r\n", i);
	if (pos == std::string::npos) {
		std::cout << "treatNL failed" << std::endl;
		return -1;
	} else if (_raw[pos] == '\r') {
		if (_raw[pos + 1] == '\n') {
			str = _raw.substr(i, pos - i);
			i = pos + 2;
		} else {
			str = _raw.substr(i, pos - i);
			i = pos + 1;
		}
	} else if (_raw[pos] == '\n') {
		str = _raw.substr(i, pos - i);
		i = pos + 1;
	}
	return 0;
}

int CGIResponseParser::parseResponseBody() {
	if (_raw[_i] == '\r') {
		if (_raw[_i - 1] == '\r' && _raw[_i + 1] != '\n') {
			std::cout << "NL in parseDocumentResponse invalid" << std::endl;
			return -1;
		}
	} else if (_raw[_i] != '\n') {
		std::cout << "NL in parseDocumentResponse invalid" << std::endl;
		return -1;
	}
	if (_raw[_i] == '\r') {
		if (_raw[_i + 1] == '\n')
			_i += 2;
		else
			_i++;
	} else if (_raw[_i] == '\n')
		_i++;
	std::string message_body = _raw.substr(_i);
	_http_res->setMessageBody(message_body);
	return 0;
}

int CGIResponseParser::parseClientLocation() {
	std::string find_word = "Location:";
	size_t pos = _raw.find(find_word, _i);
	if (pos != _i) {
		std::cout << "find() in parseClientLocation failed" << std::endl;
		return -1;
	}
	_i = pos + find_word.length();
	std::string location_value;
	if (treatNL(location_value, pos, _i) == -1) {
		std::cout << "treatNL in parseClientLocation failed" << std::endl;
		return -1;
	}
	pos = location_value.find_first_not_of(" ", 0);
	if (pos == std::string::npos) {
	} else {
		location_value = location_value.substr(pos);
	}
	std::pair<std::string, std::string> location_pair = std::make_pair("Location", location_value);
	_http_res->insertHeaderField(location_pair);
	_http_res->setStatusLine("HTTP/1.1", "302", "Moved Temporarily");
	return 0;
}

int CGIResponseParser::parseOtherField() {
	size_t pos;
	std::string other_field;
	std::string find_word;
	std::string field_name;
	std::string field_value;
	size_t i = 0;
	size_t len;
	
	while (_raw[_i] != '\n' && _raw[_i] != '\r') {
		if (treatNL(other_field, pos, _i) == -1) {
			std::cout << "treatNL in parseOtherField failed" << std::endl;
			return -1;
		}
		find_word = ":";
		pos = other_field.find(find_word, 0);
		if (pos == std::string::npos) {
			std::cout << "otherField invalid" << std::endl;
			return -1;
		}
		field_name = other_field.substr(0, pos);
		field_value = other_field.substr(pos + 1);
		i = 0;
		len = field_name.length();
		while (i < len) {
			if (isPrintable(field_name[i]) == true && field_name[i] != ' ') {
			} else {
				std::cout << "field_name invalid in parseOtherField" << std::endl;
				return -1;
			}
			i++;
		}
		pos = field_value.find_first_not_of(" ", 0);
		if (pos != std::string::npos)
			field_value = field_value.substr(pos);
		len = field_value.length();
		i = 0;
		while (i < len) {
			if (isPrintable(field_value[i]) == false) {
				std::cout << "field_value invalid in parseOtherField" << std::endl;
				return -1;
			}
			i++;
		}
		std::pair<std::string, std::string> field_pair = std::make_pair(field_name, field_value);
		_http_res->insertHeaderField(field_pair);
	}
	return 0;
}

int CGIResponseParser::parseStatus() {
    std::string find_word = "Status:";
	size_t pos = _raw.find(find_word, _i);
	if (pos != _i) {
		std::cout << "status not exist" << std::endl;
		return 0;
	}
	_i = pos + find_word.length();
	std::string status_values;
	if (treatNL(status_values, pos, _i) == -1) {
		std::cout << "treatNL in parseStatus failed" << std::endl;
		return -1;
	}
	pos = status_values.find_first_not_of(" ", 0);
	if (pos == std::string::npos) {
		std::cout << "find() in parseStatus failed" << std::endl;
		return -1;
	}
	size_t i = 0;
	size_t len = status_values.length();
	while (i < 3) {
		if (isDigit(status_values[pos + i]) == false) {
			std::cout << "status_code in parseStatus invalid" << std::endl;
			return -1; 
		}
		i++;
	}
	while (isDigit(status_values[pos + i]) == true) {
		i++;
	}
	std::string status_code = status_values.substr(pos, i);
	pos = status_values.find_first_not_of(" ", pos + i);
	if (pos == std::string::npos) {
		std::cout << "reason_phrase in parseStatus invalid" << std::endl;
		return -1;
	}
	std::string reason_phrase = status_values.substr(pos);
	_http_res->setStatusLine("HTTP/1.1", status_code, reason_phrase);
	return 0;
}

int CGIResponseParser::parseContentType() {
	std::string find_word = "Content-Type:";
	size_t pos = _raw.find(find_word, _i);
	if (pos != _i) {
		std::cout << "find() in parseContentType failed" << std::endl;
		return -1;
	}
	_i = pos + find_word.length();
	std::string media_type;
	if (treatNL(media_type, pos, _i) == -1) {
		std::cout << "treatNL in parseContentType failed" << std::endl;
		return -1;
	}
	pos = media_type.find_first_not_of(" ", 0);
	if (pos != std::string::npos)
		media_type = media_type.substr(pos);
	std::pair<std::string, std::string> content_type_pair = std::make_pair("Content-Type", media_type);
	_http_res->insertHeaderField(content_type_pair);
	return 0;
}

int CGIResponseParser::parseClientRedirDocResponse() {
	_res_type = CLIENT_REDIRDOC_RESPONSE;
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
	_res_type = CLIENT_REDIR_RESPONSE;
	if (parseClientLocation() == -1) {
		std::cout << "parseClientLocation in parseClientRedirResponse failed" << std::endl;
		return -1;
	}
	if (parseOtherField() == -1) {
		std::cout << "parseOtherField in parseClientRedirResponse failed" << std::endl;
		return -1;
	}
	return 0;
}

int CGIResponseParser::parseLocalRedirResponse() {
	_res_type = LOCAL_REDIR_RESPONSE;
	std::string find_word = "Location:";
	size_t pos = _raw.find(find_word, _i);
	if (pos != _i) {
		std::cout << "find() in parseLocalRedirResponse failed" << std::endl;
		return -1;
	}
	_i = pos + find_word.length();
	std::string location_value;
	if (treatNL(location_value, pos, _i) == -1) {
		std::cout << "treatNL in parseLocalRedirResponse failed" << std::endl;
		return -1;
	}
	if (_raw[_i] != '\n' && _raw[_i] != '\r') {
		std::cout << "NL invalid in LocalRedirResponse" << std::endl;
		return -1;
	}
	pos = location_value.find_first_not_of(" ", 0);
	if (pos != std::string::npos)
		location_value = location_value.substr(pos);
	std::pair<std::string, std::string> location_pair = std::make_pair("Location", location_value);
	_http_res->insertHeaderField(location_pair);
	_http_res->setStatusLine("HTTP/1.1", "302", "Moved Temporarily");
	return 0;
}


int CGIResponseParser::parseDocumentResponse() {
	_res_type = DOCUMENT_RESPONSE;
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

int CGIResponseParser::getResponseType() {
	std::string find_word = "Content-Type:";
	size_t pos = _raw.find(find_word, _i);
	if (pos == _i)
		return DOCUMENT_RESPONSE;
	find_word = "Location:";
	pos = _raw.find(find_word, _i);
	if (pos == _i) {
		pos = _raw.find_first_not_of(" ", _i + find_word.length());
		if (pos == std::string::npos) {
			std::cout << "find_first_not_of() in getResponseType failed" << std::endl;
			return -1;
		}
		if (_raw[pos] == '/')
		    return LOCAL_REDIR_RESPONSE;
		else {
			size_t i_tmp = pos;
			std::string client_location;
			if (treatNL(client_location, pos, i_tmp) == -1) {
				std::cout << "treatNL in getResponseType failed" << std::endl;
				return -1;
			}
			TargetParser tp;
			if (tp.parse(client_location) == -1) {
				std::cout << "client_location in getResponseType invalid" << std::endl;
				return -1;
			}
			find_word = "Status:";
			pos = _raw.find(find_word, i_tmp);
			if (pos == i_tmp)
				return CLIENT_REDIRDOC_RESPONSE;
			else
				return CLIENT_REDIR_RESPONSE;		
		}
	}
	return -1;
}

int CGIResponseParser::parse(const std::string &raw) {
	_raw = raw;
	_response_type = getResponseType();
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
			std::cout << "response_type invalid" << std::endl;
			return -1;
			break; 
	}
	return 0;
}