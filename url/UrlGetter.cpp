//
// Created by Theo Bessel on 2019-04-17.
//

#include <curl/curl.h>

#include "UrlGetter.hpp"
#include "../usual/files.hpp"

UrlGetter::UrlGetter() {
}

size_t UrlGetter::debugCallback(char* buf, size_t size, size_t nmemb, void* up) {
    for (int c = 0; c<size*nmemb; c++)
    {
        //usual::clog(buf);
        static_cast<std::string*>(up)->push_back(buf[c]);

    }
    return size*nmemb;
}

void UrlGetter::urlFormater(std::string url) {
    for (size_t pos = url.find(' '); pos != std::string::npos; pos = url.find(' ', pos)) {
        url.replace(pos, 1, "%20");
    }
    usual::clog("Formated url : ", url);
}

std::string UrlGetter::get(std::string url) {
    data = "";
    urlFormater(url);
    CURL *curl;
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &UrlGetter::debugCallback);
    #ifndef NDEBUG
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    #endif
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    usual::clog("Url get success");
    usual::writeFile(data, "/Users/theobessel/Desktop/test.html");
    return data;
}