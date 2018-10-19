CXX := g++
CXXFLAGS := -std=c++0x -g -static-libstdc++
CXX_ECN := /opt/gcc/7.1.0/bin/g++
CC := gcc

INC := $(wildcard inc/*.h)
SRC := $(wildcard src/*.cpp)
GEN := generated/parser.cpp generated/scanner.cpp
OBJ := $(patsubst src/%.cpp, build/%.o, $(SRC))
OBJ += build/scanner.o
OBJ += build/parser.o

team:
	@echo "Team: Compliation_Error\n"
	@echo "Yi Qiao"
	@echo "qiao22"
	@echo
	@echo "Shutao Wang"
	@echo "wang2590"
	
	@echo $(OBJ)
compiler: ./src/* 
	mkdir -p generated
	mkdir -p build
	flex -o generated/scanner.cpp src/scanner.ll
	bison -d -o generated/parser.cpp src/parser.yy
	$(CXX_ECN) -std=c++0x -c generated/scanner.cpp -o build/scanner.o -ll -g -static-libstdc++
	$(CXX_ECN) -std=c++0x -c generated/parser.cpp -o build/parser.o -g -static-libstdc++
	$(CXX_ECN) -std=c++0x -c src/comp.cpp -o build/comp.o -g -static-libstdc++
	$(CXX_ECN) -std=c++0x -c src/ast.cpp -o build/ast.o -g -static-libstdc++
	$(CXX_ECN) -std=c++0x -c src/opt.cpp -o build/opt.o -g -static-libstdc++
	$(CXX_ECN) -std=c++0x -c src/symtable.cpp -o build/symtable.o -g -static-libstdc++
	$(CXX_ECN) -std=c++0x -o compiler build/*.o -g -static-libstdc++

compiler_local: $(OBJ) $(INC)
	$(CXX) $(CXXFLAGS) $(OBJ) -o compiler

dirs:
	@echo Makring directories...
	@if [ ! -d "./generated" ]; then \
		mkdir -p generated; \
	fi
	@if [ ! -d "./build" ]; then \
		mkdir -p build; \
	fi

generated/scanner.cpp: src/scanner.ll dirs
	flex -o generated/scanner.cpp src/scanner.ll

generated/parser.cpp: src/parser.yy generated/scanner.cpp dirs
	bison -d -o generated/parser.cpp src/parser.yy

build/scanner.o: generated/scanner.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ -ll

build/parser.o: generated/parser.cpp build/scanner.o
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/%.o: src/%.cpp dirs build/parser.o
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf generated
	rm -rf build

