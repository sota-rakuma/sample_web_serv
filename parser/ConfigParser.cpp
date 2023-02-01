#include "ConfigParser.hpp"
#include "../utils/utils.hpp"

// ConfigParser::ConfigParser() : _i(0), _server_index(0) {
ConfigParser::ConfigParser() : _i(0), _server_i(0){
	std::string sd_tmp[] = {"\tlisten ", "\tserver_name ", "\tmax_body_size ", "\terror_page ", "\n\tlocation "};
	for (size_t i = 0; i < sizeof(sd_tmp) / sizeof(std::string); i++) {
		_searched_directives.push_back(sd_tmp[i]);
	}
	std::string sld_tmp[] = {"\t\tallowed_method ", "\t\tindex ", "\t\talias ", "\t\tautoindex ", "\t\treturn\t", "\t\tupload_place ", "\t\textension "};
	for (size_t i = 0; i < sizeof(sld_tmp) / sizeof(std::string); i++) {
		_searched_location_directives.push_back(sld_tmp[i]);
	}
	std::string am_tmp[] = {"GET", "POST", "DELETE"};
	for (size_t i = 0; i < sizeof(am_tmp) / sizeof(std::string); i++) {
		_allowed_methods.push_back(am_tmp[i]);
	}
}

ConfigParser::~ConfigParser() {
}

int ConfigParser::parseExtension(
	size_t index
) {
	_location_extension_flag = true;
	std::string find_word = ";\n";
	size_t pos = _raw.find(find_word, _i);
	if (pos == std::string::npos) {
		std::cout << "find() in parseExtension failed" << std::endl;
		return 1;
	}
	std::string extension = _raw.substr(_i, pos - _i);
	size_t len = extension.length();
	size_t i = 0;
	std::vector<std::string> vec;
	std::string separator = " ";
	size_t vec_pos;
	while (i < len) {
		size_t i_tmp = i;
		if (extension[i++] != '.') {
			std::cout << "extension invalid" << std::endl;
			return 1;
		}
		vec_pos = extension.find(separator, i);
		if (vec_pos == std::string::npos) {
			while (i < len) {
				if (std::isalnum(extension[i]) == 0) {
					std::cout << "extension invalid" << std::endl;
					return 1;
				}
				i++;
			}
			vec.push_back(extension.substr(i_tmp, i));
			// i = vec_pos + separator.length();
			break;
		} else {
			while (i < vec_pos) {
				if (std::isalpha(extension[i]) == 0) {
					std::cout << "extension invalid" << std::endl;
					return 1;
				}
				i++;
			}
		}
		vec.push_back(extension.substr(i_tmp, vec_pos - i_tmp));
		i = vec_pos + separator.length();
	}
	_i = pos + find_word.length();
	_sc_vec[_server_i].setExtension(index, vec);
	return 0;
}

int ConfigParser::parseUploadPlace(size_t index) {
	_location_upload_place_flag = true;
	std::string find_word = ";\n";
	size_t pos = _raw.find(find_word, _i);
	if (pos == std::string::npos) {
		std::cout << "find() in parseUploadPlace failed" << std::endl;
		return 1;
	}
	std::string upload_place = _raw.substr(_i, pos - _i);
	size_t len = upload_place.length();
	size_t i = 0;
	while (i < len) {
		if (upload_place[i++] != '/') {
			std::cout << "upload_place invalid" << std::endl;
			return 1;
		}
		while (i < len) {
			if (isPrintable(upload_place[i])) {
			} else if (upload_place[i] == '/') {
				break;
			} else {
				std::cout << "upload_place invalid" << std::endl;
				return 1;
			}
			i++;
		}
	}
	_i = pos + find_word.length();
	if (upload_place.size() == 0 ||
		upload_place[upload_place.size() - 1] != '/') {
			upload_place += '/';
	}
	_sc_vec[_server_i].setUploadPlace(index, upload_place);
	return 0;
}

