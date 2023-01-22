#include "ConfigParser.hpp"

// ConfigParser::ConfigParser() : _i(0), _server_index(0) {
ConfigParser::ConfigParser() : _i(0), _server_i(0){
    std::string sd_tmp[] = {"\tlisten ", "\tserver_name ", "\tmax_body_size ", "\terror_page ", "\n\tlocation "};
    for (size_t i = 0; i < sizeof(sd_tmp) / sizeof(std::string); i++) {
        _searched_directives.push_back(sd_tmp[i]);
    }
    std::string sld_tmp[] = {"\t\tallowed_method ", "\t\tindex ", "\t\talias ", "\t\tautoindex on", "\t\treturn ", "\t\tupload_place ", "\t\textension "};
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

int ConfigParser::parseExtension(std::string path) {
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
        vec_pos = extension.find(separator, i);
        if (vec_pos == std::string::npos || vec_pos == i) {
            std::cout << "find() in parseExtension failed" << std::endl;
            return 1;
        }
        if (extension[i++] != '.') {
            std::cout << "extension invalid" << std::endl;
            return 1;
        }
        while (i < vec_pos) {
            if (std::isalpha(extension[i]) == 0) {
                std::cout << "extension invalid" << std::endl;
                return 1;
            }
            i++;
        }
        vec.push_back(extension.substr(i, vec_pos - i));
        i = vec_pos + separator.length();
    }
    _i = pos + find_word.length();
    _sc_vec[_server_i].setExtension(path, vec);
    return 0;
}

int ConfigParser::parseUploadPlace(std::string path) {
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
            if (std::isalpha(upload_place[i])) {
            } else if (upload_place[i] == '/') {
                i++;
                break;
            } else {
                std::cout << "upload_place invalid" << std::endl;
                return 1;
            }
            i++;
        }
    }
    _i = pos + find_word.length();
    _sc_vec[_server_i].setUploadPlace(path, upload_place);
    return 0;
}

int ConfigParser::parseReturn(std::string path) {
    size_t i = 0;
    int status_code;
    while (i < 3) {
        if (std::isdigit(_raw[_i++]) == 0) {
            std::cout << "status_code in parseReturn invalid" << std::endl;
            return 1;
        }
        i++;
    }
    status_code = std::atoi((_raw.substr(_i - 3, _i)).c_str());
    if (_raw[_i++] != ' ') {
        std::cout << "status_code in parseReturn invalid" << std::endl;
        return 1;
    }
    // TargetParserでパースする　（今のところALPHAしか許可してない）
    std::string find_word = ";\n";
    size_t pos = _raw.find(find_word, _i);
    if (pos == std::string::npos) {
        std::cout << "find() in parseReturn failed" << std::endl;
        return 1;
    }
    std::string return_uri = _raw.substr(_i, pos - _i);
    size_t len = return_uri.length();
    i = 0;
    while (i < len) {
        if (std::isalpha(return_uri[i++]) == 0) {
            std::cout << "return_uri invalid" << std::endl;
            return 1;
        }
    }
    _i = pos + find_word.length();
    _sc_vec[_server_i].setReturn(path, status_code, return_uri);
    return 0;
}

int ConfigParser::parseAutoIndex(std::string path) {
    std::string find_word = ";\n";
    size_t pos = _raw.find(find_word, _i);
    if (pos == std::string::npos) {
        std::cout << "find() in parseIndex failed" << std::endl;
        return 1;
    }
    std::string ai_on = "autoindex on";
    size_t len = ai_on.length();
    std::string auto_index = _raw.substr(_i - len, len);
    if (auto_index != "autoindex on") {
        std::cout << "auto_index invalid" << std::endl;
        return 1;
    }
    _i = pos + find_word.length();
    _sc_vec[_server_i].setAutoIndex(path, true);
    return 0;
}

