#include "compiler_driver.hpp"

int main(int argc, char** argv){
	compiler_driver driver;
	for (++argv; *argv; ++argv){
		driver.parse(*argv);	
		break;
	}		
}
