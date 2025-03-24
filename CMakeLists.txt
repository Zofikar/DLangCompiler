cmake_minimum_required(VERSION 3.30.0)
project(randomCompiler)

set(CMAKE_CXX_STANDARD 23)

include(CTest)
include(Thirdparty.cmake)

add_executable(randomCompiler
        main.cpp
)

target_link_libraries(randomCompiler PRIVATE
        magic_enum::magic_enum
        Frontend
)

add_subdirectory(Frontend)
