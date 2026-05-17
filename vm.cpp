#include "vm.hpp"
#include "debug.hpp"
#include <iostream>
#include <stdexcept>

template<typename F>
static Value applyIntBinary(const Value&a,const Value&b,F f){
    if(!std::holds_alternative<int64_t>(a)||!std::holds_alternative<int64_t>(b))
        throw std::runtime_error("Operands must be integers");
    return (int64_t)f(std::get<int64_t>(a),std::get<int64_t>(b));
}

Value VM::pop(){
    if(stack.empty())throw std::runtime_error("Stack underflow");
    Value v=std::move(stack.back());
    stack.pop_back();
    return v;
}

const Value& VM::peek()const{
    if(stack.empty())throw std::runtime_error("Stack underflow");
    return stack.back();
}

void VM::printStack()const{
    std::cout<<"[";
    for(size_t i=0;i<stack.size();i++){
        if(i)std::cout<<", ";
        std::cout<<valueToString(stack[i]);
    }
    std::cout<<"]\n";
}

void VM::run(const Chunk& c,bool trace){
    chunk=&c;
    ip=0;
    stack.clear();

    for(;;){
        size_t currentIp=ip;
        OpCode op=(OpCode)readByte();

        if(trace){
            std::cout<<"IP="<<currentIp<<" OP="<<opcodeName(op)<<" STACK=";
            printStack();
        }

        switch(op){
            case OpCode::CONSTANT:
                push(readConstant());
                break;

            case OpCode::TRUE_:
                push(true);
                break;

            case OpCode::FALSE_:
                push(false);
                break;

            case OpCode::ADD:{
                auto b=pop();
                auto a=pop();
                push(applyIntBinary(a,b,[](auto x,auto y){return x+y;}));
                break;
            }

            case OpCode::SUBTRACT:{
                auto b=pop();
                auto a=pop();
                push(applyIntBinary(a,b,[](auto x,auto y){return x-y;}));
                break;
            }

            case OpCode::MULTIPLY:{
                auto b=pop();
                auto a=pop();
                push(applyIntBinary(a,b,[](auto x,auto y){return x*y;}));
                break;
            }

            case OpCode::DIVIDE:{
                auto b=pop();
                auto a=pop();
                push(applyIntBinary(a,b,[](auto x,auto y){
                    if(y==0)throw std::runtime_error("Division by zero");
                    return x/y;
                }));
                break;
            }

            case OpCode::NEGATE:{
                auto v=pop();
                if(!std::holds_alternative<int64_t>(v))
                    throw std::runtime_error("Operand must be integer");
                push(-std::get<int64_t>(v));
                break;
            }

            case OpCode::EQUAL:{
                auto b=pop();
                auto a=pop();
                push(valuesEqual(a,b));
                break;
            }

            case OpCode::LESS:{
                auto b=pop();
                auto a=pop();
                if(!std::holds_alternative<int64_t>(a)||!std::holds_alternative<int64_t>(b))
                    throw std::runtime_error("Operands must be integers");
                push(std::get<int64_t>(a)<std::get<int64_t>(b));
                break;
            }

            case OpCode::GET_GLOBAL:{
                auto name=readName();
                auto it=globals.find(name);
                if(it==globals.end())
                    throw std::runtime_error("Undefined variable: "+name);
                push(it->second);
                break;
            }

            case OpCode::DEFINE_GLOBAL:{
                auto name=readName();
                globals[name]=pop();
                break;
            }

            case OpCode::SET_GLOBAL:{
                auto name=readName();
                if(globals.find(name)==globals.end())
                    throw std::runtime_error("Undefined variable: "+name);
                globals[name]=peek();
                break;
            }

            case OpCode::POP:
                pop();
                break;

            case OpCode::JUMP_IF_FALSE:{
                uint16_t offset=readShort();
                if(!isTruthy(peek()))ip+=offset;
                break;
            }

            case OpCode::JUMP:
                ip+=readShort();
                break;

            case OpCode::LOOP:{
                uint16_t offset=readShort();
                ip-=offset;
                break;
            }

            case OpCode::PRINT:
                std::cout<<valueToString(pop())<<'\n';
                break;

            case OpCode::INPUT:{
                long long x;
                std::cin>>x;
                push((int64_t)x);
                break;
            }

            case OpCode::RETURN:
                return;
        }
    }
}