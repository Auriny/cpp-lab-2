cmake_minimum_required(VERSION 4.0)
project(cpp_lab_2)

set(CMAKE_CXX_STANDARD 20)

add_executable(cpp_lab_2 main.cpp
        module.h
        robot.cpp
        robot.h
        station.cpp
        station.h
        generator_module.h
        archive_module.h
        habitation_module.h
        control_center_module.h
        integrator.h
        keeper.h
        local_computer.h)
