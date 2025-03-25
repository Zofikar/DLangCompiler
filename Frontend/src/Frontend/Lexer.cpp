#include <Frontend/Lexer.h>
#include <Frontend/LexerErrors.h>
#include <Frontend/CommonDefs.h>
#include <Frontend/PrefixTree.h>

#include <frozen/unordered_set.h>

#include <array>
#include <ranges>

using namespace std::string_view_literals;

inline constexpr std::array k_comparisonOperators = {"<"sv, ">"sv, "<="sv, ">="sv, "=="sv, "!="sv};
inline constexpr std::array k_arithmeticOperators = {"*"sv, "/"sv, "%"sv, "+"sv, "-"sv};
inline constexpr std::array k_bitwiseOperators = {"&"sv, "|"sv, "^"sv, "<<"sv, ">>"sv};
inline constexpr std::array k_logicalOperators = {"&&"sv, "||"sv};
inline constexpr std::array k_assignmentOperators = {
    "="sv,
    ":="sv,
    "*="sv,
    "/="sv,
    "%="sv,
    "+="sv,
    "-="sv,
    "&="sv,
    "|="sv,
    "^="sv,
    "<<="sv,
    ">>="sv,
    "&&="sv,
    "||="sv
};
inline constexpr auto k_incOperator = "++"sv;
inline constexpr auto k_decOperator = "--"sv;
inline constexpr std::array k_unaryOperators = {"!"sv, "~"sv}; // omit - as it's arithmetic operator also, will handle conflict later

const PrefixTree k_operatorsTree = ([] {
    PrefixTree tree{};
    tree.insert(k_comparisonOperators, TokenType::comparison_operator);
    tree.insert(k_arithmeticOperators, TokenType::arithmetic_operator);
    tree.insert(k_bitwiseOperators, TokenType::bitwise_operator);
    tree.insert(k_logicalOperators, TokenType::logical_operator);
    tree.insert(k_assignmentOperators, TokenType::assignment_operator);
    tree.insert(k_incOperator, TokenType::inc_operator);
    tree.insert(k_decOperator, TokenType::dec_operator);
    tree.insert(k_unaryOperators, TokenType::un_operator);
    return tree;
})();

inline constexpr std::array kf_symbols{'{','}','(',')','[',']',',',';','.'};
inline constexpr frozen::unordered_set<char, kf_symbols.size()> k_symbols{'{','}','(',')','[',']',',',';','.'};


Lexer::Lexer(std::istream &source, std::string const &file)
    : m_rSource(source)
    , m_currLocalization(1, 0, file) {}

Token Lexer::getNextToken()
{
    while (getChar_())
    {
        if (std::isspace(m_currChar))
        {
            consume_();
            continue;
        }
        if (m_currChar == '"') { return this->stringLiteral_(); }
        if (std::isdigit(m_currChar)) { return this->numericLiteral_(); }
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
        if (isOperator_(m_currChar)) { return this->operator_(); }
        if (std::isalpha(m_currChar)) { return this->indetLike_(); }
        consume_();
        if (isValidSymbol_(m_currChar))
            return Token{TokenType::symbol, std::string(1, m_currChar), this->m_currLocalization};
        throw IllegalCharacterError("Unexpected character: " + m_currChar, this->m_currLocalization);
    }
    return Token{TokenType::eof, "", this->m_currLocalization};
}

void Lexer::recoverError()
{
    while (getChar_())
    {
        if (m_currChar == ';' || m_currChar == '}') { return; }
        consume_();
    }
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
                case 'n': buffer += '\n';
                    break;
                case 't': buffer += '\t';
                    break;
                case 'r': buffer += '\r';
                    break;
                case '"': buffer += '"';
                    break;
                case '\\': buffer += '\\';
                    break;
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
                throw IllegalCharacterError("Unexpected character: ", this->m_currLocalization);
            }
            isDecimal = true;
        } else if (m_currChar == '_' || m_currChar == '`')
        {
            consume_();
            continue;
        } else if (!std::isdigit(m_currChar))
        {
            return Token{TokenType::numeric_literal, buffer, this->m_currLocalization};
        }
        consume_();
        buffer += m_currChar;
    };
    return Token{TokenType::numeric_literal, buffer, this->m_currLocalization};
}

Token Lexer::operator_()
{
    std::string buffer{m_currChar};
    consume_();
    while (getChar_())
    {
        auto it = k_operatorsTree.find(buffer).value();
        auto it2 = it.find(m_currChar);
        if(it.oType.has_value() && !it2)
        {
            return Token{it.oType.value(), buffer, this->m_currLocalization};
        }
        if(it2)
        {
            buffer += m_currChar;
            consume_();
        } else
        {
            throw IllegalCharacterError("Unexpected character", this->m_currLocalization);
        }
    }
    throw UnexpectedEndOfFileError(this->m_currLocalization);
}

Token Lexer::indetLike_()
{
    std::string buffer(1, m_currChar);
    consume_();
    while (getChar_())
    {
        if (std::isspace(m_currChar) || isOperator_(m_currChar) || isValidSymbol_(m_currChar)) { break; }
        consume_();
        buffer += m_currChar;
        if (std::ranges::find(k_keywords, buffer) != k_keywords.end() && !std::isalpha(peekChar_()))
        {
            return Token{TokenType::keyword, buffer, this->m_currLocalization};
        }
    }
    return Token{TokenType::identifier, buffer, this->m_currLocalization};
}

void Lexer::skipWhitespace_() { while (getChar_()) { if (std::isspace(m_currChar)) { consume_(); } } }

void Lexer::skipOneLineComment_()
{
    while (getChar_())
    {
        consume_();
        if (m_currChar == '\n')
        [[unlikely]]
        {
            return;
        }
    }
}

void Lexer::skipBlockComment()
{
    while (getChar_())
    {
        consume_(); // consume the '*'
        if (m_currChar == '*' && peekChar_() == '/')
        [[unlikely]]
        {
            getChar_(); // get the '/'
            consume_(); // consume the '/'
            return;
        }
    }
    throw UnexpectedEndOfFileError(this->m_currLocalization);
}

char Lexer::peekChar_() { return static_cast<char>(m_rSource.peek()); };

char Lexer::getChar_()
{
    if (!m_consumed) return m_currChar;
    if (!this->m_rSource.get(m_currChar)) { return 0; }
    if (m_currChar == '\n')
    {
        this->m_currLocalization.line++;
        this->m_currLocalization.column = 0;
    } else { this->m_currLocalization.column++; }
    m_consumed = false;
    return m_currChar;
}

void Lexer::consume_() { m_consumed = true; }

bool Lexer::isValidSymbol_(char const c)
{
    return k_symbols.contains(c);
}

bool Lexer::isOperator_(char const c)
{
    return k_operatorsTree.find(c).has_value();
};
