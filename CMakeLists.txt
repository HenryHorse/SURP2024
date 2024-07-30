cmake_minimum_required(VERSION 3.10)


project(SURP2024)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(hello_world hello_world.cpp)

find_package(Boost 1.83 REQUIRED COMPONENTS graph)

if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(test test.cpp
            graph.cpp
            graph.h
            random_trees.cpp
            random_trees.h)
    target_link_libraries(test ${Boost_LIBRARIES})
else()
    message(FATAL_ERROR "Could not find Boost")
endif()