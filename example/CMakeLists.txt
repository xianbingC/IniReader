cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_COMPILER /usr/bin/g++)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "{CMAKE_CXX_FLAGS} -wall")

project(Demo LANGUAGES CXX)

add_executable(app main.cpp)

target_include_directories(app PRIVATE ${CMAKE_SOURCE_DIR}/../include)
