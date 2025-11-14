#include "stack.hpp"
#include <stdexcept>

// resize the dynamic array to a larger capacity
void Stack::resize_array() {
	int *new_array = new int[capacity * GROWTH_FACTOR];
	
	for (int i = 0; i < capacity; i++) {
		new_array[i] = array[i];
	}

	capacity *= GROWTH_FACTOR;
	delete [] array;
	array = new_array;
}

void Stack::push(int val) {
	size++;
	if (size >= capacity) resize_array();

	array[size - 1] = val;
}

int Stack::pop() {
	if (size == 0) throw std::invalid_argument("attempted to pop value off empty stack");

	size--;
	return array[size];
}

int Stack::peek() const {
	if (size == 0) throw std::invalid_argument("attempted to pop value off empty stack");

	return array[size - 1];
}

Stack::~Stack() {
	delete [] array;
}
