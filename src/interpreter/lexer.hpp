#ifndef LEXER_H
#define LEXER_H

#include "../data-structures/hashtable.hpp"
#include "./token.hpp"

#include <vector>
#include <string>

// hacky way to get around not being able to make static data
// https://stackoverflow.com/questions/1197106/how-do-you-initialize-static-data-members-similar-to-static-constructors
class KeywordContainer {
		HashTable<std::string, TokenType> keywords;
	public:	
		bool contains(std::string word) const { return keywords.contains(word); };
		TokenType get(std::string word) const { return keywords.get(word); };
		KeywordContainer();
};

class Lexer {
	inline static KeywordContainer keywords;

	private:
		std::string source;
		std::vector<Token> tokens;

		int start = 0;
		int current = 0;
		int line = 1;

		void scan_tokens();
		void scan_token();

		void scan_string();
		void scan_number();
		void scan_identifier();

		void scan_conditional();

		char peek() const;
		bool match_and_consume(char target);

		void add_token(TokenType type);
		void add_token(TokenType type, int literal);

		std::string current_string() const;

		bool had_error = false;
		void raise_error(int line, std::string message);

		bool at_end() const;

		std::string token_to_string(Token t) const;

	public:
		Lexer(std::string source);

		void dump_tokens() const;
		std::vector<Token> get_tokens() const { return tokens; }
};

#endif
