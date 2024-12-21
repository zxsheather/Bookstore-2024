cmake_minimum_required(VERSION 3.29)
project(BookStore2024)

set(CMAKE_CXX_STANDARD 20)

add_executable(code main.cpp
        src/Key-Value-Database.cpp
        src/Utils.cpp
)