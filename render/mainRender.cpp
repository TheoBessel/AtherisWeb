//
// Created by Theo Bessel on 2019-03-25.
//

#include "mainRender.hpp"
#include "mainWindow/mainWindow.hpp"
#include "../usual/logger.hpp"

MainRender::MainRender() {
    bool appInit = false;
    bool windowInit = true;
    if(windowInit){
        appInit = true;
        auto my_main_window = std::make_unique<MainWindow>();
        try {
            my_main_window->run(640, 360);
        } catch (const std::exception& e) {
            usual::clog(e.what());
            usual::plog(EXIT_FAILURE);
        }

         usual::plog(EXIT_SUCCESS);
    }
    usual::plog("App initialized : ", appInit);
}