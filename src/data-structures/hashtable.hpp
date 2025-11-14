#include <iostream>
#include <stdexcept>

// should be prime
#define MIN_CAPACITY 23
#define LOAD_FACTOR 0.5

// a data class to store key and value for validating
template <typename K, typename V>
struct HashTableEntry {
	public:
		K key;
		V value;
};

// This implementation uses a dynamic array with a prime number size and quadratic probing to resolve collisions
// Sice we're using templates, the implementation needs to be in the header file
template  <typename K, typename V>
class HashTable {
	private:
		HashTableEntry<K, V> **entries = new HashTableEntry<K, V>*[MIN_CAPACITY];
		int capacity = MIN_CAPACITY;
		int num_elements = 0;
		
		void resize_array();

	public:
		int hash(K key) const;
		int find_index(K key) const;

		// add and set method for more specificity and explicit errors
		void add(K key, V value);
		void set(K key, V value);
		V& get(K key);
		
		bool remove(K key);

		~HashTable<K, V>();

		// overloaded operators for dictionary style access
		V& operator [] (K key) { return get(key); };
};

template <typename K, typename V>
int HashTable<K, V>::hash(K key) const {
	std::hash<std::string> hasher;
	return hasher(key) % capacity;
}

template <typename K, typename V>
int HashTable<K, V>::find_index(K key) const {
	int current_index = hash(key);
	int num_collisions = 0;
	while (entries[current_index] != nullptr && entries[current_index]->key != key) {
		// use quadratic probing to find next index. c1 = 0, c2 = 1
		// the difference sequence of x^2 is 2x-1. Therefore, add 2x + 1 where x is the number of collisions
		num_collisions++;
		current_index += 2 * num_collisions	+ 1;
		current_index %= capacity;
	}
	return current_index;
}

template <typename K, typename V>
void HashTable<K, V>::add(K key, V value) {
	int index = find_index(key);

	if (entries[index] != nullptr) throw std::invalid_argument("attempting to add entry with key that already exists");

	num_elements++;
	entries[index] = new HashTableEntry<K, V>();
	entries[index]->key = key;
	entries[index]->value = value;


	std::cout << "add key: " << key << " with val: " << value << std::endl;
}

template <typename K, typename V>
void HashTable<K, V>::set(K key, V value) {
	int index = find_index(key);

	if (entries[index] == nullptr) throw std::invalid_argument("attempting to set entry with key that doesn't exist");

	entries[index]->value = value;

	std::cout << "set key: " << key << " with val: " << value << std::endl;
}

template <typename K, typename V>
V& HashTable<K, V>::get(K key) {
	return entries[find_index(key)]->value;
}

template <typename K, typename V>
bool HashTable<K, V>::remove(K key) {
	int index = find_index(key);

	if (entries[index] == nullptr) throw std::invalid_argument("attempting to remove key that doesn't exist");

	num_elements--;
	delete entries[index];
	entries[index] = nullptr;
}

template <typename K, typename V>
HashTable<K, V>::~HashTable() {
	delete [] entries;
}