int ConfigParser::parseReturn(size_t index) {
	_location_return_flag = true;
	std::string find_word = ";\n";
	size_t pos = _raw.find(find_word, _i);
	if (pos == std::string::npos) {
		std::cout << "find() in parseReturn failed" << std::endl;
		return 1;
	}
	std::string ret = _raw.substr(_i, pos - _i);
	size_t i = 0;
	while (i < 3) {
		if (std::isdigit(ret[i]) == 0) {
			std::cout << "status_code in parseReturn invalid" << std::endl;
			return 1;
		}
		i++;
	}
	if (ret[i++] != ' ') {
		std::cout << "status_code in parseReturn invalid" << std::endl;
		return 1;
	}
	int status_code = std::atoi((ret.substr(0, 3)).c_str());
	// TargetParserでパースする　（今のところALPHAしか許可してない）
	std::string return_uri = ret.substr(i, pos - _i - i);
	// size_t len = return_uri.length();
	TargetParser tp;
	if (tp.parse(return_uri) == -1) {
		std::cout << "return_uri invalid" << std::endl;
		return 1;
	}
	_i = pos + find_word.length();
	_sc_vec[_server_i].setReturn(index, status_code, return_uri);
	return 0;
}

int ConfigParser::parseAutoIndex(size_t index) {
	_location_autoindex_flag = true;
	std::string find_word = ";\n";
	size_t pos = _raw.find(find_word, _i);
	if (pos == std::string::npos) {
		std::cout << "find() in parseIndex failed" << std::endl;
		return 1;
	}
	std::string auto_index = _raw.substr(_i, pos - _i);
	if (auto_index != "on" && auto_index != "off") {
		std::cout << "auto_index invalid" << std::endl;
		return 1;
	}
	_i = pos + find_word.length();
	_sc_vec[_server_i].setAutoIndex(index, auto_index == "on");
	return 0;
}

int ConfigParser::parseAlias(size_t index) {
	_location_alias_flag = true;
	std::string find_word = ";\n";
	size_t pos = _raw.find(find_word, _i);
	if (pos == std::string::npos) {
		std::cout << "find() in parseAlias failed" << std::endl;
		return 1;
	}
	std::string alias_path = _raw.substr(_i, pos - _i);
	size_t len = alias_path.length();
	size_t i = 0;
	while (i < len) {
		if (alias_path[i++] != '/') {
			std::cout << "alias_path in parseAlias invalid" << std::endl;
			return 1;
		}
		while (i < len) {
			if (isPrintable(alias_path[i])) {
			} else if (alias_path[i] == '/') {
				// i++;
				break;
			} else {
				std::cout << "alias_path in parseAlias invalid" << std::endl;
				return 1;
			}
			i++;
		}
	}
	_i = pos + find_word.length();
	_sc_vec[_server_i].setAlias(index, alias_path);
	return 0;
}

int ConfigParser::parseIndex(size_t index) {
	_location_index_flag = true;
	std::string find_word = ";\n";
	size_t pos = _raw.find(find_word, _i);
	if (pos == std::string::npos) {
		std::cout << "find() in parseIndex failed" << std::endl;
		return 1;
	}
	std::string index_file = _raw.substr(_i, pos - _i);
	size_t len = index_file.length();
	size_t i = 0;
	while (i < len) {
		if (isPrintable(index_file[i]) == 0 && index_file[i] != '.') {
			std::cout << "index invalid" << std::endl;
			return 1;
		}
		i++;
	}
	_i = pos + find_word.length();
	_sc_vec[_server_i].setIndex(index, index_file);
	return 0;
}

int ConfigParser::parseAllowedMethod(size_t index) {
	_location_allowed_method_flag = true;
	const std::string & find_word = ";\n";
	size_t pos = _raw.find(find_word, _i);
	if (pos == std::string::npos) {
		std::cout << "find() in parseAllowedMethod failed" << std::endl;
		return 1;
	}
	std::string allowed_method = _raw.substr(_i, pos - _i);
	size_t len = allowed_method.length();
	size_t i = 0;
	size_t j;
	while (i < len) {
		j = 0;
		while (j < _allowed_methods.size()) {
			if (allowed_method.find(_allowed_methods[j], i) == i) {
				_sc_vec[_server_i].setAllowedMethod(
					index,
					_allowed_methods[j],
					true);
				i += _allowed_methods[j].length();
				if (i == len)
					break;
				if (_raw[_i + i] == ' ')
					i++;
				else {
					std::cout << "allowed_method invalid" << std::endl;
					return 1;
				}
				break;
			}
			j++;
		}
		if (j == _allowed_methods.size()) {
			std::cout << "allowed_method invalid" << std::endl;
			return 1;
		}
	}
	_i = pos + find_word.length();
	return 0;
}

