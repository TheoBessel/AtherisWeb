//
// Created by Theo Bessel on 2019-03-25.
//

#ifndef WEBRENDER_HTMLLEXER_HPP
#define WEBRENDER_HTMLLEXER_HPP

#include "../../usual/files.hpp"
#include "../../usual/logger.hpp"
#include <filesystem>

namespace fs = std::filesystem;

class HtmlLexer {
public:
    HtmlLexer();
    std::string getfile(fs::path path);
    void tokenize(const std::string& file);
};


#endif //WEBRENDER_HTMLLEXER_HPP
