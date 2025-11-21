#include "parser.hpp"
#include "statement.hpp"

Parser::Parser(std::vector<Token> tokens) {
	this->tokens = tokens;
}

void Parser::parse_statements() {
	statements.clear();
	current = 0;

	while(!at_end()) {
		statements.push_back(parse_statement());
	}
}

Statement Parser::parse_statement() {
	Token t = tokens.at(current);
	current++;

	switch(t.type) {
		case COLON:
			return parse_block_statement();
			break;
		case IF:
			return parse_if_statement();
			break;
		case DEF:
			return parse_function_statement();
		default:
			return ExpressionStatement(t);
		break;
	}
}

// IF ~ ELSE ~
Statement Parser::parse_if_statement() {
	Statement then_branch = parse_statement();
	if (peek().type == ELSE) {
		Statement else_branch = parse_statement();
	}
}

// DEF WORD ~
Statement Parser::parse_function_statement() {
	// add to functions
}

// : ~ ~ ... ;
Statement Parser::parse_block_statement() {
	std::vector<Statement> statements;

	// TODO:

	return BlockStatement(statements);
}

bool Parser::match_and_consume(TokenType type) {
	if (at_end()) return false;
	if (peek().type != type) return false;

	current++;
	return true;
}
