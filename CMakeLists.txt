cmake_minimum_required(VERSION 3.9)
project(untitled)

set(CMAKE_CXX_STANDARD 11)

add_executable(untitled main.cpp background.h background.cpp)
target_link_libraries(untitled sfml-graphics sfml-window sfml-system)