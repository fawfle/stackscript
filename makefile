GXX = g++

CXXFLAGS = --std=c++17 -Wall -g -pedantic

OBJ_DIR := ./build/obj
BUILD_DIR = ./build

SRC_DIR = ./src
DS_DIR = ./src/data-structures
INTERPRETER_DIR = ./src/interpreter

SRC_FILES := $(wildcard $(DS_DIR)/*.cpp) $(wildcard $(INTERPRETER_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

TARGETS = ./src/data-structures/stack.cpp ./src/interpreter/lexer.cpp ./src/interpreter/interpreter.cpp ./src/interpreter/statement.cpp

STACK_TEST = ./build/stack_test
HASHTABLE_TEST = ./build/hashtable_test
LEXER_TEST = ./build/lexer_test
PARSER_TEST = ./build/parser_test
INTERPRETER_TEST = ./build/interpreter_test

STACKSCRIPT_QUEUE_TEST = ./build/ss_queue_test
STACKSCRIPT_SEARCH_TEST = ./build/ss_search_test
STACKSCRIPT_INSORT_TEST = ./build/ss_insort_test
STACKSCRIPT_SORT_TEST = ./build/ss_sort_test

STACKSCRIPT_EXECUTABLE = ./build/stackscript

all: executable tests

tests: $(OBJ_FILES) | directories
	@echo making test files
	@$(GXX) $(CXXFLAGS) ./src/tests/test_stack.cpp $(OBJ_FILES) -o $(STACK_TEST)
	@$(GXX) $(CXXFLAGS) ./src/tests/test_hashtable.cpp $(OBJ_FILES) -o $(HASHTABLE_TEST)
	@$(GXX) $(CXXFLAGS) ./src/tests/test_lexer.cpp $(OBJ_FILES) -o $(LEXER_TEST)
	@$(GXX) $(CXXFLAGS) ./src/tests/test_parser.cpp $(OBJ_FILES) -o $(PARSER_TEST)
	@$(GXX) $(CXXFLAGS) ./src/tests/test_interpreter.cpp $(OBJ_FILES) -o $(INTERPRETER_TEST)

	@echo making program test files
	@$(GXX) $(CXXFLAGS) ./src/tests/test_stackscript_queue.cpp $(OBJ_FILES) -o $(STACKSCRIPT_QUEUE_TEST)
	@$(GXX) $(CXXFLAGS) ./src/tests/test_stackscript_search.cpp $(OBJ_FILES) -o $(STACKSCRIPT_SEARCH_TEST)
	@$(GXX) $(CXXFLAGS) ./src/tests/test_stackscript_insort.cpp $(OBJ_FILES) -o $(STACKSCRIPT_INSORT_TEST)
	@$(GXX) $(CXXFLAGS) ./src/tests/test_stackscript_sort.cpp $(OBJ_FILES) -o $(STACKSCRIPT_SORT_TEST)

executable: $(OBJ_FILES) | directories
	@echo making stackscript executable
	@$(GXX) $(CXXFLAGS) ./src/stackscript.cpp $(OBJ_FILES) -o $(STACKSCRIPT_EXECUTABLE)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | directories
	@mkdir -p $(dir $@)
	@$(GXX) $(CXXFLAGS) -c -o $@ $<

directories:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(OBJ_DIR)

clean:
	@rm -rf $(OBJ_DIR)
	@rm -rf $(BUILD_DIR)
