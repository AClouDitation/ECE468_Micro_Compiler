CXX = g++
CC = gcc
src = comp.cpp
obj = $(src:%.cpp=%.o)


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
	$(CXX) -std=c++98 -c generated/scanner.cpp -o build/scanner.o -ll -g
	$(CXX) -std=c++98 -c generated/parser.cpp -o build/parser.o -g
	$(CXX) -std=c++98 -c src/comp.cpp -o build/comp.o -g
	$(CXX) -std=c++98 -c src/symtable.cpp -o build/symtable.o -g
	$(CXX) -std=c++98 -o compiler build/comp.o build/parser.o build/scanner.o build/symtable.o -g

clean:
	rm -rf generated
	rm -rf build
	rm -rf compiler


