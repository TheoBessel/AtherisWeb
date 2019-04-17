//
// Created by Theo Bessel on 2019-04-17.
//

#ifndef WEBRENDER_URLGETTER_HPP
#define WEBRENDER_URLGETTER_HPP

#include "../usual/logger.hpp"

class UrlGetter {
public:
    UrlGetter();
    std::string data;
    static size_t debugCallback(char* buf, size_t size, size_t nmemb, void* up);
    void urlFormater(std::string url);
    std::string get(std::string url);

private:

};

#endif //WEBRENDER_URLGETTER_HPP
