cmake_minimum_required(VERSION 3.22)
project(new_json)

set(CMAKE_CXX_STANDARD 14)

add_executable(new_json srcs/main.cpp
                srcs/json.cpp srcs/parse.cpp
				includes/json.hpp includes/parse.hpp)
