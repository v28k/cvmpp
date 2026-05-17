#pragma once
#include "token.hpp"
#include <cctype>
#include <string>
#include <utility>
#include <vector>

class Lexer{
public:
    explicit Lexer(std::string src):source(std::move(src)){}
    std::vector<Token> scanTokens();

private:
    std::string source;
    size_t start=0;
    size_t current=0;
    int line=1;

    bool isAtEnd()const{return current>=source.size();}
    char advance(){return source[current++];}
    bool match(char expected);
    char peek()const{return isAtEnd()?'\0':source[current];}
    char peekNext()const{return current+1>=source.size()?'\0':source[current+1];}

    void addToken(std::vector<Token>&tokens,TokenType type,Literal lit={});
    void skipWhitespace();
    void number(std::vector<Token>&tokens);
    void identifier(std::vector<Token>&tokens);

    static bool isAlpha(char c){return std::isalpha((unsigned char)c)||c=='_';}
    static bool isAlphaNumeric(char c){return isAlpha(c)||std::isdigit((unsigned char)c);}
};