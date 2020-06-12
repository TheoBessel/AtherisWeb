cmake_minimum_required(VERSION 3.13)
set(PROJECT_NAME "WebRender")
set(CMAKE_CXX_STANDARD 17)
project(${PROJECT_NAME})

# Core ---------------------------------------------------
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/include)
# Default LLVM
if (APPLE)
    link_directories("/usr/local/opt/llvm/lib")
endif ()

# Url ----------------------------------------------------
find_package(curl)

# Compiler -----------------------------------------------

# Render -------------------------------------------------
find_package(GLFW3 3.3 REQUIRED)
find_package(vulkan REQUIRED)
find_package(glm REQUIRED)

if (GLFW_FOUND)
    message(STATUS "Packages: GLFW found")
    include_directories(${GLFW_INCLUDE_DIRS})
    link_libraries(${GLFW_LIBRARIES})
endif()

# Project ------------------------------------------------
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
        usual/files.hpp
        url/UrlGetter.cpp
        url/UrlGetter.hpp
        core/mainApp/MainApp.cpp
        core/mainApp/MainApp.hpp
)
#$<$<PLATFORM_ID:Darwin>:c++fs>
target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan curl glm glfw )
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