void ConfigParser::initLocationDirectiveFlag() {
	_location_allowed_method_flag = false;
	_location_alias_flag = false;
	_location_index_flag = false;
	_location_upload_place_flag = false;
	_location_autoindex_flag = false;
	_location_return_flag = false;
	_location_extension_flag = false;
}

int ConfigParser::parseArrangedLocationDirectives
(
	size_t index
) {
	size_t i = 0;
	std::string find_word = "\t\t";
	size_t pos;
	initLocationDirectiveFlag();
	while (_raw[_i]) {
		pos = _raw.find(find_word, _i);
		if (pos != _i) {
			find_word = "\t}\n";
			pos = _raw.find(find_word, _i);
			if (pos == _i) {
				_i = pos + find_word.length();
				break;
			}
			std::cout << "find() in parseArrangedLocationDirectives failed" << std::endl;
			return 1;
		}
		if (_raw.find(_searched_location_directives[i], _i) == _i) {
			_i += _searched_location_directives[i].length();
			if (_searched_location_directives[i] == _searched_location_directives[0]) { // "\t\tallowed_method "
				if (_location_allowed_method_flag) {
					std::cout << "duplicate allowed_method directive" << std::endl;
					return 1;
				}
				if (parseAllowedMethod(index) != 0) {
					std::cout << "parseAllowedMethod() failed" << std::endl;
					return 1;
				}
			} else if (_searched_location_directives[i] == _searched_location_directives[1]) { // "\t\tindex "
				if (_location_index_flag) {
					std::cout << "duplicate index directive" << std::endl;
					return 1;
				}
				if (parseIndex(index) != 0) {
					std::cout << "parseIndex() failed" << std::endl;
					return 1;
				}
			} else if (_searched_location_directives[i] == _searched_location_directives[2]) { // "\t\talias "
				if (_location_alias_flag) {
					std::cout << "duplicate alias directive" << std::endl;
					return 1;
				}
				if (parseAlias(index) != 0) {
					std::cout << "parseAlias() failed" << std::endl;
					return 1;
				}
			} else if (_searched_location_directives[i] == _searched_location_directives[3]) { // "\t\tautoindex "
				if (_location_autoindex_flag) {
					std::cout << "duplicate autoindex directive" << std::endl;
					return 1;
				}
				if (parseAutoIndex(index) != 0) {
					std::cout << "parseAutoIndex() failed" << std::endl;
					return 1;
				}
			} else if (_searched_location_directives[i] == _searched_location_directives[4]) { // "\t\treturn\t"
				if (_location_return_flag) {
					std::cout << "duplicate return directive" << std::endl;
					return 1;
				}
				if (parseReturn(index) != 0) {
					std::cout << "parseReturn() failed" << std::endl;
					return 1;
				}
			} else if (_searched_location_directives[i] == _searched_location_directives[5]) { // "\t\tupload_place "
				if (_location_upload_place_flag) {
					std::cout << "duplicate upload_place directive" << std::endl;
					return 1;
				}
				if (parseUploadPlace(index) != 0) {
					std::cout << "parseUploadPlace() failed" << std::endl;
					return 1;
				}
			} else if (_searched_location_directives[i] == _searched_location_directives[6]) { // "\t\textension "
				if (_location_extension_flag) {
					std::cout << "duplicate extension directive" << std::endl;
					return 1;
				}
				if (parseExtension(index) != 0) {
					std::cout << "parseExtension() failed" << std::endl;
					return 1;
				}
			}
			i = -1;
		}
		i++;
		if (i >= _searched_location_directives.size()) {
			std::cout << "not match ArrangedLocationDirectives in parseArrangedLocationDirectives" << std::endl;
			return 1;
		}
	}
	return 0;
}

