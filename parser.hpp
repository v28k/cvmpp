#pragma once
#include "ast.hpp"
#include <initializer_list>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

class Parser{
public:
    explicit Parser(std::vector<Token> tokens):tokens(std::move(tokens)){}
    std::vector<std::unique_ptr<Stmt>> parse();

private:
    std::vector<Token> tokens;
    size_t current=0;

    bool isAtEnd()const{return peek().type==TokenType::END;}
    const Token&peek()const{return tokens[current];}
    const Token&previous()const{return tokens[current-1];}
    const Token&advance(){if(!isAtEnd())current++;return previous();}
    bool check(TokenType t)const{return !isAtEnd()&&peek().type==t;}
    bool match(std::initializer_list<TokenType> types);
    bool match(TokenType type){return match({type});}
    const Token&consume(TokenType type,const std::string&msg);

    std::unique_ptr<Stmt> declaration();
    std::unique_ptr<Stmt> statement();
    std::unique_ptr<Stmt> letDeclaration();
    std::unique_ptr<Stmt> printStatement();
    std::unique_ptr<Stmt> ifStatement();
    std::unique_ptr<Stmt> whileStatement();
    std::unique_ptr<Stmt> blockStatement();
    std::unique_ptr<Stmt> expressionStatement();

    std::unique_ptr<Expr> expression();
    std::unique_ptr<Expr> assignment();
    std::unique_ptr<Expr> equality();
    std::unique_ptr<Expr> comparison();
    std::unique_ptr<Expr> term();
    std::unique_ptr<Expr> factor();
    std::unique_ptr<Expr> unary();
    std::unique_ptr<Expr> primary();
};