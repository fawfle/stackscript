#include "../data-structures/hashtable.hpp"


#include <string>

int main(int argc, char *argv[]) {
	HashTable<std::string, int> table;

	// table.add("string", 1);
	std::cout << table["string"];
}
