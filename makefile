GXX := g++
TOKENIZER := flex
PARSER := bison
FLAGS := -std=c++11 -lboost_regex -I./src/ -Wall
headers := $(wildcard /src/include/I*.hpp)

all: i++

i++: src/generated/parser.cpp src/generated/tokens.cpp $(headers)
	$(GXX) -o bin/i++ src/generated/parser.cpp src/generated/tokens.cpp $(FLAGS)

src/generated/parser.cpp: src/bison/parser.yy
	$(PARSER) -d -o src/generated/parser.cpp src/bison/parser.yy

src/generated/tokens.cpp: src/flex/tokens.l
	$(TOKENIZER) -o src/generated/tokens.cpp src/flex/tokens.l

I%hpp:

clean:
	rm ./bin/i++ ./src/generated/parser.cpp ./src/generated/parser.hpp ./src/generated/tokens.cpp
