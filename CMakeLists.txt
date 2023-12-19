cmake_minimum_required(VERSION 3.26)
project(PEA2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")
add_executable(PEA2 main.cpp
        Test.cpp
        Test.h
        algorithms/Sa.cpp
        algorithms/Sa.h
        time/Time.cpp
        time/Time.cpp
        algorithms/Ts.h algorithms/Ts.cpp)
