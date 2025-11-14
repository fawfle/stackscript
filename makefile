GXX = g++

CXXFLAGS = -Wall -g -pedantic

TARGETS = ./src/data-structures/stack.cpp

tests:
	@mkdir -p build
	$(GXX) ./src/tests/test_stack.cpp $(TARGETS) -o ./build/test_stack
