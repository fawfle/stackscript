#include "./interpreter/lexer.hpp"
#include "./interpreter/parser.hpp"
#include "./interpreter/interpreter.hpp"

#include <cstring>
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
	DebugLevel debug_level = NONE;

	// look for flags
	for (int i = 0; i < argc; i++) {
		if (strcmp(argv[i], "-d") == 0) debug_level = NORMAL;
		if (strcmp(argv[i], "-da") == 0) debug_level = ALL;
	}

	std::ifstream input_file{"./src/programs/" + program_name + ".txt"};
	
	if (!input_file.is_open()) {
		std::cerr << "Error opening file." << std::endl;
		return 1;
	}

	if (debug_level > NONE) std::cout << "DEBUG MODE ENABLED [" << (debug_level == ALL ? "ALL" : "NORMAL") << "]\n";

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

	Interpreter interpreter = Interpreter(parser.get_statements(), debug_level);

	interpreter.execute();
}
