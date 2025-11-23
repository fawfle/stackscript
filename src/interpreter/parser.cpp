#include "parser.hpp"
#include "statement.hpp"
#include "token.hpp"
#include <iostream>

Parser::Parser(std::vector<Token> tokens) {
	this->tokens = tokens;
	
	parse_statements();
}

void Parser::parse_statements() {
	statements.clear();
	current = 0;

	while(!at_end()) {
		statements.push_back(parse_statement());
	}
}

Statement *Parser::parse_statement() {
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
			// "string" print is interpreted as printing the whole string.
			Token next = peek();
			if (t.type == STRING && (match_and_consume(PRINT) || match_and_consume(CHAR_PRINT) || match_and_consume(PRINT_LN) || match_and_consume(CHAR_PRINT_LN))) {
				return new PrintStatement(t, next.type == PRINT_LN || next.type == CHAR_PRINT_LN);
			}

			// handle unexpected tokens
			switch (t.type) {
				case ELSE:
				case SEMICOLON:
					raise_error(t.line, "Unexpected token '" + t.lexeme + "'");
					return nullptr;
					break;
				default: break;
			}

			return new ExpressionStatement(t);
		break;
	}
}

// IF ~ ELSE ~
Statement *Parser::parse_if_statement() {
	Statement *then_branch = parse_statement();
	if (match_and_consume(ELSE)) {
		Statement *else_branch = parse_statement();
		return new IfStatement(then_branch, else_branch);
	}

	return new IfStatement(then_branch);
}

// DEF WORD ~
Statement *Parser::parse_function_statement() {
	// first token should be an identifier with the name
	Token next = peek();
	if (next.type != IDENTIFIER) {
		raise_error(peek().line, "Expected identifier following function declaration.");
		return nullptr;
	}

	// consume identifier
	current++;

	return new FunctionStatement(next, parse_statement());
}

// : ~ ~ ... ;
Statement *Parser::parse_block_statement() {
	std::vector<Statement*> statements;

	// go until ';'
	while (!at_end() && !match_and_consume(SEMICOLON)) {
		statements.push_back(parse_statement());
	}

	return new BlockStatement(statements);
}

bool Parser::match_and_consume(TokenType type) {
	if (at_end()) return false;
	if (peek().type != type) return false;

	current++;
	return true;
}

void Parser::dump_statements() const {
	for (Statement *s : statements) {
		std::cout << s->to_string() << std::endl;
	}
}

void Parser::raise_error(int line, std::string message) {
	std::cerr << "[Line " << line << "] Parser Error: " << message << std::endl;
	had_error = true;
}
