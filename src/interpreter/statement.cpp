#include "statement.hpp"
#include "interpreter.hpp"
#include "token.hpp"
#include <iostream>
#include <queue>

bool is_truthy(int n) {
	return n != 0;
}

ExpressionStatement::ExpressionStatement(Token token) {
	this->token = token;
	this->line = token.line;
}

void ExpressionStatement::evaluate(Interpreter *interpreter) {

	switch (token.type) {
		case PLUS: {
			int a = interpreter->pop();
			int b = interpreter->pop();
			interpreter->push(a + b);
			break;
		}
		case MINUS: {
			int a = interpreter->pop();
			int b = interpreter->pop();
			interpreter->push(b - a);
			break;
		}
		case STAR: {
			int a = interpreter->pop();
			int b = interpreter->pop();
			interpreter->push(a * b);
			break;
		}
		case SLASH: {
			int a = interpreter->pop();
			int b = interpreter->pop();
			interpreter->push(b / a);
			break;
		}
		case AND: {
			int a = interpreter->pop();
			int b = interpreter->pop();
			interpreter->push(is_truthy(a) && is_truthy(b) ? 1 : 0);
			break;
		}
		case OR: {
			int a = interpreter->pop();
			int b = interpreter->pop();
			interpreter->push(is_truthy(a) || is_truthy(b) ? 1 : 0);
			break;
			break;
		}
		case EQUAL: {
			int a = interpreter->pop();
			int b = interpreter->pop();
			interpreter->push(a == b ? 1 : 0);
			break;
		}
		case BANG_EQUAL: {
			int a = interpreter->pop();
			int b = interpreter->pop();
			interpreter->push(a != b ? 1 : 0);
			break;
		}
		case LESS: {
			int a  = interpreter->pop();
			int b = interpreter->pop();
			interpreter->push(b < a);
			break;
		}
		case LESS_EQUAL: {
			int a  = interpreter->pop();
			int b = interpreter->pop();
			interpreter->push(b <= a);
			break;
		}
		case GREATER: {
			int a  = interpreter->pop();
			int b = interpreter->pop();
			interpreter->push(b > a);
			break;
		}
		case GREATER_EQUAL: {
			int a  = interpreter->pop();
			int b = interpreter->pop();
			interpreter->push(b >= a);
			break;
		}
		case PRINT: {
			std::cout << interpreter->pop();
			break;
		}
		case CHAR_PRINT: {
			std::cout << (char)interpreter->pop();
			break;
		}
		case PRINT_LN: {
			std::cout << interpreter->pop() << std::endl;
			break;
		}
		case CHAR_PRINT_LN: {
			std::cout << (char)interpreter->pop() << std::endl;
			break;
		}
		case LN: {
			std::cout << std::endl;
			break;
		}
		case PEEK: {
			std::cout << interpreter->peek();
			break;
		}
		case CHAR_PEEK: {
			std::cout << (char)interpreter->peek();
			break;
		}
		// instructions
		case DROP: {
			interpreter->pop();
			break;
		}
		case DUP: {
			int a = interpreter->peek();
			interpreter->push(a);
			break;
		}
		case SWAP: {
			int a  = interpreter->pop();
			int b = interpreter->pop();
			interpreter->push(a);
			interpreter->push(b);
			break;
		}
		case N_SWAP: {
			int n = interpreter->pop();
			int *nums = new int[n];

			for (int i = 0; i < n; i++) {
				nums[i] = (interpreter->pop());
			}
			for (int i = 0; i < n; i++) {
				interpreter->push(nums[i]);
			}

			delete [] nums;
			break;
		}
		// literals
		case NUMBER: {
			interpreter->push(token.literal);
			break;
		}
		// when evaluated on own, adds a bunch of chars
		case STRING: {
			for (uint i = 0; i < token.lexeme.length(); i++) {
				interpreter->push(token.lexeme[i]);
			}
		}
		// identifier
		case IDENTIFIER: {
			interpreter->call(token.lexeme);
			break;
		}
		// input
		case INPUT: {
			std::string input;
			std::cin >> input;
			interpreter->push(std::stoi(input));
		}
		case CHAR_INPUT: {
			std::string input;
			std::cin >> input;
			for (uint i = 0; i < input.length(); i++) {
				interpreter->push(input[i]);
			}
		}
		default: {
			interpreter->raise_error(token.line, "invalid expression statement token");
		}
	}
}

std::string ExpressionStatement::to_string() {
	return "(Expression) " + token.lexeme + " [LINE " + std::to_string(token.line) + "]";
}

IfStatement::IfStatement(Statement *then_branch) {
	this->then_branch = then_branch;
	this->else_branch = nullptr;

	this->line = then_branch->line;
}

IfStatement::IfStatement(Statement *then_branch, Statement *else_branch) {
	this->then_branch = then_branch;
	this->else_branch = else_branch;

	this->line = then_branch->line;
}

IfStatement::~IfStatement() {
	delete then_branch;
	if (else_branch != nullptr) delete else_branch;
}

void IfStatement::evaluate(Interpreter *interpreter) {
	int n = interpreter->pop();
	if (is_truthy(n)) {
			then_branch->evaluate(interpreter);
	} else if (else_branch != nullptr) {
			else_branch->evaluate(interpreter);
	}
}

std::string IfStatement::to_string() {
	std::string res = "(If Statement)";
	if (then_branch != nullptr) res += " THEN: [" + then_branch->to_string() + "]";
	if (else_branch != nullptr) res += " ELSE: [" + else_branch->to_string() + "]";
	return res;
}

FunctionStatement::FunctionStatement(Token identifier, Statement *statement) {
	this->identifier = identifier;
	this->statement = statement;

	this->line = identifier.line;
}

FunctionStatement::~FunctionStatement() {
	delete statement;
}

void FunctionStatement::evaluate(Interpreter *interpreter) {
	interpreter->register_function(this);
}

std::string FunctionStatement::to_string() {
	std::string res = "(Function)";
	if (statement != nullptr) res += " [" + statement->to_string() + "]";
	return res;
}

BlockStatement::BlockStatement(std::vector<Statement*> statements) {
	this->statements = statements;

	this->line = statements.size() > 0 ? statements.at(0)->line : -1;
}

BlockStatement::~BlockStatement() {
	for (Statement *s : statements) {
		delete s;
	}
}

void BlockStatement::evaluate(Interpreter *interpreter) {
	for (Statement *s : statements) {
		s->evaluate(interpreter);
	}
}

std::string BlockStatement::to_string() {
	return "(Block) length: " + std::to_string(statements.size());
}

PrintStatement::PrintStatement(Token string, bool new_line) {
	this->string = string.lexeme;
	this->new_line = new_line;

	this->line = string.line;
}

void PrintStatement::evaluate(Interpreter *interpreter) {
	std::cout << string;
	if (new_line) std::cout << std::endl;
}

std::string PrintStatement::to_string() {
	return "(Print) " + string;
}
