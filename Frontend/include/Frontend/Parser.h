#pragma once

#include <Frontend/Lexer.h>

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>


struct Node
{

};

class Parser
{
public:
    Parser(Lexer &lexer);

private:
    Lexer &lexer;
};