int ConfigParser::parseAlias(std::string path) {
    std::string find_word = ";\n";
    size_t pos = _raw.find(find_word, _i);
    if (pos == std::string::npos) {
        std::cout << "find() in parseAlias failed" << std::endl;
        return 1;
    }
    std::string alias_path = _raw.substr(_i, pos - _i);
    // std::cout << alias_path << std::endl;
    size_t len = alias_path.length();
    size_t i = 0;
    while (i < len) {
        if (alias_path[i++] != '/') {
            std::cout << "alias_path in parseAlias invalid" << std::endl;
            return 1;
        }
        while (i < len) {
            if (std::isalpha(alias_path[i])) {
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
    _sc_vec[_server_i].setAlias(path, alias_path);
    return 0;
}

int ConfigParser::parseIndex(std::string path) {
    std::string find_word = ";\n";
    size_t pos = _raw.find(find_word, _i);
    if (pos == std::string::npos) {
        std::cout << "find() in parseIndex failed" << std::endl;
        return 1;
    }
    std::string index = _raw.substr(_i, pos - _i);
    size_t len = index.length();
    size_t i = 0;
    while (i < len) {
        if (std::isalpha(index[i]) == 0 && index[i] != '.') {
            std::cout << "index invalid" << std::endl;
            return 1;
        }
        i++;
    }
    _i = pos + find_word.length();
    _sc_vec[_server_i].setIndex(path, index);
    return 0;
}

int ConfigParser::parseAllowedMethod(std::string path) {
    std::string find_word = ";\n";
    size_t pos = _raw.find(find_word, _i);
    if (pos == std::string::npos) {
        std::cout << "find() in parseAllowedMethod failed" << std::endl;
        return 1;
    }
    std::string allowed_method = _raw.substr(_i, pos - _i);
    size_t len = allowed_method.length();
    size_t i = 0;
    size_t j;
    std::vector<std::string> method_vec;
    bool end_flag = false;
    while (i < len && end_flag == false) {
        std::string separator = " ";
        j = 0;
        while (j < 3) {
            if (allowed_method.find(_allowed_methods[j], i) == i) {
                i += _allowed_methods[j].length();
                break;
            }
            j++;
        }
        if (j == 3) {
            std::cout << "allowed_method invalid" << std::endl;
            return 1;
        }

        // j = 0;
        // while (j < 3 && i < len && end_flag == false) {
        //     if (allowed_method.find(_allowed_methods[j], i) == i) {
        //         method_vec.push_back(_allowed_methods[j]);
        //         i += _allowed_methods[j].length();
        //         if (allowed_method[i] == ' ') {
        //             i++;
        //             break;
        //         }
        //         else if (!allowed_method[i]) {
        //             end_flag = true;
        //             i--;
        //             break;
        //         }
        //     }
        //     j++;
        //     if (allowed_method[i] != 'G' && allowed_method[i] != 'P' && allowed_method[i] != 'D') {
        //         std::cout << "allowed_method invalid" << std::endl;
        //         return 1;
        //     }
        //     // j = -1;
        // }
        // // i++;
    }
    _i = pos + find_word.length();
    _sc_vec[_server_i].setAllowedMethod(path, method_vec);
    // _sc[_server_index].setAllowedMethod(path, method_vec);
    return 0;
}

int ConfigParser::parseArrangedLocationDirectives(std::string path) {
    size_t i = 0;
    std::string find_word = "\t\t";
    size_t pos;
    while (_raw[_i]) {
        pos = _raw.find(find_word, _i);
        if (pos != _i) {
            find_word = "\t}\n";
            pos = _raw.find(find_word, _i);
            if (pos == _i) {
                _i = pos + find_word.length();
                std::cout << "parseArrangedLocationDirectives successed" << std::endl;
                break;
            }
            std::cout << "find() in parseArrangedLocationDirectives failed" << std::endl;
            return 1;
        }
        // _i = pos + find_word.length();
        if (_raw.find(_searched_location_directives[i], _i) == _i) {
            _i += _searched_location_directives[i].length();
            if (_searched_location_directives[i] == _searched_location_directives[0]) { // "\t\tallowed_method "
                if (parseAllowedMethod(path) != 0) {
                    std::cout << "parseAllowedMethod() failed" << std::endl;
                    return 1;
                }
            } else if (_searched_location_directives[i] == _searched_location_directives[1]) { // "\t\tindex "
                if (parseIndex(path) != 0) {
                    std::cout << "parseIndex() failed" << std::endl;
                    return 1;
                }
            } else if (_searched_location_directives[i] == _searched_location_directives[2]) { // "\t\talias "
                if (parseAlias(path) != 0) {
                    std::cout << "parseAlias() failed" << std::endl;
                    return 1;
                }
            } else if (_searched_location_directives[i] == _searched_location_directives[3]) { // "\t\tautoindex on"
                if (parseAutoIndex(path) != 0) {
                    std::cout << "parseAutoIndex() failed" << std::endl;
                    return 1;
                }
            } else if (_searched_location_directives[i] == _searched_location_directives[4]) { // "\t\treturn "
                if (parseReturn(path) != 0) {
                    std::cout << "parseReturn() failed" << std::endl;
                    return 1;
                }
            } else if (_searched_location_directives[i] == _searched_location_directives[5]) { // "\t\tupload_place "
                if (parseUploadPlace(path) != 0) {
                    std::cout << "parseUploadPlace() failed" << std::endl;
                    return 1;
                }
            } else if (_searched_location_directives[i] == _searched_location_directives[6]) { // "\t\textension "
                if (parseExtension(path) != 0) {
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
    size_t pos = _raw.find(find_word, _i);
    if (pos == std::string::npos) {
        std::cout << "find() in parseLocation failed" << std::endl;
        return 1;
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
            if (std::isalpha(path[i])) {
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
    _i = pos + find_word.length();
    if (parseArrangedLocationDirectives(path) != 0) {
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
                if (std::isalnum(default_error_page[i]) == 0 && default_error_page[i] != '.') {
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
        if (std::isalnum(server_name[i++]) == 0) {
            std::cout << "server_name invalid" << std::endl;
            return 1;
        }
    }
    _i = pos + find_word.length();
    _sc_vec[_server_i].setServerName(server_name);
    return 0;
}

int ConfigParser::parseListen() {
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
                if (parseListen() != 0) {
                    std::cout << "parseListen() failed" << std::endl;
                    return 1;
                }
            } else if (_searched_directives[i] == _searched_directives[1]) { // "\tserver_name "
                if (parseServerName() != 0) {
                    std::cout << "parseServerName() failed" << std::endl;
                    return 1;
                }
            } else if (_searched_directives[i] == _searched_directives[2]) { // \tmax_body_size
                if (parseMaxBodySize() != 0) {
                    std::cout << "parseMaxBodySize() failed" << std::endl;
                    return 1;
                }
            } else if (_searched_directives[i] == _searched_directives[3]) { // \terror_page
                if (parseErrorPage() != 0) {
                    std::cout << "parseErrorPage() failed" << std::endl;
                    return 1;
                }
            } else if (_searched_directives[i] == _searched_directives[4]) { // \n\tlocation
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

int ConfigParser::parseServer() {
    std::string find_word = "server {\n";
    if (_raw.find(find_word, _i) == _i) {
        _i += find_word.length();
        _sc_vec.push_back(ServerConfig());
        if (parseArrangedDirectives() != 0) {
            std::cout << "parseArrangedDirectives() failed" << std::endl;
            return 1;
        }
        _server_i++;
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

int main() {
    ConfigParser cp;
    if (cp.parse("../test.conf") != 0) {
        std::cout << "parse() failed" << std::endl;
        return 1;
    }
    std::cout << "parse() successed" << std::endl;
    // for test
    std::cout << "sc_vec_size = " << cp.getScVecSize() << std::endl;
    std::cout << "-----------------------------------------" << std::endl;
    for (size_t i = 0; i < cp.getScVecSize(); i++) {
        std::cout << "_listen[" << i << "] = " << cp.getListen(i) << std::endl;
        std::cout << "_server_name[" << i << "] = " << cp.getServerName(i) << std::endl;
        std::cout << "_max_body_size[" << i << "] = " << cp.getMaxBodySize(i) << std::endl;
        std::cout << "_default_error_page[" << "0" << "][400] = " << cp.getDefaultErrorPage(0)[400] << std::endl;
        std::cout << "_default_error_page[" << "0" << "][404] = " << cp.getDefaultErrorPage(0)[404] << std::endl;
        std::cout << "_default_error_page[" << "0" << "][500] = " << cp.getDefaultErrorPage(0)[500] << std::endl;
        std::cout << "_default_error_page[" << "1" << "][404] = " << cp.getDefaultErrorPage(1)[404] << std::endl;
        // for (std::map<int, std::string>::iterator iter = cp.getDefaultErrorPage(i).begin(); iter != cp.getDefaultErrorPage(i).end(); iter++) {
        //     std::cout << "_default_error_page[" << i << "](key) = " << iter->first << std::endl;
        //     std::cout << "_default_error_page[" << i << "](value) = " << iter->second << std::endl;
        //     break;
        // }
        std::cout << "alias[/] = " << cp.getLocationMap(i)["/"].getAlias() << std::endl;
        std::cout << "alias[/bbb] = " << cp.getLocationMap(i)["/bbb"].getAlias() << std::endl;
        std::cout << "index_file[/] = " << cp.getLocationMap(i)["/"].getIndexFile() << std::endl;
        std::cout << "index_file[/bbb] = " << cp.getLocationMap(i)["/bbb"].getIndexFile() << std::endl;
        std::cout << "upload_place[/] = " << cp.getLocationMap(i)["/"].getUploadPlace() << std::endl;
        // std::cout << cp.getLocationMap(i)["/"].getAllowedMethod().size() << std::endl;
        for (size_t i = 0; i < cp.getLocationMap(i)["/"].getAllowedMethod().size(); i++) {
            std::cout << "allowed_method[/] = " << cp.getLocationMap(i)["/"].getAllowedMethod()[i] << std::endl;
        }
        std::cout << "-----------------------------------------" << std::endl;
    }
    return 0;
}