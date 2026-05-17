#include "lexer.hpp"
#include <stdexcept>
#include <utility>

bool Lexer::match(char expected){
    if(isAtEnd()||source[current]!=expected)return false;
    current++;
    return true;
}

void Lexer::addToken(std::vector<Token>&tokens,TokenType type,Literal lit){
    tokens.push_back({type,source.substr(start,current-start),std::move(lit),line});
}

void Lexer::skipWhitespace(){
    for(;;){
        if(isAtEnd())return;
        char c=peek();
        switch(c){
            case ' ':
            case '\r':
            case '\t':
                current++;
                break;
            case '\n':
                line++;
                current++;
                break;
            case '/':
                if(peekNext()=='/'){
                    while(!isAtEnd()&&peek()!='\n')current++;
                }else{
                    return;
                }
                break;
            default:
                return;
        }
    }
}

void Lexer::number(std::vector<Token>&tokens){
    while(std::isdigit((unsigned char)peek()))advance();
    int64_t v=std::stoll(source.substr(start,current-start));
    addToken(tokens,TokenType::NUMBER,v);
}

void Lexer::identifier(std::vector<Token>&tokens){
    while(isAlphaNumeric(peek()))advance();
    std::string text=source.substr(start,current-start);

    if(text=="let")addToken(tokens,TokenType::LET);
    else if(text=="print")addToken(tokens,TokenType::PRINT);
    else if(text=="if")addToken(tokens,TokenType::IF);
    else if(text=="else")addToken(tokens,TokenType::ELSE);
    else if(text=="while")addToken(tokens,TokenType::WHILE);
    else if(text=="true")addToken(tokens,TokenType::TRUE_,true);
    else if(text=="false")addToken(tokens,TokenType::FALSE_,false);
    else if(text=="input")addToken(tokens,TokenType::INPUT);
    else addToken(tokens,TokenType::IDENTIFIER);
}

std::vector<Token> Lexer::scanTokens(){
    std::vector<Token> tokens;

    while(!isAtEnd()){
        skipWhitespace();
        start=current;
        if(isAtEnd())break;

        char c=advance();
        if(std::isdigit((unsigned char)c)){
            number(tokens);
        }else if(isAlpha(c)){
            identifier(tokens);
        }else{
            switch(c){
                case '+':addToken(tokens,TokenType::PLUS);break;
                case '-':addToken(tokens,TokenType::MINUS);break;
                case '*':addToken(tokens,TokenType::STAR);break;
                case '/':addToken(tokens,TokenType::SLASH);break;
                case '(':addToken(tokens,TokenType::LPAREN);break;
                case ')':addToken(tokens,TokenType::RPAREN);break;
                case '{':addToken(tokens,TokenType::LBRACE);break;
                case '}':addToken(tokens,TokenType::RBRACE);break;
                case ';':addToken(tokens,TokenType::SEMICOLON);break;
                case ',':addToken(tokens,TokenType::COMMA);break;
                case '=':addToken(tokens,match('=')?TokenType::EQUAL_EQUAL:TokenType::EQUAL);break;
                case '<':addToken(tokens,TokenType::LESS);break;
                default:
                    throw std::runtime_error(std::string("Unexpected character: ")+c+" at line "+std::to_string(line));
            }
        }
    }

    tokens.push_back({TokenType::END,"",{},line});
    return tokens;
}