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
    comparison_operator,
    arithmetic_operator,
    bitwise_operator,
    logical_operator,
    assignment_operator,
    inc_operator,
    dec_operator,
    un_operator
  };

static_assert(TokenType::identifier == TokenType::type); // Just to indicate that token will take type as identifier

struct Localization {
    size_t line;
    size_t column;
    std::string file;

    [[nodiscard]] std::string toString() const { return file + "(" + std::to_string(line) + ":" + std::to_string(column) + ")"; }
};

inline std::ostream& operator<<(std::ostream& os, const Localization& obj)
{
    os << obj.toString();
    return os;
}


struct Token {
    TokenType type{TokenType::eof};
    std::string value;
    Localization localization{0, 0, ""};
};