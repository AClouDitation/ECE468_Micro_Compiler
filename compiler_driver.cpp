#include "compiler_driver.hpp"

compiler_driver::compiler_driver(){
}

compiler_driver::~compiler_driver(){
}

void compiler_driver::parse(const std::string &fn){
	fs = new std::ifstream();
	//TODO: error handling if open failed
	fs -> open(fn,std::ios::in);
	scan_begin();
	scan_end();
	delete fs;
}

void compiler_driver::error(const std::string &msg){
	std::cerr << msg << std::endl;
}
