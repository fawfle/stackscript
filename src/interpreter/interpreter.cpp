#include "interpreter.hpp"
#include <iostream>

Interpreter::Interpreter(std::vector<Statement*> statements) {
	this->statements = statements;
	this->functions = new HashTable<std::string, Statement*>;

	this->stack = new Stack();
}

void Interpreter::execute() {
	current = 0;

	while (!at_end()) {
		statements.at(current)->evaluate(this);
		current++;

		if (had_error) break;
	}
}

void Interpreter::call(std::string word) {
	if (!functions->contains(word)) {
		raise_error(statements.at(current)->line, "Call to unknown word " + word);
	}

	functions->get(word)->evaluate(this);
}

void Interpreter::raise_error(int line, std::string message) {
	std::cerr << "[Line " << line << "] Runtime Error: " << message << std::endl;
	had_error = true;
}

void Interpreter::register_function(FunctionStatement *function) {
	Token identifier = function->get_identifier();
	if (functions->contains(identifier.lexeme)) {
		raise_error(identifier.line, "Multiple definitions of function with identifier " + identifier.lexeme);
		return;
	}

	functions->add(identifier.lexeme, function->get_statement());
}