int ConfigParser::parseLocation() {
	std::string find_word = " {\n";
	bool is_exact = false;
	size_t pos = _raw.find(find_word, _i);
	if (pos == std::string::npos) {
		std::cout << "find() in parseLocation failed" << std::endl;
		return 1;
	}
	if (_raw[_i] == '=') {
		is_exact = true;
		_i += 1;
	}
	std::string path = _raw.substr(_i, pos - _i);
	size_t len = path.length();
	// pathの文字数に制限をつける場合
	// if (len >= ??) {
	// }

	size_t i = 0;
	while (i < len) {
		if (path[i++] != '/') {
			std::cout << "path in parseLocation invalid" << std::endl;
			return 1;
		}
		while (i < len) {
			if (isPrintable(path[i])) {
			} else if (path[i] == '/') {
				i++;
				break;
			} else {
				std::cout << "path in parseLocation invalid" << std::endl;
				return 1;
			}
			i++;
		}
	}

	_sc_vec[_server_i].addLocation();
	size_t index = _sc_vec[_server_i].getLocationVec().size() - 1;
	_sc_vec[_server_i].setPath(index, path);
	_sc_vec[_server_i].setExact(index, is_exact);
	_i = pos + find_word.length();

	if (parseArrangedLocationDirectives(index) != 0) {
		std::cout << "parseArrangedLocations() failed" << std::endl;
		return 1;
	}
	return 0;
}

int ConfigParser::parseErrorPage() {
	int status_code_tmp;
	std::vector<int> status_codes;
	size_t i = 0;
	while (i < 3) {
		if (std::isdigit(_raw[_i++]) == 0) {
			std::cout << "status_code in parseErrorPage invalid" << std::endl;
			return 1;
		}
		i++;
	}
	if (_raw[_i++] != ' ') {
		std::cout << "status_code in parseErrorPage invalid" << std::endl;
		return 1;
	}
	status_code_tmp = std::atoi((_raw.substr(_i - 4, 3)).c_str());
	status_codes.push_back(status_code_tmp);
	// size_t status_codes_len = 4;
	while (std::isdigit(_raw[_i])) {
		i = 0;
		while (i < 3) {
			if (std::isdigit(_raw[_i++]) == 0) {
				std::cout << "status_code in parseErrorPage invalid" << std::endl;
				return 1;
			}
			i++;
			if (i == 3)
				break;
		}
		if (_raw[_i++] != ' ' || i != 3) {
			std::cout << "status_code in parseErrorPage invalid" << std::endl;
			return 1;
		}
		// status_codes_len += 4;
		status_code_tmp = std::atoi((_raw.substr(_i - 4, 3)).c_str());
		status_codes.push_back(status_code_tmp);
	}
	std::string find_word = ";\n";
	size_t pos = _raw.find(find_word, _i);
	if (pos == std::string::npos) {
		std::cout << "find() in parseErrorPage failed" << std::endl;
		return 1;
	}
	std::string default_error_page = _raw.substr(_i, pos - _i);
	size_t len = default_error_page.length();
	// default_error_pageの文字数に制限をつける場合
	// if (len >= ??) {
	// }
	i = 0;
	std::vector<std::string> vec;
	std::string separator = "/";
	size_t vec_pos;
	if (default_error_page[i++] != '/') {
		std::cout << "default_error_page invalid" << std::endl;
		return 1;
	}
	while (i < len) {
		vec_pos = default_error_page.find(separator, i);
		if (vec_pos == std::string::npos) {
			vec.push_back(default_error_page.substr(i, len));
			while (i < len) {
				if (isPrintable(default_error_page[i]) == 0 && default_error_page[i] != '.') {
					std::cout << "default_error_page invalid" << std::endl;
					return 1;
				}
				i++;
			}
			break;
		}
		vec.push_back(default_error_page.substr(i, vec_pos - i));
		i = vec_pos + separator.length();
	}
	_i = pos + find_word.length();
	_sc_vec[_server_i].setDefaultErrorPages(status_codes, default_error_page);
	return 0;
}

int ConfigParser::parseMaxBodySize() {
	_max_body_size_flag = true;
	std::string find_word = ";\n";
	size_t pos;
	pos = _raw.find(find_word, _i);
	if (pos == std::string::npos) {
		std::cout << "find() in parseMaxBodySize failed" << std::endl;
		return 1;
	}
	std::string max_body_size = _raw.substr(_i, pos - _i);
	size_t len = max_body_size.length();
	if (len >= 8) {
		std::cout << "max_body_size invalid" << std::endl;
		return 1;
	}
	size_t i = 0;
	while (i < len) {
		if (std::isdigit(max_body_size[i++]) == 0) {
			std::cout << "max_body_size invalid" << std::endl;
			return 1;
		}
	}
	_i = pos + find_word.length();
	int max_body_size_digit = std::atoi(max_body_size.c_str());
	_sc_vec[_server_i].setMaxBodySize(max_body_size_digit);
	return 0;
}

