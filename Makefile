export PATH := /opt/gcc/gcc-7.1.0:$(PATH)
CXX = g++
CC = gcc

team:
	@echo "Team: Compliation_Error\n"
	@echo "Yi Qiao"
	@echo "qiao22"
	@echo
	@echo "Shutao Wang"
	@echo "wang2590"

compiler: ./src/* 

	mkdir -p generated
	mkdir -p build
	flex -o generated/scanner.cpp src/scanner.ll
	bison -d -o generated/parser.cpp src/parser.yy
	$(CXX) -std=c++0x -c generated/scanner.cpp -o build/scanner.o -ll -g -static-libstdc++
	$(CXX) -std=c++0x -c generated/parser.cpp -o build/parser.o -g -static-libstdc++
	$(CXX) -std=c++0x -c src/comp.cpp -o build/comp.o -g -static-libstdc++
	$(CXX) -std=c++0x -c src/ast.cpp -o build/ast.o -g -static-libstdc++
	$(CXX) -std=c++0x -c src/symtable.cpp -o build/symtable.o -g -static-libstdc++
	$(CXX) -std=c++0x -o compiler build/scanner.o build/parser.o build/comp.o build/ast.o build/symtable.o -g -static-libstdc++

clean:
	rm -rf generated
	rm -rf build

