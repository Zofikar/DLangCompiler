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
    std::ifstream file(argv[1]);
    Lexer lexer(file, argv[1]);
    Token token;
    do
    {
        try
        {
            token = lexer.getNextToken();
            std::cout << "Token: \"" << token.value << "\", type: " << magic_enum::enum_name(token.type) << ", at (" << token.localization << ")\n";
        } catch (LexerError &e)
        {
            std::cerr << e.what() << std::endl;
            if (e.isRecoverable()) { lexer.recoverError(); } else { return 1; }
        }
    } while (token.type != TokenType::eof);
    return 0;
}
