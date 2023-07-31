cmake_minimum_required(VERSION 3.21)
project(yadro_test)

set(CMAKE_CXX_STANDARD 20)

add_executable(yadro_test main.cpp file_reader.cpp file_reader.h config.cpp config.h mysort.cpp mysort.h test/main_test.cpp)
