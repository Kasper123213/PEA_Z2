cmake_minimum_required(VERSION 3.26)
project(PEA2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")
add_executable(PEA2 src/main.cpp
        src/Test.cpp
        src/Test.h
        src/algorithms/Sa.cpp
        src/algorithms/Sa.h
        src/time/Time.cpp
        src/time/Time.cpp
        src/algorithms/Ts.h src/algorithms/Ts.cpp)
