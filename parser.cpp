#include "parser.hpp"

bool Parser::match(std::initializer_list<TokenType> types){
    for(auto t:types){
        if(check(t)){
            advance();
            return true;
        }
    }
    return false;
}

const Token&Parser::consume(TokenType type,const std::string&msg){
    if(check(type))return advance();
    throw std::runtime_error(msg+" at line "+std::to_string(peek().line));
}

std::vector<std::unique_ptr<Stmt>> Parser::parse(){
    std::vector<std::unique_ptr<Stmt>> stmts;
    while(!isAtEnd())stmts.push_back(declaration());
    return stmts;
}

std::unique_ptr<Stmt> Parser::declaration(){
    if(match(TokenType::LET))return letDeclaration();
    return statement();
}

std::unique_ptr<Stmt> Parser::statement(){
    if(match(TokenType::PRINT))return printStatement();
    if(match(TokenType::IF))return ifStatement();
    if(match(TokenType::WHILE))return whileStatement();
    if(match(TokenType::LBRACE))return blockStatement();
    return expressionStatement();
}

std::unique_ptr<Stmt> Parser::letDeclaration(){
    auto name=consume(TokenType::IDENTIFIER,"Expected variable name").lexeme;
    consume(TokenType::EQUAL,"Expected '=' after variable name");
    auto init=expression();
    consume(TokenType::SEMICOLON,"Expected ';' after declaration");
    return std::make_unique<VarStmt>(name,std::move(init));
}

std::unique_ptr<Stmt> Parser::printStatement(){
    auto expr=expression();
    consume(TokenType::SEMICOLON,"Expected ';' after print");
    return std::make_unique<PrintStmt>(std::move(expr));
}

std::unique_ptr<Stmt> Parser::ifStatement(){
    consume(TokenType::LPAREN,"Expected '(' after if");
    auto cond=expression();
    consume(TokenType::RPAREN,"Expected ')' after condition");
    auto thenBranch=statement();
    std::unique_ptr<Stmt> elseBranch=nullptr;
    if(match(TokenType::ELSE))elseBranch=statement();
    return std::make_unique<IfStmt>(std::move(cond),std::move(thenBranch),std::move(elseBranch));
}

std::unique_ptr<Stmt> Parser::whileStatement(){
    consume(TokenType::LPAREN,"Expected '(' after while");
    auto cond=expression();
    consume(TokenType::RPAREN,"Expected ')' after condition");
    auto body=statement();
    return std::make_unique<WhileStmt>(std::move(cond),std::move(body));
}

std::unique_ptr<Stmt> Parser::blockStatement(){
    std::vector<std::unique_ptr<Stmt>> stmts;
    while(!check(TokenType::RBRACE)&&!isAtEnd())stmts.push_back(declaration());
    consume(TokenType::RBRACE,"Expected '}' after block");
    return std::make_unique<BlockStmt>(std::move(stmts));
}

std::unique_ptr<Stmt> Parser::expressionStatement(){
    auto expr=expression();
    consume(TokenType::SEMICOLON,"Expected ';' after expression");
    return std::make_unique<ExprStmt>(std::move(expr));
}

std::unique_ptr<Expr> Parser::expression(){
    return assignment();
}

std::unique_ptr<Expr> Parser::assignment(){
    auto expr=equality();
    if(match(TokenType::EQUAL)){
        auto value=assignment();
        if(auto* v=dynamic_cast<VariableExpr*>(expr.get()))
            return std::make_unique<AssignExpr>(v->name,std::move(value));
        throw std::runtime_error("Invalid assignment target at line "+std::to_string(previous().line));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::equality(){
    auto expr=comparison();
    while(match(TokenType::EQUAL_EQUAL)){
        TokenType op=previous().type;
        auto right=comparison();
        expr=std::make_unique<BinaryExpr>(std::move(expr),op,std::move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::comparison(){
    auto expr=term();
    while(match(TokenType::LESS)){
        TokenType op=previous().type;
        auto right=term();
        expr=std::make_unique<BinaryExpr>(std::move(expr),op,std::move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::term(){
    auto expr=factor();
    while(match({TokenType::PLUS,TokenType::MINUS})){
        TokenType op=previous().type;
        auto right=factor();
        expr=std::make_unique<BinaryExpr>(std::move(expr),op,std::move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::factor(){
    auto expr=unary();
    while(match({TokenType::STAR,TokenType::SLASH})){
        TokenType op=previous().type;
        auto right=unary();
        expr=std::make_unique<BinaryExpr>(std::move(expr),op,std::move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::unary(){
    if(match(TokenType::MINUS)){
        TokenType op=previous().type;
        auto right=unary();
        return std::make_unique<UnaryExpr>(op,std::move(right));
    }
    return primary();
}

std::unique_ptr<Expr> Parser::primary(){
    if(match(TokenType::NUMBER))
        return std::make_unique<LiteralExpr>(std::get<int64_t>(previous().literal));
    if(match(TokenType::TRUE_))
        return std::make_unique<LiteralExpr>(true);
    if(match(TokenType::FALSE_))
        return std::make_unique<LiteralExpr>(false);
    if(match(TokenType::INPUT))
        return std::make_unique<InputExpr>();
    if(match(TokenType::IDENTIFIER))
        return std::make_unique<VariableExpr>(previous().lexeme);
    if(match(TokenType::LPAREN)){
        auto expr=expression();
        consume(TokenType::RPAREN,"Expected ')' after expression");
        return std::make_unique<GroupingExpr>(std::move(expr));
    }
    throw std::runtime_error("Expected expression at line "+std::to_string(peek().line));
}