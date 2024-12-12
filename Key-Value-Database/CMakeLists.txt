cmake_minimum_required(VERSION 3.29)
project(MemoryRiver_Easy)

set(CMAKE_CXX_STANDARD 20)

add_executable(MemoryRiver_Easy main.cpp
        MemoryRiver.h
        Key-Value-Database.h
        Key-Value-Database.cpp)
