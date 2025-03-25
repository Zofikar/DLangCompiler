#include <Frontend/Lexer.h>
#include <Frontend/LexerErrors.h>
#include <Frontend/LexerTypes.h>

#include <gtest/gtest.h>
#include <magic_enum/magic_enum.hpp>

TEST(Lexer, stringLiteral)
{
    std::string const expected = "test";
    std::istringstream is('"' + expected + '"');
    Lexer lexer(is);
    Token token = lexer.getNextToken();
    EXPECT_EQ(token.type, TokenType::string_literal);
    EXPECT_EQ(token.value, expected);
}

TEST(Lexer, numericLiteral_Int)
{
    std::string const expected = "123";
    std::istringstream is(expected);
    Lexer lexer(is);
    Token token = lexer.getNextToken();
    EXPECT_EQ(token.type, TokenType::numeric_literal);
    EXPECT_EQ(token.value, expected);
}

TEST(Lexer, numericLiteral_Float)
{
    std::string const expected = "123.321";
    std::istringstream is(expected);
    Lexer lexer(is);
    Token token = lexer.getNextToken();
    EXPECT_EQ(token.type, TokenType::numeric_literal);
    EXPECT_EQ(token.value, expected);
}

TEST(Lexer, assignmentOperator)
{
    std::string const expected = ":=";
    std::istringstream is(expected + ' ');
    Lexer lexer(is);
    Token token = lexer.getNextToken();
    EXPECT_EQ(token.type, TokenType::assignment_operator);
    EXPECT_EQ(token.value, expected);
}

TEST(Lexer, unaryOperator)
{
    std::string const expected = "!";
    std::istringstream is(expected + ' ');
    Lexer lexer(is);
    Token token = lexer.getNextToken();
    EXPECT_EQ(token.type, TokenType::un_operator);
    EXPECT_EQ(token.value, expected);
}

TEST(Lexer, incOperator)
{
    std::string const expected = "++";
    std::istringstream is(expected + ' ');
    Lexer lexer(is);
    Token token = lexer.getNextToken();
    EXPECT_EQ(token.type, TokenType::inc_operator);
    EXPECT_EQ(token.value, expected);
}

TEST(Lexer, decOperator)
{
    std::string const expected = "--";
    std::istringstream is(expected + ' ');
    Lexer lexer(is);
    Token token = lexer.getNextToken();
    EXPECT_EQ(token.type, TokenType::dec_operator);
    EXPECT_EQ(token.value, expected);
}

TEST(Lexer, logicalOperator)
{
    std::string const expected = "&&";
    std::istringstream is(expected + ' ');
    Lexer lexer(is);
    Token token = lexer.getNextToken();
    EXPECT_EQ(token.type, TokenType::logical_operator);
    EXPECT_EQ(token.value, expected);
}

TEST(Lexer, bitwiseOperator)
{
    std::string const expected = "&";
    std::istringstream is(expected + ' ');
    Lexer lexer(is);
    Token token = lexer.getNextToken();
    EXPECT_EQ(token.type, TokenType::bitwise_operator);
    EXPECT_EQ(token.value, expected);
}

TEST(Lexer, arithmeticOperator)
{
    std::string const expected = "+";
    std::istringstream is(expected + ' ');
    Lexer lexer(is);
    Token token = lexer.getNextToken();
    EXPECT_EQ(token.type, TokenType::arithmetic_operator);
    EXPECT_EQ(token.value, expected);
}

TEST(Lexer, comparisonOperator)
{
    std::string const expected = "==";
    std::istringstream is(expected + ' ');
    Lexer lexer(is);
    Token token = lexer.getNextToken();
    EXPECT_EQ(token.type, TokenType::comparison_operator);
    EXPECT_EQ(token.value, expected);
}

TEST(Lexer, identifier)
{
    std::string const expected = "identifier";
    std::istringstream is(expected + ' ');
    Lexer lexer(is);
    Token token = lexer.getNextToken();
    EXPECT_EQ(token.type, TokenType::identifier);
    EXPECT_EQ(token.value, expected);
}

TEST(Lexer, keyword)
{
    std::string const expected = "impl";
    std::istringstream is(expected + ' ');
    Lexer lexer(is);
    Token token = lexer.getNextToken();
    EXPECT_EQ(token.type, TokenType::keyword);
    EXPECT_EQ(token.value, expected);
}

