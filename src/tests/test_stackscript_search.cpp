#include "../interpreter/lexer.hpp"
#include "../interpreter/parser.hpp"
#include "../interpreter/interpreter.hpp"

#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <chrono>

std::string slurp(std::ifstream& in) {
    std::ostringstream sstr;
    sstr << in.rdbuf();
    return sstr.str();
}

double test_queue(Interpreter &interpreter, std::stringstream &ss, int size, int searches) {
	ss.clear();

	for (int i = 0; i < size; i++) {
		ss << "a " + std::to_string(i) + " ";
	}

	// search for 10 random elements
	for (int i = 0; i < searches; i++) {
		ss << "s " << std::rand() % size << " ";
	}

	ss << "e";

	auto start_time = std::chrono::high_resolution_clock::now();

	interpreter.execute();

	auto end_time = std::chrono::high_resolution_clock::now();
	double elapsed_microseconds = std::chrono::duration<double, std::micro>(end_time - start_time).count();
	double microseconds_per_op = elapsed_microseconds / searches;

	return microseconds_per_op;
}

int main(int argc, char *argv[]) {
	std::srand(std::time({}));
	std::ifstream input_file{"./src/programs/search.txt"};
	
	if (!input_file.is_open()) { std::cout << "Error opening file\n"; return 1; }

	std::string file_string = slurp(input_file);
	Lexer lexer = Lexer(file_string);
	if (lexer.get_had_error()) { std::cout << "Lexer error\n"; return 1; }

	Parser parser = Parser(lexer.get_tokens());
	if (parser.get_had_error()) { std::cout << "Parser error\n"; return 1; }

	
	// inject ss
	std::stringstream ss;

	Interpreter interpreter = Interpreter(parser.get_statements(), &ss);
	interpreter.no_print = true;

	for (int i = 0; i < 15; i++) {
		int n = std::pow(2, i);
		double microseconds_per_op = test_queue(interpreter, ss, n, 10);
		std::cout << "For num_entries: " << n << " (2^" << i << ") μs per op is: " << microseconds_per_op << "\n";
	}
}
