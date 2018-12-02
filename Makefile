CXXFLAGS := -std=c++11 -g -static-libstdc++
CXX_ECN := /opt/gcc/7.1.0/bin/g++
CXX_LOCAL := g++
CC := gcc
CXX := 

SRC := $(shell find src -type f -name '*.cpp')
OBJ := $(patsubst src/%.cpp, build/%.o, $(SRC))
BUILD_PATH = $(PWD)/build

team:
	@echo "Team: Compliation_Error\n"
	@echo "Yi Qiao"
	@echo "qiao22"
	@echo
	@echo "Shutao Wang"
	@echo "wang2590"
	
compiler: CXX := $(CXX_ECN)
compiler: main

compiler_local: CXX := $(CXX_LOCAL)
compiler_local: frontend link

frontend: scanner parser
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c generated/scanner.cpp -o build/scanner.o -ll
	$(CXX) $(CXXFLAGS) -c generated/parser.cpp -o build/parser.o 

scanner: src/frontend/scanner.ll
	@mkdir -p generated
	flex -o generated/scanner.cpp src/frontend/scanner.ll

parser:	src/frontend/parser.yy
	@mkdir -p generated
	bison -d -o generated/parser.cpp src/frontend/parser.yy

build/%.o: ./src/%.cpp ./inc/%.hpp
	$(CXX) $< $(CXXFLAGS) -o $@

link: $(OBJ)
	$(CXX) $(CXXFLAGS) -o compiler $(OBJ)

clean:
	rm -rf generated
	rm -rf build
