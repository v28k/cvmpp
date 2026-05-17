#pragma once
#include "chunk.hpp"
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

class VM{
public:
    void run(const Chunk& chunk,bool trace=false);

private:
    const Chunk* chunk=nullptr;
    size_t ip=0;
    std::vector<Value> stack;
    std::unordered_map<std::string,Value> globals;

    uint8_t readByte(){return (*chunk).code[ip++];}
    uint16_t readShort(){uint16_t hi=readByte();uint16_t lo=readByte();return (hi<<8)|lo;}
    const Value& readConstant(){return (*chunk).constants[readShort()];}
    const std::string& readName(){return std::get<std::string>((*chunk).constants[readShort()]);}

    void push(Value v){stack.push_back(std::move(v));}
    Value pop();
    const Value& peek()const;

    void printStack()const;
};