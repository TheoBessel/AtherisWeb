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
            my_main_window->run();
        } catch (const std::exception& e) {
            usual::clog("[RENDER-ERROR] : ", e.what());
            usual::clog("EXIT_FAILURE : ", EXIT_FAILURE);
        }
         usual::clog("EXIT_SUCCESS : ", EXIT_SUCCESS);
    }
    usual::clog("App initialized : ", appInit);
}