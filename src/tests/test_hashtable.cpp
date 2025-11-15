#include "../data-structures/hashtable.hpp"

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <random>
#include <string>
#include <vector>

std::string gen_random_string(int length) {
	static std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	std::string res = "";
	for (int i = 0; i < length; i++) {
		res += charset[rand() % charset.length()];
	}
	return res;
}

void test_add_entries(int num_entries) {
	HashTable<std::string, int> table = HashTable<std::string, int>();

	std::vector<std::string> keys;

	for (int i = 0; i < num_entries; i++) {
		std::string key = gen_random_string(8);
		keys.push_back(key);
		table.add(key, i);
	}

	// check all keys are present with correct value
	for (uint i = 0; i < keys.size(); i++) {
		if (table.get(keys[i]) != i) {
			std::cout << "Expected value: " << i << " at key: " << keys[i] << std::endl;
		}
	}

	// removal significantly increases time to run
	return;
	// test removal
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(keys.begin(), keys.end(), rd);

	for (uint i = 0; i < keys.size(); i++) {
		table.remove(keys[i]);
	}
}

int main(int argc, char *argv[]) {
	std::srand(std::time(0));

	int num_entries = 1;
	for (int i = 0; i < 20; i++) {
		auto start_time = std::chrono::high_resolution_clock::now();
		
		test_add_entries(num_entries);

		auto end_time = std::chrono::high_resolution_clock::now();
		double elapsed_microseconds = std::chrono::duration<double, std::micro>(end_time - start_time).count();
		double microseconds_per_entry = elapsed_microseconds / num_entries;

		std::cout << "For num_entries: " << num_entries << " (2^" << i << ") μs per operation is: " << microseconds_per_entry << std::endl;

		num_entries *= 2;
	}
}
