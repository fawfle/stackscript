#include "interpreter.hpp"
#include <iostream>
#include <stdexcept>

Interpreter::Interpreter(std::vector<Statement*> statements, bool debug) {
	this->statements = statements;
	this->debug = debug;

	this->functions = nullptr;
	this->stack = nullptr;

	this->istream = &std::cin;
}

Interpreter::Interpreter(std::vector<Statement*> statements, std::istream *istream, bool debug) {
	this->statements = statements;
	this->debug = debug;

	this->functions = nullptr;
	this->stack = nullptr;

	this->istream = istream;
}

void Interpreter::execute() {
	current = 0;
	had_error = false;

	if (stack != nullptr) delete stack;
	stack = new Stack();

	if (functions != nullptr) delete functions;
	functions = new HashTable<std::string, Statement*>;

	while (!at_end()) {
		Statement *statement = statements.at(current);
		statement->evaluate(this);
		current++;

		// if (debug) {
		// 	std::cout << "[" << statement->to_string() << "] ";
		// 	dump_stack();
		// }

		if (had_error) break;
	}
}

void Interpreter::push(int val) {
	stack->push(val);
}

int Interpreter::pop() {
	if (stack->is_empty()) {
		raise_error(current_statement()->line, "Cannot Pop, stack is empty");
		return -1;
	}

	return stack->pop();
}

int Interpreter::peek() {
	if (stack->is_empty()) {
		raise_error(current_statement()->line, "Cannot peek, stack is empty");
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
	throw std::invalid_argument("[Line " + std::to_string(line) + "] Runtime Error: " + message);
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
