GXX = g++

CXXFLAGS = -Wall -g -pedantic

TARGETS = ./src/data-structures/stack.cpp ./src/interpreter/lexer.cpp

STACK_TEST = ./build/stack_test
HASHTABLE_TEST = ./build/hashtable_test
SCANNER_TEST = ./build/lexer_test

tests:
	@mkdir -p build
	$(GXX) ./src/tests/test_stack.cpp $(TARGETS) -o $(STACK_TEST)
	$(GXX) ./src/tests/test_hashtable.cpp $(TARGETS) -o $(HASHTABLE_TEST)
	$(GXX) ./src/tests/test_lexer.cpp $(TARGETS) -o $(SCANNER_TEST)

clean:
	rm $(STACK_TEST)
	rm $(HASHTABLE_TEST)
	rm $(SCANNER_TEST)
