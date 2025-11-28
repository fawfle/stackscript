#include "./interpreter/lexer.hpp"
#include "./interpreter/parser.hpp"
#include "./interpreter/interpreter.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

// helper function to have entire file in a string
std::string slurp(std::ifstream& in) {
    std::ostringstream sstr;
    sstr << in.rdbuf();
    return sstr.str();
}

int main(int argc, char *argv[]) {
	if (argc <= 1) {
		std::cerr << "Expected an argument for which program to run." << std::endl;
		return 1;
	}

	std::string program_name = argv[1];

	std::ifstream input_file{"./src/programs/" + program_name + ".txt"};
	
	if (!input_file.is_open()) {
		std::cerr << "Error opening file." << std::endl;
	}

	std::string file_string = slurp(input_file);
	Lexer lexer = Lexer(file_string);
	if (lexer.get_had_error()) {
		std::cerr << "Stopping due to lexer error." << std::endl;
		return 1;
	}

	Parser parser = Parser(lexer.get_tokens());
	
	if (parser.get_had_error()) {
		std::cerr << "Stopping due to parser error." << std::endl;
		return 1;
	}

	Interpreter interpreter = Interpreter(parser.get_statements());

	interpreter.execute();
}
