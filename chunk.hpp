#pragma once
#include "value.hpp"
#include <cstdint>
#include <utility>
#include <vector>

enum class OpCode:uint8_t{
    CONSTANT,
    TRUE_,FALSE_,
    ADD,SUBTRACT,MULTIPLY,DIVIDE,
    NEGATE,
    EQUAL,LESS,
    GET_GLOBAL,DEFINE_GLOBAL,SET_GLOBAL,
    POP,
    JUMP_IF_FALSE,JUMP,LOOP,
    PRINT,INPUT,
    RETURN
};

struct Chunk{
    std::vector<uint8_t> code;
    std::vector<Value> constants;

    size_t addConstant(Value v){
        constants.push_back(std::move(v));
        return constants.size()-1;
    }

    void write(uint8_t byte){
        code.push_back(byte);
    }

    void writeShort(uint16_t s){
        code.push_back((s>>8)&0xff);
        code.push_back(s&0xff);
    }

    size_t emitJump(OpCode op){
        size_t pos=code.size();
        write((uint8_t)op);
        write(0xff);
        write(0xff);
        return pos;
    }

    void patchJump(size_t opPos){
        size_t jump=code.size()-opPos-3;
        code[opPos+1]=(jump>>8)&0xff;
        code[opPos+2]=jump&0xff;
    }

    void emitLoop(size_t loopStart){
        write((uint8_t)OpCode::LOOP);
        size_t offset=code.size()+2-loopStart;
        write((offset>>8)&0xff);
        write(offset&0xff);
    }
};