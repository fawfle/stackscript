#include "statement.hpp"
#include "interpreter.hpp"
#include "token.hpp"
#include <iostream>

bool is_truthy(int n) {
	return n != 0;
}

ExpressionStatement::ExpressionStatement(Token token) {
	this->token = token;
	this->line = token.line;
}

void ExpressionStatement::evaluate(Interpreter *interpreter) {
	Stack *stack = interpreter->stack;

	switch (token.type) {
		case PLUS: {
			int a = stack->pop();
			int b = stack->pop();
			stack->push(a + b);
			break;
		}
		case MINUS: {
			int a = stack->pop();
			int b = stack->pop();
			stack->push(b - a);
			break;
		}
		case STAR: {
			int a = stack->pop();
			int b = stack->pop();
			stack->push(a * b);
			break;
		}
		case SLASH: {
			int a = stack->pop();
			int b = stack->pop();
			stack->push(b / a);
			break;
		}
		case AND: {
			int a = stack->pop();
			int b = stack->pop();
			stack->push(is_truthy(a) && is_truthy(b) ? 1 : 0);
			break;
		}
		case OR: {
			int a = stack->pop();
			int b = stack->pop();
			stack->push(is_truthy(a) || is_truthy(b) ? 1 : 0);
			break;
			break;
		}
		case EQUAL: {
			int a = stack->pop();
			int b = stack->pop();
			stack->push(a == b ? 1 : 0);
			break;
		}
		case BANG_EQUAL: {
			int a = stack->pop();
			int b = stack->pop();
			stack->push(a != b ? 1 : 0);
			break;
		}
		case LESS: {
			int a  = stack->pop();
			int b = stack->pop();
			stack->push(b < a);
			break;
		}
		case LESS_EQUAL: {
			int a  = stack->pop();
			int b = stack->pop();
			stack->push(b <= a);
			break;
		}
		case GREATER: {
			int a  = stack->pop();
			int b = stack->pop();
			stack->push(b > a);
			break;
		}
		case GREATER_EQUAL: {
			int a  = stack->pop();
			int b = stack->pop();
			stack->push(b >= a);
			break;
		}
		case PRINT: {
			std::cout << stack->pop();
			break;
		}
		case CHAR_PRINT: {
			std::cout << (char)stack->pop();
			break;
		}
		case PRINT_LN: {
			std::cout << stack->pop() << std::endl;
			break;
		}
		case CHAR_PRINT_LN: {
			std::cout << (char)stack->pop() << std::endl;
			break;
		}
		case LN: {
			std::cout << std::endl;
			break;
		}
		case PEEK: {
			std::cout << stack->peek();
			break;
		}
		case CHAR_PEEK: {
			std::cout << (char)stack->peek();
			break;
		}
		// instructions
		case DUP: {
			int a = stack->peek();
			stack->push(a);
			break;
		}
		case SWAP: {
			int a  = stack->pop();
			int b = stack->pop();
			stack->push(a);
			stack->push(b);
			break;
		}
		case N_SWAP: {
			Stack s;
			int n = stack->pop();
			for (int i = 0; i < n; i++) {
				s.push(stack->pop());
			}
			for (int i = 0; i < n; i++) {
				stack->push(s.pop());
			}
			break;
		}
		// literals
		case NUMBER: {
			stack->push(token.literal);
			break;
		}
		// when evaluated on own, adds a bunch of chars
		case STRING: {
			for (uint i = 0; i < token.lexeme.length(); i++) {
				stack->push(token.lexeme[i]);
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
			stack->push(std::stoi(input));
		}
		case CHAR_INPUT: {
			std::string input;
			std::cin >> input;
			for (uint i = 0; i < input.length(); i++) {
				stack->push(input[i]);
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
	int n = interpreter->stack->pop();
	if (is_truthy(n)) {
			then_branch->evaluate(interpreter);
	} else if (else_branch != nullptr) {
			else_branch->evaluate(interpreter);
	}
}

std::string IfStatement::to_string() {
	std::string res = "(If Statement) THEN: [" + then_branch->to_string() + "]";
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
	return "(Function) [" + statement->to_string() + "]"; 
}

BlockStatement::BlockStatement(std::vector<Statement*> statements) {
	this->statements = statements;

	this->line = statements.at(0)->line;
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
