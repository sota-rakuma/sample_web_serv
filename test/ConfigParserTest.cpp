#include "Test.hpp"

void ConfigParserTest() {
    ConfigParser cp;
    if (cp.parse("../parser/zzz.conf") != 0) {
        std::cout << "parse() failed" << std::endl;
        return;
    }
    std::cout << "parse() successed" << std::endl;
    std::cout << "sc_vec_size = " << cp.getScVecSize() << std::endl;
    std::cout << "-----------------------------------------" << std::endl;
    for (size_t i = 0; i < cp.getScVecSize(); i++) {
        std::cout << "_sc_vec[" << i << "]._listen = " << cp.getListen(i) << std::endl;
        std::cout << "_sc_vec[" << i << "]._server_name = " << cp.getServerName(i) << std::endl;
        std::cout << "_sc_vec[" << i << "]._max_body_size = " << cp.getMaxBodySize(i) << std::endl;
        std::cout << "_sc_vec[" << "0" << "]._default_error_page[400] = " << cp.getDefaultErrorPage(0)[400] << std::endl;
        std::cout << "_sc_vec[" << "0" << "]._default_error_page[404] = " << cp.getDefaultErrorPage(0)[404] << std::endl;
        std::cout << "_default_error_page[" << "0" << "][500] = " << cp.getDefaultErrorPage(0)[500] << std::endl;
        std::cout << "_default_error_page[" << "1" << "][404] = " << cp.getDefaultErrorPage(1)[404] << std::endl;
        std::cout << "_sc_vec[" << i << "].location['/'].alias = " << cp.getLocationMap(i)["/"].getAlias() << std::endl;
        std::cout << "_sc_vec[" << i << "].location['/bbb'].alias = " << cp.getLocationMap(i)["/bbb"].getAlias() << std::endl;
        std::cout << "_sc_vec[" << i << "].location['/'].index_file = " << cp.getLocationMap(i)["/"].getIndexFile() << std::endl;
        std::cout << "_sc_vec[" << i << "].location['/bbb'].index_file = " << cp.getLocationMap(i)["/bbb"].getIndexFile() << std::endl;
        std::cout << "_sc_vec[" << i << "].location['/'].upload_place = " << cp.getLocationMap(i)["/"].getUploadPlace() << std::endl;
        std::cout << "_sc_vec[" << i << "].location['/'].autoindex = " << cp.getLocationMap(i)["/"].getAutoIndex() << std::endl;
        std::cout << "location['/'].allowedmethod.size = " << cp.getLocationMap(i)["/"].getAllowedMethod().size() << std::endl;

        const std::map<std::string, bool> &tmp =  cp.getLocationMap(i)["/"].getAllowedMethod();
        for (std::map<std::string, bool>::const_iterator it = tmp.begin();
            it != tmp.end();
            it++) {
            std::cout
            << "_sc_vec[" << i << "].location['/'].allowed_method[" << it->first << "] = "
            << it->second << std::endl;
        }
        for (size_t j = 0; j < cp.getLocationMap(i)["/"].getCgiExtensions().size(); j++) {
            std::cout << "_sc_vec[" << i << "].location['/'].extension[" << j << "] = " << cp.getLocationMap(i)["/"].getCgiExtensions()[j] << std::endl;
        }
        std::cout << "_sc_vec[" << i << "].location['/'].return.key = " << cp.getLocationMap(i)["/"].getReturn().first << std::endl;
        std::cout << "_sc_vec[" << i << "].location['/'].return.value = " << cp.getLocationMap(i)["/"].getReturn().second << std::endl;
        std::cout << "_sc_vec[" << i << "].location['/bbb'].return.key = " << cp.getLocationMap(i)["/bbb"].getReturn().first << std::endl;
        std::cout << "_sc_vec[" << i << "].location['/bbb'].return.value = " << cp.getLocationMap(i)["/bbb"].getReturn().second << std::endl;
        std::cout << "-----------------------------------------" << std::endl;
    }
}