project(Frontend)

add_library(Frontend
        src/Frontend/Lexer.cpp
        src/Frontend/Parser.cpp
)

target_include_directories(Frontend PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(test)