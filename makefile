CXX := g++
PARSER := bison
TOKENIZER := flex
HEADERS := $(wildcard /src/include/I*.hpp)
INC_DIR := ./src/
CXXFLAGS := -std=c++11 -Wall  -lboost_regex -lfl -I$(INC_DIR)

all: i++

i++: src/generated/parser.cpp src/generated/tokens.c $(HEADERS)
	$(CXX) -o bin/i++ src/generated/parser.cpp src/generated/tokens.c $(CXXFLAGS)

src/generated/parser.cpp: src/bison/parser.yy
	$(PARSER) -d -t -o src/generated/parser.cpp src/bison/parser.yy

src/generated/tokens.c: src/flex/tokens.l
	$(TOKENIZER) -o src/generated/tokens.c src/flex/tokens.l

I%hpp:

clean:
	rm src/generated/parser.cpp src/generated/parser.hpp src/generated/tokens.c bin/i++
