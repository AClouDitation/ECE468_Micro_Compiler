team:
	@echo "Team: Compliation_Error\n"
	@echo "Yi Qiao"
	@echo "qiao22"
	@echo
	@echo "Shutao Wang"
	@echo "wang2590"

compiler:
	flex -+ scanner.l
	g++	lex.yy.cc -o compiler -ll

clean:
	rm lex.yy.cc -rf

