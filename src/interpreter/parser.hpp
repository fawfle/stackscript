#ifndef PARSER_H
#define PARSER_H

#include "statement.hpp"
#include <vector>

class Parser {
	private:
		std::vector<Token> tokens;
		// statements must be pointers to ensure they're derived classes
		std::vector<Statement*> statements;
		
		int current = 0;

		void parse_statements();
		Statement *parse_statement();

		Statement *parse_if_statement();
		Statement *parse_function_statement();
		Statement *parse_block_statement();

		bool at_end() const { return peek().type == END; };

		Token peek() const { return tokens.at(current); }
		bool match_and_consume(TokenType type);

		bool had_error = false;
		void raise_error(int line, std::string message);

	public:
		Parser(std::vector<Token> tokens);

		void dump_statements() const;

		std::vector<Statement*> get_statements() const { return statements; };
};

#endif
