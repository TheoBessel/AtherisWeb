#include "../render/mainRender.hpp"
#include "../compiler/html/htmlLexer.hpp"
#include "mainApp/MainApp.hpp"

int main() {
    auto my_app = std::make_unique<MainApp>();
    return 0;
}