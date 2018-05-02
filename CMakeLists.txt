cmake_minimum_required(VERSION 3.10)
project(MaladIF)

set(CMAKE_CXX_STANDARD 17)

add_executable(MaladIF main.cpp CLI.cpp Database.cpp FileReader.cpp)