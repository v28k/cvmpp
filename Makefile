CXX=g++
CXXFLAGS=-std=c++17 -O2 -Wall -Wextra -pedantic

TARGET=cvmpp
SRC=main.cpp lexer.cpp parser.cpp compiler.cpp vm.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean