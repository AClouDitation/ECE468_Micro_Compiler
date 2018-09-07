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

compiler: scanner.l parser.y $(src) 

	flex scanner.l
	bison -d parser.y
	$(CC) -c lex.yy.c -o lex.o -ll
	$(CC) -c parser.tab.c -o parser.o 
	$(CXX) -c comp.cpp -o comp.o
	$(CXX) -o compiler comp.o parser.o lex.o


clean:
	rm -rf lex.yy.c
	rm -rf parser.tab.c
	rm -rf parser.tab.h
	rm -rf *.o


