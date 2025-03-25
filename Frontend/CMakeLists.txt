project(Frontend)

add_library(Frontend
        src/Frontend/Lexer.cpp
        src/Frontend/Parser.cpp
)

target_link_libraries(Frontend PUBLIC
    magic_enum::magic_enum
    frozen
)

target_include_directories(Frontend PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(test)