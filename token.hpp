#pragma once
#include <cstdint>
#include <string>
#include <variant>

using Literal=std::variant<std::monostate,int64_t,bool,std::string>;

enum class TokenType{
    END,
    NUMBER,
    IDENTIFIER,
    LET,PRINT,IF,ELSE,WHILE,TRUE_,FALSE_,INPUT,
    PLUS,MINUS,STAR,SLASH,
    EQUAL,EQUAL_EQUAL,LESS,
    LPAREN,RPAREN,LBRACE,RBRACE,
    SEMICOLON,COMMA
};

struct Token{
    TokenType type;
    std::string lexeme;
    Literal literal;
    int line;
};

inline const char* tokenTypeName(TokenType type){
    switch(type){
        case TokenType::END:return "END";
        case TokenType::NUMBER:return "NUMBER";
        case TokenType::IDENTIFIER:return "IDENTIFIER";
        case TokenType::LET:return "LET";
        case TokenType::PRINT:return "PRINT";
        case TokenType::IF:return "IF";
        case TokenType::ELSE:return "ELSE";
        case TokenType::WHILE:return "WHILE";
        case TokenType::TRUE_:return "TRUE";
        case TokenType::FALSE_:return "FALSE";
        case TokenType::INPUT:return "INPUT";
        case TokenType::PLUS:return "PLUS";
        case TokenType::MINUS:return "MINUS";
        case TokenType::STAR:return "STAR";
        case TokenType::SLASH:return "SLASH";
        case TokenType::EQUAL:return "EQUAL";
        case TokenType::EQUAL_EQUAL:return "EQUAL_EQUAL";
        case TokenType::LESS:return "LESS";
        case TokenType::LPAREN:return "LPAREN";
        case TokenType::RPAREN:return "RPAREN";
        case TokenType::LBRACE:return "LBRACE";
        case TokenType::RBRACE:return "RBRACE";
        case TokenType::SEMICOLON:return "SEMICOLON";
        case TokenType::COMMA:return "COMMA";
        default:return "UNKNOWN";
    }
}