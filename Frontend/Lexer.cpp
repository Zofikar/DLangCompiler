#include "Lexer.h"
#include "LexerErrors.h"
#include "Asserts.h"
#include "CommonDefs.h"

#include <array>
#include <ranges>

Lexer::Lexer(std::istream& source) : m_rSource(source)
{
}

Token Lexer::getNextToken()
{
    while (getChar_())
    {
        if (std::isspace(m_currChar))
        {
            consume_();
            continue;
        }
        if (m_currChar == '"')
        {
            return this->stringLiteral_();
        }
        if (std::isdigit(m_currChar))
        {
            return this->numericLiteral_();
        }
        if (m_currChar == '/')
        {
            if (peekChar_() == '/')
            {
                consume_(); // consume the 1st '/'
                getChar_(); // get the 2nd '/'
                consume_(); // consume the 2nd '/'
                this->skipOneLineComment_();
                continue;
            }
            if (peekChar_() == '*')
            {
                consume_(); // consume the 1st '/'
                getChar_(); // get the '*'
                consume_(); // consume the '*'
                this->skipBlockComment();
                continue;
            }
        }
        if (isSymbol())
        {
            return this->symbol_();
        }
        if (std::isalpha(m_currChar))
        {
            return this->indetLike_();
        }
        throw IllegalCharacterError("Unexpected character", this->m_currLocalization);
    }
    return Token{TokenType::eof, "EOF", this->m_currLocalization};
};

Token Lexer::stringLiteral_()
{
    consume_(); // consume opening quote
    std::string buffer{};
    while (getChar_())
    {
        if (m_currChar == '"')
        {
            consume_();
            return Token{TokenType::string_literal, buffer, this->m_currLocalization};
        }
        if (m_currChar == '\\')
        {
            switch (peekChar_())
            {
            case 'n': buffer += '\n'; break;
            case 't': buffer += '\t'; break;
            case 'r': buffer += '\r'; break;
            case '"': buffer += '"'; break;
            case '\\': buffer += '\\'; break;
            case 0: throw UnexpectedEndOfFileError(this->m_currLocalization);
            default: throw IllegalCharacterError("Unexpected escape sequence", this->m_currLocalization);
            }
            consume_();
            getChar_();
            consume_();
            continue;
        }
        if (m_currChar == '\n')
        {
            consume_();
            throw IllegalCharacterError("Unexpected end of line", this->m_currLocalization);
        }
        consume_();
        buffer += m_currChar;
    }
    throw UnexpectedEndOfFileError(this->m_currLocalization);
};


Token Lexer::numericLiteral_()
{
    std::string buffer(1, m_currChar);
    consume_();
    bool isDecimal = false;

    while (getChar_())
    {
        if (m_currChar == '.')
        {
            if (isDecimal)
            {
                consume_();
                throw IllegalCharacterError("Unexpected character", this->m_currLocalization);
            }
            isDecimal = true;
        }
        else if (m_currChar == '_' || m_currChar == '`')
        {
            consume_();
            continue;
        }
        else if (!std::isdigit(m_currChar))
        {
            consume_();
            return Token{TokenType::numeric_literal, buffer, this->m_currLocalization};
        }
        consume_();
        buffer += m_currChar;
    };
    return Token{TokenType::numeric_literal, buffer, this->m_currLocalization};
}

Token Lexer::symbol_()
{
    std::string buffer{m_currChar};
    consume_();
    switch (m_currChar)
    {
    case '&':
    case '|':
    case '+':
    case '-':
    case '/':
        if (peekChar_() == m_currChar)
        {
            getChar_();
            consume_();
            buffer += m_currChar;
        }
    case ':':
    case '*':
    case '%':
    case '?':
    case '!':
    case '=':
    case '^':
        if (peekChar_() == '=')
        {
            getChar_();
            consume_();
            buffer += m_currChar;
        }
        break;
    case '<':
    case '>':
        if (peekChar_() == '=')
        {
            getChar_();
            consume_();
            buffer += m_currChar;
        }
        else if (peekChar_() == m_currChar)
        {
            getChar_();
            consume_();
            buffer += m_currChar;
            if (peekChar_() == '=')
            {
                getChar_();
                consume_();
                buffer += m_currChar;
            }
        }
    default: break;
    }
    return Token{TokenType::symbol, buffer, this->m_currLocalization};
}

Token Lexer::indetLike_()
{
    std::string buffer(1, m_currChar);
    consume_();
    while (getChar_())
    {
        if (std::isspace(m_currChar) || isSymbol())
        {
            break;
        }
        consume_();
        buffer += m_currChar;
        if (std::ranges::find(k_keywords, buffer) != k_keywords.end())
        {
            return Token{TokenType::keyword, buffer, this->m_currLocalization};
        }
    }
    return Token{TokenType::identifier, buffer, this->m_currLocalization};
}

void Lexer::skipWhitespace_()
{
    while (getChar_())
    {
        if (std::isspace(m_currChar))
        {
            consume_();
        }
    }
}

void Lexer::skipOneLineComment_()
{
    while (getChar_())
    {
        consume_();
        if (m_currChar == '\n') [[unlikely]]
        {
            return;
        }
    }
    // EOF is valid, do not consume!
}

void Lexer::skipBlockComment()
{
    while (getChar_())
    {
        consume_(); // consume the '*'
        if (m_currChar == '*' && peekChar_() == '/') [[unlikely]]
        {
            getChar_(); // get the '/'
            consume_(); // consume the '/'
            return;
        }
    }
}

char Lexer::peekChar_()
{
    return static_cast<char>(m_rSource.peek());
};

char Lexer::getChar_()
{
    if (!m_consumed) return m_currChar;
    if (!this->m_rSource.get(m_currChar))
    {
        return 0;
    }
    if (m_currChar == '\n')
    {
        this->m_currLocalization.line++;
        this->m_currLocalization.column = 0;
    }
    else
    {
        this->m_currLocalization.column++;
    }
    m_consumed = false;
    return m_currChar;
}

void Lexer::consume_()
{
    m_consumed = true;
}

bool Lexer::isSymbol() const
{
    switch (m_currChar)
    {
    case '(':
    case ')':
    case '{':
    case '}':
    case '[':
    case ']':
    case ';':
    case ':':
    case ',':
    case '.':
    case '*':
    case '/':        // handled separately as it can be a comment
    case '%':
    case '+':
    case '-':
    case '=':
    case '<':
    case '>':
    case '!':
    case '~':
    case '&':
    case '|':
    case '^':
    case '?' :
        return true;
    default: return false;
    }
};
