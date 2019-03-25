//
// Created by Theo Bessel on 2019-03-25.
//

#ifndef WEBRENDER_MAINWINDOW_HPP
#define WEBRENDER_MAINWINDOW_HPP

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

class MainWindow {
public:
    MainWindow();
    void run(int WIDTH, int HEIGHT) {
        initWindow(WIDTH, HEIGHT);
        initVulkan();
        mainLoop();
        cleanup();
    }

private:
    GLFWwindow* window;
    VkInstance instance;

    void initWindow(int WIDTH, int HEIGHT);
    void initVulkan();
    void mainLoop();
    void cleanup();
    void createInstance();
};


#endif //WEBRENDER_MAINWINDOW_HPP