int ConfigParser::parseServerName() {
	_server_name_flag = true;
	std::string find_word = ";\n";
	size_t pos = _raw.find(find_word, _i);
	if (pos == std::string::npos) {
		std::cout << "find() in parseServerName failed" << std::endl;
		return 1;
	}
	std::string server_name = _raw.substr(_i, pos - _i);
	size_t len = server_name.length();
	size_t i = 0;
	while (i < len) {
		if (isPrintable(server_name[i++]) == 0) {
			std::cout << "server_name invalid" << std::endl;
			return 1;
		}
	}
	_i = pos + find_word.length();
	_sc_vec[_server_i].setServerName(server_name);
	return 0;
}

int ConfigParser::parseListen() {
	_listen_flag = true;
	std::string find_word = ";\n";
	size_t pos = _raw.find(find_word, _i);
	if (pos == std::string::npos) {
		std::cout << "find() in parseListen failed" << std::endl;
		return 1;
	}
	std::string port = _raw.substr(_i, pos - _i);
	size_t len = port.length();
	if (len >= 6) {
		std::cout << "port too long" << std::endl;
		return 1;
	}
	size_t i = 0;
	while (i < len) {
		if (std::isdigit(port[i++]) == 0) {
			std::cout << "port invalid" << std::endl;
			return 1;
		}
	}
	_i = pos + find_word.length();
	_sc_vec[_server_i].setListen(port);
	return 0;
}

int ConfigParser::parseArrangedDirectives() {
	size_t  i = 0;
	while (_raw[_i])
	{
		if (_raw.find(_searched_directives[i], _i) == _i) {
			_i += _searched_directives[i].length();
			if (_searched_directives[i] == _searched_directives[0]) { // "\tlisten "
				if (_listen_flag) {
					std::cout << "duplicate listen directive" << std::endl;
					return 1;
				}
				if (parseListen() != 0) {
					std::cout << "parseListen() failed" << std::endl;
					return 1;
				}
			} else if (_searched_directives[i] == _searched_directives[1]) { // "\tserver_name "
				if (_server_name_flag) {
					std::cout << "duplicate server_name directive" << std::endl;
					return 1;
				}
				if (parseServerName() != 0) {
					std::cout << "parseServerName() failed" << std::endl;
					return 1;
				}
			} else if (_searched_directives[i] == _searched_directives[2]) { // "\tmax_body_size "
				if (_max_body_size_flag) {
					std::cout << "duplicate max_body_size directive" << std::endl;
					return 1;
				}
				if (parseMaxBodySize() != 0) {
					std::cout << "parseMaxBodySize() failed" << std::endl;
					return 1;
				}
			} else if (_searched_directives[i] == _searched_directives[3]) { // "\terror_page "
				if (parseErrorPage() != 0) {
					std::cout << "parseErrorPage() failed" << std::endl;
					return 1;
				}
			} else if (_searched_directives[i] == _searched_directives[4]) {
				// \n\tlocation
				if (parseLocation() != 0) {
					std::cout << "parseLocation() failed" << std::endl;
					return 1;
				}
				if (_raw[_i] == '}') {
					_i++;
					if (_raw[_i] == '\n')
						_i++;
					break;
				}
			}
			i = -1;
		}
		i++;
		if (i >= _searched_directives.size()) {
			std::cout << "not match ArrangedDirectives in parseArrangedDirectives" << std::endl;
			return 1;
		}
	}
	return 0;
}

void ConfigParser::initDirectiveFlag() {
	_listen_flag = false;
	_server_name_flag = false;
	_max_body_size_flag = false;
	_location_allowed_method_flag = false;
	_location_alias_flag = false;
	_location_index_flag = false;
	_location_upload_place_flag = false;
	_location_autoindex_flag = false;
	_location_return_flag = false;
	_location_extension_flag = false;
}

