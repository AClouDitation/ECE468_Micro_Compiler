CXXFLAGS := -std=c++0x -g -static-libstdc++
CXX_ECN := /opt/gcc/7.1.0/bin/g++
CXX_LOCAL := g++
CC := gcc
CXX := 

team:
	@echo "Team: Compliation_Error\n"
	@echo "Yi Qiao"
	@echo "qiao22"
	@echo
	@echo "Shutao Wang"
	@echo "wang2590"
	
compiler: CXX := $(CXX_ECN)
compiler: compile

compiler_local: CXX := $(CXX_LOCAL)
compiler_local: compile

compile: ./src/*
	mkdir -p generated
	mkdir -p build
	flex -o generated/scanner.cpp src/scanner.ll
	bison -d -o generated/parser.cpp src/parser.yy
	$(CXX) $(CXXFLAGS) -c generated/scanner.cpp -o build/scanner.o -ll
	$(CXX) $(CXXFLAGS) -c generated/parser.cpp -o build/parser.o 
	$(CXX) $(CXXFLAGS) -c src/main.cpp -o build/comp.o
	$(CXX) $(CXXFLAGS) -c src/utility.cpp -o build/utility.o
	$(CXX) $(CXXFLAGS) -c src/ast/ExprNode.cpp -o build/ExprNode.o 
	$(CXX) $(CXXFLAGS) -c src/ast/StmtNode.cpp -o build/StmtNode.o 
	$(CXX) $(CXXFLAGS) -c src/opt.cpp -o build/opt.o 
	$(CXX) $(CXXFLAGS) -c src/symtable/symtable.cpp -o build/symtable.o 
	$(CXX) $(CXXFLAGS) -o compiler build/*.o 	

clean:
	rm -rf generated
	rm -rf build

