cmake_minimum_required(VERSION 3.30.0)
project(randomCompiler)

include(Thirdparty.cmake)

set(CMAKE_CXX_STANDARD 23)

add_executable(randomCompiler
        main.cpp
        Frontend/Lexer.cpp
)

target_include_directories(randomCompiler PRIVATE Helpers Frontend)