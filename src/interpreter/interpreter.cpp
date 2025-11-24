#include "interpreter.hpp"
#include <iostream>

#define DEBUG true

Interpreter::Interpreter(std::vector<Statement*> statements) {
	this->statements = statements;
	this->functions = new HashTable<std::string, Statement*>;

	this->stack = new Stack();
}

void Interpreter::execute() {
	current = 0;
	had_error = false;

	while (!at_end()) {
		Statement *statement = statements.at(current);
		statement->evaluate(this);
		current++;

		if (DEBUG) {
			std::cout << "[" << statement->to_string() << "] ";
			dump_stack();
		}

		if (had_error) break;
	}
}

void Interpreter::push(int val) {
	stack->push(val);
}

int Interpreter::pop() {
	if (stack->is_empty()) {
		raise_error(current_statement()->line, "Cannot Pop, stack is empty.");
		return -1;
	}

	return stack->pop();
}

int Interpreter::peek() {
	if (stack->is_empty()) {
		raise_error(current_statement()->line, "Cannot peek, stack is empty.");
		return -1;
	}

	return stack->peek();
}

void Interpreter::call(std::string word) {
	if (!functions->contains(word)) {
		raise_error(current_statement()->line, "Call to unknown word " + word);
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

void Interpreter::dump_stack() const {
	std::cout << "STACK: " << stack->to_string("->") << std::endl;
}
