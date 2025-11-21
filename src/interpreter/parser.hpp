#include "statement.hpp"
#include <vector>

class Parser {
	private:
		std::vector<Token> tokens;
		std::vector<Statement> statements;
		
		int current = 0;

		void parse_statements();
		Statement parse_statement();

		Statement parse_if_statement();
		Statement parse_function_statement();
		Statement parse_block_statement();

	
		bool at_end() const { return peek().type == END; };

		Token peek() const { return tokens.at(current); }
		bool match_and_consume(TokenType type);

	public:
		Parser(std::vector<Token> tokens);
};
