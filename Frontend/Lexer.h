#pragma once

#include <filesystem>
#include <fstream>

#include "LexerTypes.h"

class Lexer
{
public:
    explicit Lexer(std::istream& source);
    [[nodiscard]] Token getNextToken();

private:
    Token stringLiteral_();
    Token numericLiteral_();
    Token operator_();
    Token indetLike_();
    void skipWhitespace_();
    void skipOneLineComment_();
    void skipBlockComment();
    char getChar_();
    char peekChar_();
    void consume_();
    [[nodiscard]] bool isOperator_() const;

    bool m_consumed{true};
    char m_currChar{};
    std::istream& m_rSource;
    Localization m_currLocalization{1, 0};
};
