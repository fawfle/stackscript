#include <cmath>
#include <iostream>
#include <chrono>

#include "../data-structures/stack.hpp"

int main(int argc, char *argv[]) {
	for (int i = 1; i < 30; i++) {
		int num_elements = std::pow(2, i);

		Stack stack = Stack();

		auto start_time = std::chrono::high_resolution_clock::now();

		// push integers equal to their index onto the stack
		for (int j = 0; j < num_elements; j++) {
			stack.push(j);
		}

		// pop integers and test that their value (the index) is what we expect
		for (int j = num_elements - 1; j >= 0; j--) {
			int val = stack.pop();
			if (val != j) {
				std::cerr << "Error: incorrect value at stack index: " << j << std::endl;
			}
		}

		auto end_time = std::chrono::high_resolution_clock::now();
		double elapsed_microseconds = std::chrono::duration<double, std::micro>(end_time - start_time).count();
		double microseconds_per_op = elapsed_microseconds / num_elements;

		std::cout << "For num_elements: " << num_elements << " (2^" << i << ") μs per operation is: " << microseconds_per_op << std::endl;
	}
}
