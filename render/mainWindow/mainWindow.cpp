//
// Created by Theo Bessel on 2019-03-25.
//

#include "mainWindow.hpp"
#include "../../usual/logger.hpp"

#include <GLFW/glfw3.h>
#define GLFW_INCLUDE_VULKAN

#include <vulkan/vulkan.h>

MainWindow::MainWindow() {

}

void MainWindow::initWindow(int WIDTH, int HEIGHT) {
    // Initialisation
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    // Creates window
    window = glfwCreateWindow(WIDTH, HEIGHT, "Render window", nullptr, nullptr);
}

void MainWindow::createInstance() {
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Web Render";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 1, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 1, 0);
    appInfo.apiVersion = VK_API_VERSION_1_1;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;

    createInfo.enabledLayerCount = 0;
    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
    }
}

void MainWindow::initVulkan() {
    createInstance();
}

void MainWindow::mainLoop() {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
   }
}

void MainWindow::cleanup() {
    vkDestroyInstance(instance, nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();
}