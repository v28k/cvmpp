cmake_minimum_required(VERSION 3.16)
project(cvmpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(cvmpp
    main.cpp
    lexer.cpp
    parser.cpp
    compiler.cpp
    vm.cpp
)

target_include_directories(cvmpp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})