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
compiler_local: build/scanner.o link

build/parser.o: src/frontend/parser.yy
	@mkdir -p generated
	@mkdir -p build
	bison -d -o generated/parser.cpp src/frontend/parser.yy
	$(CXX) $(CXXFLAGS) -c generated/parser.cpp -o build/parser.o 

build/scanner.o: build/parser.o src/frontend/scanner.ll
	flex -o generated/scanner.cpp src/frontend/scanner.ll
	$(CXX) $(CXXFLAGS) -c generated/scanner.cpp -o build/scanner.o -ll

build/main.o: ./src/main.cpp
	$(CXX) $(CXXFLAGS) $< -c -o build/main.o 

build/%.o: ./src/%.cpp ./inc/%.hpp
	@mkdir -p $(@D)
	$(CXX) $< $(CXXFLAGS) -c -o $@

link: build/scanner.o build/parser.o $(OBJ)
	@echo "linking object files ......"
	@$(CXX) $(CXXFLAGS) -o compiler $^
	@echo "Done!"

clean:
	rm -rf generated
	rm -rf build
