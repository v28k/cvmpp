#pragma once
#include "ast.hpp"
#include "chunk.hpp"
#include <memory>
#include <string>
#include <vector>

class Compiler{
public:
    Chunk compile(const std::vector<std::unique_ptr<Stmt>>& statements);

private:
    Chunk chunk;

    void compileStmt(const Stmt* stmt);
    void compileExpr(const Expr* expr);

    void emitByte(OpCode op){
        chunk.write((uint8_t)op);
    }

    void emitConstant(const Value&v){
        if(std::holds_alternative<bool>(v)){
            emitByte(std::get<bool>(v)?OpCode::TRUE_:OpCode::FALSE_);
            return;
        }
        size_t idx=chunk.addConstant(v);
        emitByte(OpCode::CONSTANT);
        chunk.writeShort((uint16_t)idx);
    }

    void emitNameOp(OpCode op,const std::string&name){
        size_t idx=chunk.addConstant(name);
        emitByte(op);
        chunk.writeShort((uint16_t)idx);
    }
};