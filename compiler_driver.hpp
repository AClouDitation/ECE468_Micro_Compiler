#ifndef COMPILER_DRIVER
#define COMPILER_DRIVER
#include <string>
#include <map>
#include <iostream>
#include <fstream>

class FlexLexer;
class compiler_driver{
public:
	compiler_driver();
	virtual ~compiler_driver();

	std::map<std::string, int> variables;

	int result;
	void scan_begin();
	void scan_end();
	bool trace_scanning;

	void parse(const std::string &filename);
	std::ifstream* fs;

	void error(const std::string &msg);
	FlexLexer* lexer;
};

#endif
