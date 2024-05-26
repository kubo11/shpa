cmake_minimum_required(VERSION 3.25)
project(shpa)

set(CMAKE_CXX_STANDARD 20)

add_executable(shpa main.cpp
        graph.cpp
        graph.h
        fast_set.cpp
        fast_set.h
        path.cpp
        path.h
        shpa.cpp
        shpa.h
)

add_executable(generator generator.cpp)