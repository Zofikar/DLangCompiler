include(FetchContent)

FetchContent_Declare(
        magic_enum
        GIT_REPOSITORY https://github.com/Neargye/magic_enum.git
        GIT_TAG        v0.9.7
)

FetchContent_MakeAvailable(magic_enum)

include_directories(
        ${magic_enum_SOURCE_DIR}/include
        ${frozen_SOURCE_DIR}/include
)