TEST(Lexer, symbol)
{
    std::string const expected = "(";
    std::istringstream is(expected + ' ');
    Lexer lexer(is);
    Token token = lexer.getNextToken();
    EXPECT_EQ(token.type, TokenType::symbol);
    EXPECT_EQ(token.value, expected);
}

TEST(Lexer, ifStatement)
{
    std::istringstream is("if(a==2){a++;}");
    std::array expectedTokens = {
        TokenType::keyword,
        TokenType::symbol,
        TokenType::identifier,
        TokenType::comparison_operator,
        TokenType::numeric_literal,
        TokenType::symbol,
        TokenType::symbol,
        TokenType::identifier,
        TokenType::inc_operator,
        TokenType::symbol,
        TokenType::symbol,
        TokenType::eof
    };
    Lexer lexer(is);
    for (auto const &expected: expectedTokens)
    {
        auto const token = lexer.getNextToken();
        EXPECT_EQ(expected, token.type) << "Expected " << magic_enum::enum_name(expected) << " but got " <<
 magic_enum::enum_name(token.type) << " with value " << token.value;
    }
}

TEST(Lexer, ifElseStatement)
{
    std::istringstream is("if(a==2){a++;}else{a--;}");
    std::array expectedTokens = {
        TokenType::keyword,
        TokenType::symbol,
        TokenType::identifier,
        TokenType::comparison_operator,
        TokenType::numeric_literal,
        TokenType::symbol,
        TokenType::symbol,
        TokenType::identifier,
        TokenType::inc_operator,
        TokenType::symbol,
        TokenType::symbol,
        TokenType::keyword,
        TokenType::symbol,
        TokenType::identifier,
        TokenType::dec_operator,
        TokenType::symbol,
        TokenType::symbol,
        TokenType::eof
    };
    Lexer lexer(is);
    for (auto const &expected: expectedTokens)
    {
        auto const token = lexer.getNextToken();
        EXPECT_EQ(expected, token.type) << "Expected " << magic_enum::enum_name(expected) << " but got " <<
 magic_enum::enum_name(token.type) << " with value " << token.value;
    }
}

TEST(Lexer, unterminatedBlockComment)
{
    std::istringstream is("/* unterminated comment");
    Lexer lexer(is);
    EXPECT_THROW(lexer.getNextToken(), UnexpectedEndOfFileError);
}

TEST(Lexer, singleLineComment)
{
    std::istringstream is("// this is a comment\nx := 42;");
    std::array expectedTokens = {
        TokenType::identifier,
        TokenType::assignment_operator,
        TokenType::numeric_literal,
        TokenType::symbol,
        TokenType::eof
    };
    Lexer lexer(is);
    for (auto const &expected: expectedTokens)
    {
        auto const token = lexer.getNextToken();
        EXPECT_EQ(expected, token.type) << "Expected " << magic_enum::enum_name(expected) << " but got " <<
 magic_enum::enum_name(token.type) << " with value " << token.value;
    }
}

TEST(Lexer, blockComment)
{
    std::istringstream is("/* this is a comment */  x := 42;");
    std::array expectedTokens = {
        TokenType::identifier,
        TokenType::assignment_operator,
        TokenType::numeric_literal,
        TokenType::symbol,
        TokenType::eof
    };
    Lexer lexer(is);
    for (auto const &expected: expectedTokens)
    {
        auto const token = lexer.getNextToken();
        EXPECT_EQ(expected, token.type) << "Expected " << magic_enum::enum_name(expected) << " but got " <<
 magic_enum::enum_name(token.type) << " with value " << token.value;
    }
}

TEST(Lexer, stringLiteralWithNewline)
{
    std::istringstream is("\"string with newline\n\"");
    Lexer lexer(is);
    EXPECT_THROW(lexer.getNextToken(), IllegalCharacterError);
}

TEST(Lexer, numericLiteralWithUnderscores)
{
    std::string const expected = "1_000_000";
    std::istringstream is(expected);
    Lexer lexer(is);
    Token token = lexer.getNextToken();
    EXPECT_EQ(token.type, TokenType::numeric_literal);
    EXPECT_EQ(token.value, "1000000");
}

TEST(Lexer, numericLiteralWithBackticks)
{
    std::string const expected = "1`000`000";
    std::istringstream is(expected);
    Lexer lexer(is);
    Token token = lexer.getNextToken();
    EXPECT_EQ(token.type, TokenType::numeric_literal);
    EXPECT_EQ(token.value, "1000000");
}