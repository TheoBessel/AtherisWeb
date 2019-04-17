//
// Created by Theo Bessel on 2019-04-17.
//

//#include <memory>

#include "MainApp.hpp"
#include "../../render/mainRender.hpp"
#include "../../compiler/html/htmlLexer.hpp"
#include "../../url/UrlGetter.hpp"

MainApp::MainApp() {
    m_urlHandler = std::make_unique<UrlGetter>();
    m_HtmlLexer = std::make_unique<HtmlLexer>();
    //m_render = std::make_unique<MainRender>();
    //m_urlHandler->get("https://www.google.fr/search?q=moteur");
    m_urlHandler->get("https://curl.haxx.se/libcurl/c/CURLOPT_WRITEDATA.html");
}