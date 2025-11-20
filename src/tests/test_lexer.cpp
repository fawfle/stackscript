#include "../interpreter/lexer.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

std::string slurp(std::ifstream& in) {
    std::ostringstream sstr;
    sstr << in.rdbuf();
    return sstr.str();
}

int main(int argc, char *argv[]) {
	std::ifstream input_file{"./src/tests/programs/test_lexer.txt"};
	
	if (!input_file.is_open()) {
		std::cout << "Error opening file." << std::endl;
	}

	std::string file_string = slurp(input_file);
	std::cout << "File string: " << file_string << std::endl;

	Lexer lexer = Lexer(file_string);

	lexer.dump_tokens();
}
