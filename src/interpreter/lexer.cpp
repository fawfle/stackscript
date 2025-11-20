#include "lexer.hpp"
#include <iostream>
#include <string>

bool is_digit(char c) {
	return c >= '0' && c <= '9';
}

bool is_alphabetic(char c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool is_alphanumeric(char c) {
	return is_alphabetic(c) || is_digit(c);
}

// this is horrible
std::string token_type_to_string(TokenType type) {
	switch (type) {
		case LEFT_PAREN: return "LEFT_PAREN"; case RIGHT_PAREN: return "RIGHT_PAREN";
		case LEFT_BRACE: return "LEFT_BRACE"; case RIGHT_BRACE: return "RIGHT_BRACE";
		case COLON: return "COLON"; case SEMICOLON: return "SEMICOLON";
		case MINUS: return "MINUS"; case PLUS: return "PLUS"; case STAR: return "STAR"; case SLASH: return "SLASH";
		case AND: return "AND"; case OR: return "OR";
		case BANG: return "BANG"; case BANG_EQUAL: return "BANG_EQUAL";
		case EQUAL: return "EQUAL";
		case GREATER: return "GREATER"; case GREATER_EQUAL: return "GREATER_EQUAL"; case LESS: return "LESS"; case LESS_EQUAL: return "LESS_EQUAL";
		case IDENTIFIER: return "IDENTIFIER"; case NUMBER: return "NUMBER";
		case IF: return "IF"; case ELSE: return "ELSE";
		case DEF: return "DEF";
		case PRINT: return "PRINT"; case CHAR_PRINT: return "CHAR_PRINT"; case PEEK: return "PEEK";
		case DUP: return "DUP"; case SWAP: return "SWAP"; case N_SWAP: return "N_SWAP";
		case HASHTAG: return "HASHTAG";
		case END: return "END";
	}
	return "";
}

// statically initialize map of keywords
KeywordContainer::KeywordContainer() {
	keywords.add("IF", IF);
	keywords.add("ELSE", ELSE);
	keywords.add("DEF", DEF);
	keywords.add("print", PRINT);
	keywords.add("cprint", CHAR_PRINT);
	keywords.add("p", PRINT);
	keywords.add("cp", CHAR_PRINT);
	keywords.add("peek", PEEK);
	keywords.add("dup", DUP);
	keywords.add("swap", SWAP);
	keywords.add("nswap", N_SWAP);
}

// Need to handle edge cases
Lexer::Lexer(std::string source) {
	this->source = source;

	scan_tokens();
}

void Lexer::scan_tokens() {
	this->tokens.clear();
	line = 1;

	while(!at_end()) {
		start = current;
		scan_token();
	}

	tokens.push_back(Token{END, "", 0, 1});
}

void Lexer::scan_token() {
	char c = source[current];
	current++;

	// check tokens
	switch (c) {
		case '(': add_token(LEFT_PAREN); break;
		case ')': add_token(RIGHT_PAREN); break;
		case '{': add_token(LEFT_BRACE); break;
		case '}': add_token(RIGHT_BRACE); break;
		case ':': add_token(COLON); break;
		case ';': add_token(SEMICOLON); break;

		// since these represent comments, skip to newline
		case '#': 
			add_token(HASHTAG); 
			while (peek() != '\n' && !at_end()) current++;
			break;

		case '+': add_token(PLUS); break;
		case '-': add_token(MINUS); break;
		case '*': add_token(STAR); break;
		case '/': add_token(SLASH); break;

		case '&': add_token(AND); break;
		case '|':
			if (match_and_consume('|')) add_token(OR);
			else raise_error(line, "Possibly unmatched or.");
			break;
		
		// '!' operator is more complicated as it can exist as '!' or '!='
		case '!': add_token(match_and_consume('=')? BANG_EQUAL : BANG); break;
		// same goes for '<' and '>'
		case '>': add_token(match_and_consume('=') ? GREATER_EQUAL : GREATER); break;
		case '<': add_token(match_and_consume('=') ? LESS_EQUAL : LESS); break;

		case '"': scan_string(); break;

		// ignore whitespace
		case ' ':
		case '\r':
		case '\t':
		break;

		case '\n': line++; break;
		

		default:
			// numbers start with a digit
			if (is_digit(c)) {
				scan_number();
				break;
			}
			// identifiers start with letters
			if (is_alphabetic(c)) {
				scan_identifier();
				break;
			}

			// I love appending chars to strings
			std::string message = "Unexpected Character: ";
			message += c;
			raise_error(line, message);
			break;
	}
}

// scans string 
void Lexer::scan_string() {
	while (!at_end() && peek() != '\"' && peek() != '\n') {
		start = current;
		char c = source[current];
		current++;
		// for practical reasons, "strings" and chars are treated as lists of individual numbers to be pushed onto the stack
		add_token(NUMBER, c);
	}

	if (at_end() || peek() == '\n') {
		raise_error(line, "Unterminated string.");
		return;
	}

	// consume endquote
	current++;
}

void Lexer::scan_number() {
	while (is_digit(peek())) {
		std::cout << "peeked";
		current++;
	}

	add_token(NUMBER, std::stoi(current_string()));
}

void Lexer::scan_identifier() {
	while (is_alphanumeric(peek())) current++;
	
	TokenType type;
	std::string word = current_string();
	if (Lexer::keywords.contains(word)) type = Lexer::keywords.get(word);
	else type = IDENTIFIER;

	add_token(type);
}

char Lexer::peek() const {
	if (at_end()) return '\0';
	return source[current];
}

// checks if next char is equal to c and advances current if it is.
bool Lexer::match_and_consume(char target) {
	if (at_end()) return false;
	if (source[current] != target) return false;

	current++;
	return true;
}

void Lexer::add_token(TokenType type) {
	add_token(type, 0);
}

void Lexer::add_token(TokenType type, int literal) {
	std::string text = current_string();
	tokens.push_back(Token{type, text, literal, line});
}

std::string Lexer::current_string() const {
	return source.substr(start, current - start);
}

bool Lexer::at_end() const {
	return current >= source.length();
}

void Lexer::raise_error(int line, std::string message) {
	std::cerr << "[Line " << line << "] Error: " << message << std::endl;
	had_error = true;
}

void Lexer::dump_tokens() const {
	for (Token t : tokens) {
		std::cout << token_to_string(t) << std::endl;
	}
}

std::string Lexer::token_to_string(Token t) const {
	std::string s = "(TYPE) " + token_type_to_string(t.type) + " (TEXT) " + t.lexemme;

	if (t.type == NUMBER) s += " (LITERAL) " + std::to_string(t.literal);
	s += " [LINE " + std::to_string(line) + "]";

	return s;
}
