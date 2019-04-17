//
// Created by Theo Bessel on 2019-04-17.
//

#ifndef WEBRENDER_MAINAPP_HPP
#define WEBRENDER_MAINAPP_HPP


#include <memory>
#include "../../url/UrlGetter.hpp"

class UrlGetter;
class HtmlLexer;
class MainRender;
class MainApp {
public:
    // Code
    MainApp();

private:
    std::unique_ptr<UrlGetter> m_urlHandler;
    std::unique_ptr<HtmlLexer> m_HtmlLexer;
    std::unique_ptr<MainRender> m_render;

};


#endif //WEBRENDER_MAINAPP_HPP
