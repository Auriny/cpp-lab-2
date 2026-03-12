cmake_minimum_required(VERSION 4.0)
project(cpp_lab_2)

set(CMAKE_CXX_STANDARD 20)

add_executable(cpp_lab_2 main.cpp
        module.h
        module.cpp
        robot.cpp
        robot.h
        station.cpp
        station.h)
