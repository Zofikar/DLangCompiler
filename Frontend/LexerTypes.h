#pragma once

#include <string>

enum class TokenType{
    eof = -1,
    string_literal,
    numeric_literal,
    identifier,
    type = identifier,
    keyword,
    symbol,
  };

static_assert(TokenType::identifier == TokenType::type); // Just to indicate that token will take type as identifier

struct Localization {
    size_t line;
    size_t column;
};

inline std::ostream& operator<<(std::ostream& os, const Localization& obj)
{
    os << std::to_string(obj.line) << ":" << std::to_string(obj.column);
    return os;
}


struct Token {
    TokenType type{TokenType::eof};
    std::string value;
    Localization localization{0,0};
};