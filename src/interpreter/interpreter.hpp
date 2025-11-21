#ifndef INTERPRETER_H
#define INTERPRETER_H

class Statement;

#include "statement.hpp"
#include "../data-structures/stack.hpp"
#include "../data-structures/hashtable.hpp"

class Interpreter {
	private:
		HashTable<std::string, Statement> words;
	public:
		Stack stack;
		void call(std::string);

		void raise_error(int line, std::string message);
};

#endif
