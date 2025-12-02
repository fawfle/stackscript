#ifndef INTERPRETER_H
#define INTERPRETER_H

class Statement;
class FunctionStatement;

#include "statement.hpp"
#include "../data-structures/stack.hpp"
#include "../data-structures/hashtable.hpp"

#include <string>
#include <vector>

enum DebugLevel {
	NONE,
	NORMAL,
	ALL
};

class Interpreter {
	std::vector<Statement*> statements;
	HashTable<std::string, Statement*> *functions;

	std::istream *istream = &std::cin;

	Stack *stack;

	int current = 0;

	bool had_error = false;

	Statement *current_statement() { return statements.at(current); }
	bool at_end() const { return (uint)current >= statements.size(); }

	public:
		Interpreter(std::vector<Statement*> statements, DebugLevel debug_level = NONE);
		Interpreter(std::vector<Statement*> statements, std::istream *istream, DebugLevel debug_level = NONE);

		void execute();

		// wrappers for accessing stack. Allow for better error handling
		void push(int val);
		int pop();
		int peek();

		void call(std::string word);
		void register_function(FunctionStatement *function);

		void raise_error(int line, std::string message);
		bool get_had_error() const { return had_error; }

		std::istream *get_istream() { return istream; };

		void dump_stack() const;
		void print_stack() const;

		DebugLevel debug_level = NONE;
		// flag for supressing print statements when testing
		bool no_print = false;
};

#endif
