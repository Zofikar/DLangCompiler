#pragma once
#include <Lexer.h>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>


class Parser
{
public:
    Parser(Lexer &lexer);

    typedef std::unordered_map<std::string, std::string> Signature;

private:
    std::unordered_set<std::string> types;                            //1st pass fill - 2nd pass use
    std::unordered_map<std::string, Signature> functions;             //1st pass fill - 2nd pass use
    std::vector<std::unordered_map<std::string, std::string> > stack; //2nd pass fill and use
    Lexer &lexer;
};
