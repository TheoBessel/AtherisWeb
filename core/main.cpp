#include "../render/mainRender.hpp"
#include "../compiler/html/htmlLexer.hpp"

int main() {
    auto my_app = std::make_unique<MainRender>();
    auto my_html_lexer = std::make_unique<HtmlLexer>();
    return 0;
}