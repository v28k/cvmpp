#include "compiler.hpp"
#include <stdexcept>

Chunk Compiler::compile(const std::vector<std::unique_ptr<Stmt>>& statements){
    chunk=Chunk();
    for(const auto& s:statements)compileStmt(s.get());
    emitByte(OpCode::RETURN);
    return chunk;
}

void Compiler::compileStmt(const Stmt* stmt){
    if(auto s=dynamic_cast<const ExprStmt*>(stmt)){
        compileExpr(s->expr.get());
        emitByte(OpCode::POP);
    }else if(auto s=dynamic_cast<const PrintStmt*>(stmt)){
        compileExpr(s->expr.get());
        emitByte(OpCode::PRINT);
    }else if(auto s=dynamic_cast<const VarStmt*>(stmt)){
        compileExpr(s->initializer.get());
        emitNameOp(OpCode::DEFINE_GLOBAL,s->name);
    }else if(auto s=dynamic_cast<const BlockStmt*>(stmt)){
        for(const auto& st:s->statements)compileStmt(st.get());
    }else if(auto s=dynamic_cast<const IfStmt*>(stmt)){
        compileExpr(s->condition.get());
        size_t thenJump=chunk.emitJump(OpCode::JUMP_IF_FALSE);
        emitByte(OpCode::POP);
        compileStmt(s->thenBranch.get());
        size_t elseJump=chunk.emitJump(OpCode::JUMP);
        chunk.patchJump(thenJump);
        emitByte(OpCode::POP);
        if(s->elseBranch)compileStmt(s->elseBranch.get());
        chunk.patchJump(elseJump);
    }else if(auto s=dynamic_cast<const WhileStmt*>(stmt)){
        size_t loopStart=chunk.code.size();
        compileExpr(s->condition.get());
        size_t exitJump=chunk.emitJump(OpCode::JUMP_IF_FALSE);
        emitByte(OpCode::POP);
        compileStmt(s->body.get());
        chunk.emitLoop(loopStart);
        chunk.patchJump(exitJump);
        emitByte(OpCode::POP);
    }else{
        throw std::runtime_error("Unknown statement type");
    }
}

void Compiler::compileExpr(const Expr* expr){
    if(auto e=dynamic_cast<const LiteralExpr*>(expr)){
        emitConstant(e->value);
    }else if(dynamic_cast<const InputExpr*>(expr)){
        emitByte(OpCode::INPUT);
    }else if(auto e=dynamic_cast<const VariableExpr*>(expr)){
        emitNameOp(OpCode::GET_GLOBAL,e->name);
    }else if(auto e=dynamic_cast<const AssignExpr*>(expr)){
        compileExpr(e->value.get());
        emitNameOp(OpCode::SET_GLOBAL,e->name);
    }else if(auto e=dynamic_cast<const GroupingExpr*>(expr)){
        compileExpr(e->expr.get());
    }else if(auto e=dynamic_cast<const UnaryExpr*>(expr)){
        compileExpr(e->right.get());
        if(e->op==TokenType::MINUS)emitByte(OpCode::NEGATE);
        else throw std::runtime_error("Unsupported unary operator");
    }else if(auto e=dynamic_cast<const BinaryExpr*>(expr)){
        compileExpr(e->left.get());
        compileExpr(e->right.get());
        switch(e->op){
            case TokenType::PLUS:emitByte(OpCode::ADD);break;
            case TokenType::MINUS:emitByte(OpCode::SUBTRACT);break;
            case TokenType::STAR:emitByte(OpCode::MULTIPLY);break;
            case TokenType::SLASH:emitByte(OpCode::DIVIDE);break;
            case TokenType::EQUAL_EQUAL:emitByte(OpCode::EQUAL);break;
            case TokenType::LESS:emitByte(OpCode::LESS);break;
            default:throw std::runtime_error("Unsupported binary operator");
        }
    }else{
        throw std::runtime_error("Unknown expression type");
    }
}