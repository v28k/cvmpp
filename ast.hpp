#pragma once
#include "token.hpp"
#include "value.hpp"
#include <memory>
#include <string>
#include <utility>
#include <vector>

struct Expr{
    virtual ~Expr()=default;
};

struct LiteralExpr:Expr{
    Value value;
    explicit LiteralExpr(Value v):value(std::move(v)){}
};

struct VariableExpr:Expr{
    std::string name;
    explicit VariableExpr(std::string n):name(std::move(n)){}
};

struct AssignExpr:Expr{
    std::string name;
    std::unique_ptr<Expr> value;
    AssignExpr(std::string n,std::unique_ptr<Expr> v):name(std::move(n)),value(std::move(v)){}
};

struct UnaryExpr:Expr{
    TokenType op;
    std::unique_ptr<Expr> right;
    UnaryExpr(TokenType o,std::unique_ptr<Expr> r):op(o),right(std::move(r)){}
};

struct BinaryExpr:Expr{
    std::unique_ptr<Expr> left;
    TokenType op;
    std::unique_ptr<Expr> right;
    BinaryExpr(std::unique_ptr<Expr> l,TokenType o,std::unique_ptr<Expr> r):left(std::move(l)),op(o),right(std::move(r)){}
};

struct GroupingExpr:Expr{
    std::unique_ptr<Expr> expr;
    explicit GroupingExpr(std::unique_ptr<Expr> e):expr(std::move(e)){}
};

struct InputExpr:Expr{};

struct Stmt{
    virtual ~Stmt()=default;
};

struct ExprStmt:Stmt{
    std::unique_ptr<Expr> expr;
    explicit ExprStmt(std::unique_ptr<Expr> e):expr(std::move(e)){}
};

struct PrintStmt:Stmt{
    std::unique_ptr<Expr> expr;
    explicit PrintStmt(std::unique_ptr<Expr> e):expr(std::move(e)){}
};

struct VarStmt:Stmt{
    std::string name;
    std::unique_ptr<Expr> initializer;
    VarStmt(std::string n,std::unique_ptr<Expr> i):name(std::move(n)),initializer(std::move(i)){}
};

struct BlockStmt:Stmt{
    std::vector<std::unique_ptr<Stmt>> statements;
    explicit BlockStmt(std::vector<std::unique_ptr<Stmt>> s):statements(std::move(s)){}
};

struct IfStmt:Stmt{
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> thenBranch;
    std::unique_ptr<Stmt> elseBranch;
    IfStmt(std::unique_ptr<Expr> c,std::unique_ptr<Stmt> t,std::unique_ptr<Stmt> e):condition(std::move(c)),thenBranch(std::move(t)),elseBranch(std::move(e)){}
};

struct WhileStmt:Stmt{
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> body;
    WhileStmt(std::unique_ptr<Expr> c,std::unique_ptr<Stmt> b):condition(std::move(c)),body(std::move(b)){}
};