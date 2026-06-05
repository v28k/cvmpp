# CVM++ : Custom Virtual Machine & Bytecode Compiler

![C++](https://img.shields.io/badge/Language-C++17-blue)
![Architecture](https://img.shields.io/badge/Architecture-Stack--Based-orange)
![Build](https://img.shields.io/badge/Build-CMake%20%7C%20Make-success)

CVM++ is an end-to-end programming language runtime built entirely from scratch in C++17. It encompasses the complete execution pipeline: from lexical analysis and recursive-descent parsing to bytecode compilation and stack-based virtual machine execution. 

Designed with a focus on performance and formal language theory, CVM++ features an expandable 8-bit Instruction Set Architecture (ISA), dynamic type safety, and real-time execution profiling.

## 🚀 Features
* **Interactive REPL:** A persistent Read-Eval-Print Loop that retains global state across runtime execution chunks.
* **Microsecond Profiler:** Built-in telemetry to measure raw bytecode execution speed (`--profile`).
* **5-Stage Pipeline:** Source Code → Tokens → AST → Bytecode → VM Execution.
* **Robust Tooling:** Includes a bytecode disassembler and a VM state tracer for deep memory debugging.
* **Turing-Complete Constructs:** Supports while-loops, conditional branching, dynamic I/O, and nested arithmetic.

## 🧠 Language Grammar (EBNF)
CVM++ utilizes a clean, intuitive syntax. The formal grammar processed by the recursive-descent parser is structured as follows:

```ebnf
program        ::= declaration* EOF
declaration    ::= varDecl | statement
varDecl        ::= "let" IDENTIFIER "=" expression ";"
statement      ::= exprStmt | printStmt | ifStmt | whileStmt | block
exprStmt       ::= expression ";"
printStmt      ::= "print" expression ";"
ifStmt         ::= "if" expression block ("else" block)?
whileStmt      ::= "while" expression block
block          ::= "{" declaration* "}"
expression     ::= equality
equality       ::= comparison ( "==" comparison )*
comparison     ::= term ( "<" term )*
term           ::= factor ( ( "-" | "+" ) factor )*
factor         ::= unary ( ( "/" | "*" ) unary )*
unary          ::= "-" unary | primary
primary        ::= NUMBER | "true" | "false" | IDENTIFIER | "input" | "(" expression ")"
