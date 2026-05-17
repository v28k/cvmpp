#pragma once
#include <cstdint>
#include <string>
#include <variant>

using Value=std::variant<int64_t,bool,std::string>;

inline std::string valueToString(const Value&v){
    if(std::holds_alternative<int64_t>(v))return std::to_string(std::get<int64_t>(v));
    if(std::holds_alternative<bool>(v))return std::get<bool>(v)?"true":"false";
    return std::get<std::string>(v);
}

inline bool isTruthy(const Value&v){
    if(std::holds_alternative<bool>(v))return std::get<bool>(v);
    if(std::holds_alternative<int64_t>(v))return std::get<int64_t>(v)!=0;
    return !std::get<std::string>(v).empty();
}

inline bool valuesEqual(const Value&a,const Value&b){
    if(a.index()!=b.index())return false;
    if(std::holds_alternative<int64_t>(a))return std::get<int64_t>(a)==std::get<int64_t>(b);
    if(std::holds_alternative<bool>(a))return std::get<bool>(a)==std::get<bool>(b);
    return std::get<std::string>(a)==std::get<std::string>(b);
}