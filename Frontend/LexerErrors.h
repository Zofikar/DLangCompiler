#pragma once

#include <stdexcept>

#include "LexerTypes.h"

class LexerError : public std::runtime_error
{
public:
    explicit LexerError(const std::string &message, const Localization localization) : std::runtime_error(message), localization(localization) {}
    [[nodiscard]] Localization getLocalization() const { return localization; }
    [[nodiscard]] const char *what() const noexcept override
    {
        return (std::string(std::runtime_error::what()) + " at line " + std::to_string(localization.line) + " column " + std::to_string(localization.column)).c_str();
    }
private:
    const Localization localization;
};


class IllegalCharacterError : LexerError
{
public:
    explicit IllegalCharacterError(const std::string &message, const Localization localization) : LexerError(message, localization) {}
};

class UnexpectedEndOfFileError : IllegalCharacterError
{
public:
    explicit UnexpectedEndOfFileError(const Localization localization) : IllegalCharacterError(message, localization) {}
private:
    static constexpr auto message = "Unexpected end of file";
};