int ConfigParser::parseServer() {
	std::string find_word = "server {\n";
	if (_raw.find(find_word, _i) == _i) {
		_i += find_word.length();
		_sc_vec.push_back(ServerConfig());
		initDirectiveFlag();
		if (parseArrangedDirectives() != 0) {
			std::cout << "parseArrangedDirectives() failed" << std::endl;
			return 1;
		}
		_server_i++;
	} else if (_raw[_i]) {
		std::cout << "server_directive invalid" << std::endl;
		return 1;
	} else {
		std::cout << "parse end" << std::endl;
		return 0;
	}
	return 0;
}

int ConfigParser::checkPermission(const std::string &file) {
	std::ifstream ifs(file);
	if (!ifs.is_open()) {
		std::cout << "open failed" << std::endl;
		return 1;
	}
	if (std::getline(ifs, _raw, '\0').fail() == true) {
		std::cout << "cannot read file by end" << std::endl;
		return 1;
	}
	return 0;
}

int ConfigParser::parse(const std::string &file) {
	if (checkPermission(file) != 0) {
		std::cout << "permission denied" << std::endl;
		return 1;
	}
	while (_raw[_i]) {
		if (parseServer() != 0) {
			std::cout << "parseServer() failed" << std::endl;
			return 1;
		}
		if (_raw[_i] == '\n') {// 改行分
			_i++;
		}
	}
	return 0;
}

const std::vector<ServerConfig> & ConfigParser::getScVec() const
{
	return _sc_vec;
}

// int main() {
//     ConfigParser cp;
//     if (cp.parse("zzz.conf") != 0) {
//         std::cout << "parse() failed" << std::endl;
//         return 1;
//     }
//     // for test
//     std::cout << "parse() successed" << std::endl;
//     std::cout << "sc_vec_size = " << cp.getScVecSize() << std::endl;
//     std::cout << "-----------------------------------------" << std::endl;
//     for (size_t i = 0; i < cp.getScVecSize(); i++) {
//         std::cout << "_listen[" << i << "] = " << cp.getListen(i) << std::endl;
//         std::cout << "_server_name[" << i << "] = " << cp.getServerName(i) << std::endl;
//         std::cout << "_max_body_size[" << i << "] = " << cp.getMaxBodySize(i) << std::endl;
//         std::cout << "_default_error_page[" << "0" << "][400] = " << cp.getDefaultErrorPage(0)[400] << std::endl;
//         std::cout << "_default_error_page[" << "0" << "][404] = " << cp.getDefaultErrorPage(0)[404] << std::endl;
//         std::cout << "_default_error_page[" << "0" << "][500] = " << cp.getDefaultErrorPage(0)[500] << std::endl;
//         std::cout << "_default_error_page[" << "1" << "][404] = " << cp.getDefaultErrorPage(1)[404] << std::endl;
//         std::cout << "alias[/] = " << cp.getLocationVec(i)["/"].getAlias() << std::endl;
//         std::cout << "alias[/bbb] = " << cp.getLocationVec(i)["/bbb"].getAlias() << std::endl;
//         std::cout << "index_file[/] = " << cp.getLocationVec(i)["/"].getIndexFile() << std::endl;
//         std::cout << "index_file[/bbb] = " << cp.getLocationVec(i)["/bbb"].getIndexFile() << std::endl;
//         std::cout << "upload_place[/] = " << cp.getLocationVec(i)["/"].getUploadPlace() << std::endl;
//         std::cout << "autoindex[/] = " << cp.getLocationVec(i)["/"].getAutoIndex() << std::endl;
//         std::cout << cp.getLocationVec(i)["/"].getAllowedMethod().size() << std::endl;
//         for (size_t j = 0; j < cp.getLocationVec(i)["/"].getAllowedMethod().size(); j++) {
//             std::cout << "allowed_method[/] = " << cp.getLocationVec(i)["/"].getAllowedMethod()[j] << std::endl;
//         }
//         for (size_t j = 0; j < cp.getLocationVec(i)["/"].getCgiExtensions().size(); j++) {
//             std::cout << "extension[/] = " << cp.getLocationVec(i)["/"].getCgiExtensions()[j] << std::endl;
//         }
//         std::cout << "return.key = " << cp.getLocationVec(i)["/"].getReturn().first << std::endl;
//         std::cout << "return.value = " << cp.getLocationVec(i)["/"].getReturn().second << std::endl;
//         std::cout << "-----------------------------------------" << std::endl;
//     }
//     return 0;
// }