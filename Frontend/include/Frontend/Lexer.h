#pragma once

#include <Frontend/LexerTypes.h>

#include <filesystem>
#include <fstream>

class Lexer
{
public:
    explicit Lexer(std::istream& source, std::string const& file = "unknown");
    [[nodiscard]] Token getNextToken();
    void recoverError();

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
    [[nodiscard]] static bool isValidSymbol_(char c);
    [[nodiscard]] static bool isOperator_(char c) ;

    bool m_consumed{true};
    char m_currChar{};
    std::istream& m_rSource;
    Localization m_currLocalization;
};
