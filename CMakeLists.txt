cmake_minimum_required(VERSION 3.13)
set(PROJECT_NAME "WebRender")

# Core ---------------------------------------------------
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/include)

# Url ----------------------------------------------------
SET(CURL_PATH "usr/local/include/curl")
include_directories(AFTER ${CURL_PATH})

# Compiler -----------------------------------------------

# Render -------------------------------------------------
SET(VULKAN_PATH "/Users/theobessel/VulkanSDK_1.1.97.0/macOS/")
SET(GLFW_PATH "/usr/local/Cellar/glfw/3.2.1")
SET(GLM_PATH "/usr/local/Cellar/glm/0.9.9.4")
include_directories(AFTER ${VULKAN_PATH}/Include)
include_directories(AFTER ${GLFW_PATH}/include)
include_directories(AFTER ${GLM_PATH}/include)
link_directories(${VULKAN_PATH}/Bin)

# Project ------------------------------------------------
project(${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 17)
add_executable(
        ${PROJECT_NAME}
        core/main.cpp
        render/mainRender.hpp
        render/mainRender.cpp
        usual/logger.hpp
        compiler/html/htmlLexer.cpp
        compiler/html/htmlLexer.hpp
        render/mainWindow/mainWindow.cpp
        render/mainWindow/mainWindow.hpp
)

target_link_libraries (${PROJECT_NAME} ${VULKAN_PATH}/lib/libvulkan.1.1.97.dylib ${GLFW_PATH}/lib/libglfw.3.2.dylib)