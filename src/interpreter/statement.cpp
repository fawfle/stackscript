#include "./statement.hpp"
#include "interpreter.hpp"
#include <iostream>

bool is_truthy(int n) {
	return n != 0;
}

ExpressionStatement::ExpressionStatement(Token token) {
	this->token = token;
}

void ExpressionStatement::evaluate(Interpreter &interpreter) {
	Stack &stack = interpreter.stack;

	switch (token.type) {
		case PLUS: {
			int a = stack.pop();
			int b = stack.pop();
			stack.push(a + b);
			break;
		}
		case MINUS: {
			int a = stack.pop();
			int b = stack.pop();
			stack.push(b - a);
			break;
		}
		case STAR: {
			int a = stack.pop();
			int b = stack.pop();
			stack.push(a * b);
			break;
		}
		case SLASH: {
			int a = stack.pop();
			int b = stack.pop();
			stack.push(b / a);
			break;
		}
		case AND: {
			int a = stack.pop();
			int b = stack.pop();
			stack.push(is_truthy(a) && is_truthy(b) ? 1 : 0);
			break;
		}
		case OR: {
			int a = stack.pop();
			int b = stack.pop();
			stack.push(is_truthy(a) || is_truthy(b) ? 1 : 0);
			break;
			break;
		}
		case EQUAL: {
			int a = stack.pop();
			int b = stack.pop();
			stack.push(a == b ? 1 : 0);
			break;
		}
		case BANG_EQUAL: {
			int a = stack.pop();
			int b = stack.pop();
			stack.push(a != b ? 1 : 0);
			break;
		}
		case LESS: {
			int a  = stack.pop();
			int b = stack.pop();
			stack.push(b < a);
			break;
		}
		case LESS_EQUAL: {
			int a  = stack.pop();
			int b = stack.pop();
			stack.push(b <= a);
			break;
		}
		case GREATER: {
			int a  = stack.pop();
			int b = stack.pop();
			stack.push(b > a);
			break;
		}
		case GREATER_EQUAL: {
			int a  = stack.pop();
			int b = stack.pop();
			stack.push(b >= a);
			break;
		}
		case PRINT: {
			std::cout << stack.pop();
			break;
		}
		case CHAR_PRINT: {
			std::cout << (char)stack.pop();
			break;
		}
		case PEEK: {
			std::cout << stack.peek();
			break;
		}
		case CHAR_PEEK: {
			std::cout << (char)stack.peek();
			break;
		}
		// instructions
		case DUP: {
			int a = stack.peek();
			stack.push(a);
			break;
		}
		case SWAP: {
			int a  = stack.pop();
			int b = stack.pop();
			stack.push(a);
			stack.push(b);
			break;
		}
		case N_SWAP: {
			Stack s;
			int n = stack.pop();
			for (int i = 0; i < n; i++) {
				s.push(stack.pop());
			}
			for (int i = 0; i < n; i++) {
				stack.push(s.pop());
			}
			break;
		}
		// literals
		case NUMBER: {
			stack.push(token.literal);
			break;
		}
		// when evaluated on own, adds a bunch of chars
		case STRING: {
			for (int i = 0; i < token.lexemme.length(); i++) {
				stack.push(token.lexemme[i]);
			}
		}
		// identifier
		case IDENTIFIER: {
			interpreter.call(token.lexemme);
			break;
		}
		// input
		case INPUT: {
			std::string input;
			std::cin >> input;
			stack.push(std::stoi(input));
		}
		case CHAR_INPUT: {
			std::string input;
			std::cin >> input;
			for (int i = 0; i < input.length(); i++) {
				stack.push(input[i]);
			}
		}
		default: {
			interpreter.raise_error(token.line, "invalid expression statement token");
		}
	}
}

IfStatement::IfStatement(Statement then_branch) {
	this->then_branch = then_branch;
}

IfStatement::IfStatement(Statement then_branch, Statement else_branch) {
	this->then_branch = then_branch;
	this->else_branch = else_branch;
}

void IfStatement::evaluate(Interpreter &interpreter) {
	int n = interpreter.stack.pop();
	if (is_truthy(n)) {
			then_branch.evaluate(interpreter);
	} else {
			else_branch.evaluate(interpreter);
	}
}

FunctionStatement::FunctionStatement(Statement statement) {
	this->statement = statement;
}

void FunctionStatement::evaluate(Interpreter &interpreter) {
		statement.evaluate(interpreter);
}

BlockStatement::BlockStatement(std::vector<Statement> statements) {
	this->statements = statements;
}

void BlockStatement::evaluate(Interpreter &interpreter) {
	for (Statement s : statements) {
		s.evaluate(interpreter);
	}
}
