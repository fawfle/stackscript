#ifndef INTERPRETER_H
#define INTERPRETER_H

class Statement;
class FunctionStatement;

#include "statement.hpp"
#include "../data-structures/stack.hpp"
#include "../data-structures/hashtable.hpp"

#include <string>
#include <vector>

class Interpreter {
	std::vector<Statement*> statements;
	HashTable<std::string, Statement*> *functions;

	Stack *stack;

	int current = 0;

	bool had_error = false;

	Statement *current_statement() { return statements.at(current); }
	bool at_end() const { return (uint)current >= statements.size(); }

	bool debug = false;

	public:
		Interpreter(std::vector<Statement*> statements, bool debug = false);

		void execute();

		// wrappers for accessing stack. Allow for better error handling
		void push(int val);
		int pop();
		int peek();

		void call(std::string word);
		void register_function(FunctionStatement *function);

		void raise_error(int line, std::string message);
		bool get_had_error() const { return had_error; }

		void dump_stack() const;
};

#endif
