#include "statement.hpp"
#include "interpreter.hpp"
#include "token.hpp"
#include <iostream>

bool is_truthy(int n) {
	return n != 0;
}

void debug_log(Statement *statement, Interpreter *interpreter) {
	std::cout << "[" << statement->to_string() << "] ";
	interpreter->dump_stack();
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
		case BANG: {
			int a = interpreter->pop();
			interpreter->push(is_truthy(a) ? 0 : 1);
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
			int a = interpreter->pop();
			if (interpreter->no_print) break;
			std::cout << a;
			break;
		}
		case CHAR_PRINT: {
			char c = (char)interpreter->pop();
			if (interpreter->no_print) break;
			std::cout << c;
			break;
		}
		case PRINT_LN: {
			int a = interpreter->pop();
			if (interpreter->no_print) break;
			std::cout << a << std::endl;
			break;
		}
		case CHAR_PRINT_LN: {
			char c = (char)interpreter->pop();
			if (interpreter->no_print) break;
			std::cout << c << std::endl;
			break;
		}
		case LN: {
			if (interpreter->no_print) break;
			std::cout << std::endl;
			break;
		}
		case PEEK: {
			if (interpreter->no_print) break;
			std::cout << interpreter->peek();
			break;
		}
		case CHAR_PEEK: {
			if (interpreter->no_print) break;
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
			break;
		}
		// identifier
		case IDENTIFIER: {
			interpreter->call(token.lexeme);
			break;
		}
		// input
		case INPUT: {
			std::string input;
			*interpreter->get_istream() >> input;
			interpreter->push(std::stoi(input));
			break;
		}
		case CHAR_INPUT: {
			std::string input;
			*interpreter->get_istream() >> input;
			for (uint i = 0; i < input.length(); i++) {
				interpreter->push(input[i]);
			}
			break;
		}
		default: {
			interpreter->raise_error(token.line, "invalid expression statement token: " + token.lexeme);
		}
		break;
	}

	if (interpreter->debug) debug_log(this, interpreter);
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

	if (interpreter->debug) debug_log(this, interpreter);
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

RepeatStatement::RepeatStatement(Statement *statement) {
	this->statement = statement;
	
	this->line = statement->line;
}

RepeatStatement::~RepeatStatement() {
	delete statement;
}

void RepeatStatement::evaluate(Interpreter *interpreter) {
	int n = interpreter->pop();
	for (int i = 0; i < n; i++) {
		interpreter->push(i);
		statement->evaluate(interpreter);
	}
}

std::string RepeatStatement::to_string() {
	std::string res = "(Repeat)";
	if (statement != nullptr) res += " [" + statement->to_string() + "]";
	return res;
}

WhileStatement::WhileStatement(Statement *statement, Statement *condition) {
	this->statement = statement;
	this->condition = condition;
	
	this->line = statement->line;
}

WhileStatement::~WhileStatement() {
	delete statement;
	delete condition;
}

void WhileStatement::evaluate(Interpreter *interpreter) {
	do {
		statement->evaluate(interpreter);
		condition->evaluate(interpreter);
	} while (is_truthy(interpreter->pop()));
}

std::string WhileStatement::to_string() {
	std::string res = "(While)";
	if (statement != nullptr) res += " Statement: [" + statement->to_string() + "]";
	if (condition != nullptr) res += " Condition: [" + condition->to_string() + "]";
	return res;
}

PrintStatement::PrintStatement(Token string, bool new_line) {
	this->string = string.lexeme;
	this->new_line = new_line;

	this->line = string.line;
}

void PrintStatement::evaluate(Interpreter *interpreter) {
	if (interpreter->no_print) return;

	std::cout << string;
	if (new_line) std::cout << std::endl;

	if (interpreter->debug) debug_log(this, interpreter);
}

std::string PrintStatement::to_string() {
	return "(Print) " + string;
}
