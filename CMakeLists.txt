cmake_minimum_required(VERSION 3.29)
project(BookStore2024)

set(CMAKE_CXX_STANDARD 20)

# 若是 Clang 或 GCC，设置 Debug 时的编译和链接选项
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=address")

add_executable(code main.cpp
        src/Key-Value-Database.cpp
        src/Utils.cpp
)