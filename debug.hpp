#pragma once
#include "chunk.hpp"
#include <iomanip>
#include <iostream>
#include <string>

inline std::string opcodeName(OpCode op){
    switch(op){
        case OpCode::CONSTANT:return "CONSTANT";
        case OpCode::TRUE_:return "TRUE";
        case OpCode::FALSE_:return "FALSE";
        case OpCode::ADD:return "ADD";
        case OpCode::SUBTRACT:return "SUBTRACT";
        case OpCode::MULTIPLY:return "MULTIPLY";
        case OpCode::DIVIDE:return "DIVIDE";
        case OpCode::NEGATE:return "NEGATE";
        case OpCode::EQUAL:return "EQUAL";
        case OpCode::LESS:return "LESS";
        case OpCode::GET_GLOBAL:return "GET_GLOBAL";
        case OpCode::DEFINE_GLOBAL:return "DEFINE_GLOBAL";
        case OpCode::SET_GLOBAL:return "SET_GLOBAL";
        case OpCode::POP:return "POP";
        case OpCode::JUMP_IF_FALSE:return "JUMP_IF_FALSE";
        case OpCode::JUMP:return "JUMP";
        case OpCode::LOOP:return "LOOP";
        case OpCode::PRINT:return "PRINT";
        case OpCode::INPUT:return "INPUT";
        case OpCode::RETURN:return "RETURN";
        default:return "UNKNOWN";
    }
}

inline size_t instructionSize(const Chunk& chunk,size_t offset){
    OpCode op=(OpCode)chunk.code[offset];
    switch(op){
        case OpCode::CONSTANT:
        case OpCode::GET_GLOBAL:
        case OpCode::DEFINE_GLOBAL:
        case OpCode::SET_GLOBAL:
        case OpCode::JUMP_IF_FALSE:
        case OpCode::JUMP:
        case OpCode::LOOP:
            return 3;
        default:
            return 1;
    }
}

inline void disassembleChunk(const Chunk& chunk,const std::string& name="chunk"){
    std::cout<<"== "<<name<<" ==\n";
    for(size_t offset=0;offset<chunk.code.size();){
        std::cout<<std::setw(4)<<std::setfill('0')<<offset<<" ";

        OpCode op=(OpCode)chunk.code[offset];
        std::cout<<opcodeName(op);

        switch(op){
            case OpCode::CONSTANT:{
                uint16_t index=(chunk.code[offset+1]<<8)|chunk.code[offset+2];
                std::cout<<" "<<index<<" -> "<<valueToString(chunk.constants[index]);
                break;
            }
            case OpCode::GET_GLOBAL:
            case OpCode::DEFINE_GLOBAL:
            case OpCode::SET_GLOBAL:{
                uint16_t index=(chunk.code[offset+1]<<8)|chunk.code[offset+2];
                std::cout<<" "<<index<<" -> "<<std::get<std::string>(chunk.constants[index]);
                break;
            }
            case OpCode::JUMP_IF_FALSE:
            case OpCode::JUMP:
            case OpCode::LOOP:{
                uint16_t jump=(chunk.code[offset+1]<<8)|chunk.code[offset+2];
                std::cout<<" "<<jump;
                break;
            }
            default:
                break;
        }

        std::cout<<"\n";
        offset+=instructionSize(chunk,offset);
    }
}