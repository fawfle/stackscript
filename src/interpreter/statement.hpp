#ifndef STATEMENT_H
#define STATEMENT_H

class Interpreter;

#include "interpreter.hpp"
#include "./token.hpp"

#include <string>
#include <vector>

class Statement {
	public:
		int line = 0;
		virtual ~Statement() {};
		virtual void evaluate(Interpreter *interpreter)=0;
		virtual std::string to_string()=0;
};

// class for expressions
class ExpressionStatement : public Statement {
	Token token;
	public:
		ExpressionStatement(Token token);
		void evaluate(Interpreter *interpreter) override;
		std::string to_string() override;
};

class IfStatement : public Statement {
	Statement *then_branch = nullptr;
	Statement *else_branch = nullptr;
	public:
		IfStatement(Statement *then_branch);
		IfStatement(Statement *then_branch, Statement *else_branch);
		~IfStatement();
		void evaluate(Interpreter *interpreter) override;
		std::string to_string() override;
};

class FunctionStatement : public Statement {
	Token identifier;
	Statement *statement = nullptr;
	public:
		FunctionStatement(Token identifier, Statement *statement);
		~FunctionStatement();
		void evaluate(Interpreter *interpreter) override;
		std::string to_string() override;

		Token get_identifier() const { return identifier; };
		Statement *get_statement() const { return statement; };
};

class BlockStatement : public Statement {
	Token end_token;
	std::vector<Statement*> statements;
	public:
		BlockStatement(std::vector<Statement*> statements);
		~BlockStatement();
		void evaluate(Interpreter *interpreter) override;
		std::string to_string() override;
};

class RepeatStatement : public Statement {
	Statement *statement = nullptr;
	public:
		RepeatStatement(Statement *statement);
		~RepeatStatement();

		void evaluate(Interpreter *interpreter) override;
		std::string to_string() override;
};

class WhileStatement : public Statement {
	Statement *statement = nullptr;
	Statement *condition = nullptr;

	public:
		WhileStatement(Statement *statement, Statement *condition);
		~WhileStatement();

	void evaluate(Interpreter *interpreter) override;
	std::string to_string() override;
};

class PrintStatement : public Statement {
	std::string string;
	bool new_line = false;
	public:
		PrintStatement(Token string, bool new_line = false);
		void evaluate(Interpreter *interpreter) override;
		std::string to_string() override;
};

#endif
