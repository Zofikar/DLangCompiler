#pragma once

#include <Frontend/LexerTypes.h>

#include <stdexcept>

class LexerError : public std::runtime_error
{
public:
    explicit LexerError(std::string const &message, Localization const &localization, bool const recoverable = true)
        : std::runtime_error((localization.toString() + ": " + message).c_str())
        , localization(localization)
        , recoverable(recoverable) {}

    [[nodiscard]] Localization getLocalization() const { return localization; }
    [[nodiscard]] bool isRecoverable() const { return recoverable; }

private:
    Localization const localization;
    bool const recoverable;
};


class IllegalCharacterError : public LexerError
{
public:
    explicit IllegalCharacterError(
        const std::string &message,
        const Localization localization,
        bool const recoverable = true)
        : LexerError(message, localization, recoverable) {}
};

class UnexpectedEndOfFileError : public IllegalCharacterError
{
public:
    explicit UnexpectedEndOfFileError(const Localization localization) : IllegalCharacterError(message, localization, false) {}
private:
    static constexpr auto message = "Unexpected end of file";
};