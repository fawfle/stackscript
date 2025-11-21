#ifndef STATEMENT_H
#define STATEMENT_H

class Interpreter;

#include "interpreter.hpp"
#include "./token.hpp"
#include <vector>

class Statement {
	public:
		virtual void evaluate(Interpreter &interpreter);
};

// class for expressions
class ExpressionStatement : public Statement {
	Token token;
	public:
		ExpressionStatement(Token token);
		void evaluate(Interpreter &interpreter) override;
};

class IfStatement : public Statement {
	Statement then_branch;
	Statement else_branch;
	public:
		IfStatement(Statement then_branch);
		IfStatement(Statement then_branch, Statement else_branch);
		void evaluate(Interpreter &interpreter) override;
};

class FunctionStatement : public Statement {
	Statement statement;
	public:
		FunctionStatement(Statement statement);
		void evaluate(Interpreter &interpreter) override;
};

class BlockStatement : public Statement {
	Token end_token;
	std::vector<Statement> statements;
	public:
		BlockStatement(std::vector<Statement> statements);
		void evaluate(Interpreter &interpreter) override;
};

#endif
