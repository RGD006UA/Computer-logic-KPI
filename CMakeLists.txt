cmake_minimum_required(VERSION 3.21)
project(CL)

set(CMAKE_CXX_STANDARD 23)

add_executable(CL main.cpp simplification.cpp mura.cpp mili.cpp operations.cpp
        bool_logic.cpp
)