#pragma once

#include <array>
#include <string_view>

inline constexpr std::array k_keywords = {
    std::string_view{"if"},
    std::string_view{"else"}, // conditional
    std::string_view{"for"},
    std::string_view{"while"}, // loops
    std::string_view{"return"},
    std::string_view{"break"},
    std::string_view{"continue"}, // control flow
    std::string_view{"struct"},
    std::string_view{"trait"},
    std::string_view{"impl"},
    std::string_view{"enum"}, // user defined types
    std::string_view{"mut"}, // mutability
    std::string_view{"type"}, // type alias
};


inline constexpr std::array k_built_in_types = {
    std::string_view{"int8"},
    std::string_view{"int16"},
    std::string_view{"int32"},
    std::string_view{"int64"},
    std::string_view{"uint8"},
    std::string_view{"uint16"},
    std::string_view{"uint32"},
    std::string_view{"uint64"},
    std::string_view{"float32"},
    std::string_view{"float64"},
    std::string_view{"bool"},
    std::string_view{"char"},
    std::string_view{"string"},
    std::string_view{"void"},
};

