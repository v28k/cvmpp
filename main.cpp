#include "lexer.hpp"
#include "parser.hpp"
#include "compiler.hpp"
#include "vm.hpp"
#include "debug.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

static std::string readFile(const std::string& path){
    std::ifstream in(path);
    if(!in)throw std::runtime_error("Cannot open file: "+path);
    std::ostringstream ss;
    ss<<in.rdbuf();
    return ss.str();
}

int main(int argc,char**argv){
    try{
        bool trace=false;
        bool disassemble=false;
        std::string filename;

        for(int i=1;i<argc;i++){
            std::string arg=argv[i];
            if(arg=="--trace")trace=true;
            else if(arg=="--disassemble")disassemble=true;
            else filename=arg;
        }

        std::string src;
        if(!filename.empty()){
            src=readFile(filename);
        }else{
            std::ostringstream ss;
            ss<<std::cin.rdbuf();
            src=ss.str();
        }

        Lexer lex(src);
        auto tokens=lex.scanTokens();

        Parser parser(std::move(tokens));
        auto ast=parser.parse();

        Compiler compiler;
        auto chunk=compiler.compile(ast);

        if(disassemble){
            disassembleChunk(chunk,"CVM++ bytecode");
        }

        VM vm;
        vm.run(chunk,trace);
    }catch(const std::exception& e){
        std::cerr<<"Error: "<<e.what()<<'\n';
        return 1;
    }

    return 0;
}