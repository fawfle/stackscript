#include "../interpreter/lexer.hpp"
#include "../interpreter/parser.hpp"
#include "../interpreter/interpreter.hpp"

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
	std::ifstream input_file{"./src/tests/programs/test_interpreter.txt"};
	
	if (!input_file.is_open()) {
		std::cout << "Error opening file." << std::endl;
	}

	std::string file_string = slurp(input_file);
	std::cout << "File string: " << file_string << std::endl;

	Lexer lexer = Lexer(file_string);

	std::cout << "Dumping tokens..." << std::endl;
	lexer.dump_tokens();

	Parser parser = Parser(lexer.get_tokens());
	
	std::cout << "Dumping statements..." << std::endl;
	parser.dump_statements();

	std::cout << "Running..." << std::endl;
	Interpreter interpreter = Interpreter(parser.get_statements());

	interpreter.execute();
}
