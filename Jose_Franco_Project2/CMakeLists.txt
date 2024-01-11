cmake_minimum_required(VERSION 3.26)
project(sdl_test)

set(CMAKE_CXX_STANDARD 20)


set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_modules)
set(SDL2_PATH "C:/Users/jfran/Downloads/SDL2-devel-2.28.5-mingw/SDL2-2.28.5/x86_64-w64-mingw32")
find_package(SDL2 REQUIRED)

add_executable(sdl_test main.cpp
        snake.cpp
        snake.h
        pong.h
        pong.cpp
)

include_directories(${SDL2_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY})
