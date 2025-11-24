#include <cmath>
#include <iostream>
#include <chrono>

#include "../data-structures/stack.hpp"

void test_stack(int size) {

		Stack stack = Stack();

		// push integers equal to their index onto the stack
		for (int j = 0; j < size; j++) {
			stack.push(j);
		}

		// pop integers and test that their value (the index) is what we expect
		for (int j = size - 1; j >= 0; j--) {
			int val = stack.pop();
			if (val != j) {
				std::cerr << "Error: incorrect value at stack index: " << j << std::endl;
			}
		}
}

int main(int argc, char *argv[]) {
	Stack stack;
	stack.push(1);
	stack.push(2);
	stack.push(3);
	std::cout << stack.to_string("->", 2) << std::endl;

	for (int i = 1; i < 30; i++) {
		int num_elements = std::pow(2, i);

		auto start_time = std::chrono::high_resolution_clock::now();

		test_stack(num_elements);

		auto end_time = std::chrono::high_resolution_clock::now();
		double elapsed_microseconds = std::chrono::duration<double, std::micro>(end_time - start_time).count();
		double microseconds_per_op = elapsed_microseconds / num_elements;

		std::cout << "For num_elements: " << num_elements << " (2^" << i << ") μs per operation is: " << microseconds_per_op << std::endl;
	}
}
