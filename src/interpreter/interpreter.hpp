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

	int current = 0;

	bool had_error = false;

	bool at_end() const { return (uint)current >= statements.size(); }

	public:
		Interpreter(std::vector<Statement*> statements);

		Stack *stack;

		void execute();

		void call(std::string word);
		void register_function(FunctionStatement *function);

		void raise_error(int line, std::string message);
};

#endif
