//
// Created by Theo Bessel on 2019-03-25.
//

#include "htmlLexer.hpp"
#include "../../usual/logger.hpp"
#include "../../usual/files.hpp"

HtmlLexer::HtmlLexer() {
    bool lexerInit = false;
    bool lexingSuccess = true;
    if(lexingSuccess){
        lexerInit = true;
    }
    usual::clog("Lexing achieved : ", lexerInit);
}