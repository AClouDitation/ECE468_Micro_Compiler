cc = g++
deps = compiler_driver.hpp
src = comp.cpp
src += compiler_driver.cpp
obj = $(src:%.cpp=%.o)


team:
	@echo "Team: Compliation_Error\n"
	@echo "Yi Qiao"
	@echo "qiao22"
	@echo
	@echo "Shutao Wang"
	@echo "wang2590"

compiler: $(obj) scanner.l

	flex -+ scanner.l
	$(cc) -c lex.yy.cc -o lex.o -ll
	$(cc) -o compiler $(obj) lex.o



%.o: %.cpp $(deps)
	$(cc) -c $< -o $@

clean:
	rm -rf lex.yy.cc
	rm -rf $(obj) lex.o

