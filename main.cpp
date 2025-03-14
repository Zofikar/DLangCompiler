#include <iostream>
#include "Frontend/Lexer.h"
#include "Frontend/LexerErrors.h"

#include <magic_enum/magic_enum.hpp>

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <file>" << std::endl;
        return 1;
    }
    try
    {
        std::ifstream file(argv[1]);
        Lexer lexer(file);
        Token token;
        do {
            token = lexer.getNextToken();
            std::cout << "Token: \"" << token.value << "\", type: " << magic_enum::enum_name(token.type) << ", at (" << token.localization << ")\n";
        } while (token.type != TokenType::eof);
    } catch (LexerError& e)
    {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
