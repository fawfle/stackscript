#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>
#include <stdexcept>
#include <string>

// should be prime
#define MIN_TABLE_CAPACITY 23
#define LOAD_FACTOR 0.50

#define MAX_CHECKS 15

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
		HashTableEntry<K, V> **entries = nullptr;
		int capacity = 0;
		int num_elements = 0;
		
		// since entries stores pointers to entries, they must manually be deallocated with free_table on destruction
		void free_table();
		void resize_table();

		// "method for finding index in list". Still uses own hash function. Meant for copying
		// typing gets really weird with constant methods. Here it's pass by constant pointer (const methods convert members to constant pointers)
		int find_open_index_in_entry_list(HashTableEntry<K, V> **const& entry_list, K key) const;

	public:
		HashTable();

		int hash(K key) const;
		// use quadratic probing with c1 = 0 and c2 = 1
		int probe(int start, int i) const { return (start + i * i) % capacity; }
		// find valid location for new keys
		int find_open_index(K key) const;
		// find location of key
		int find_key_index(K key) const;

		// add and set method for more specificity and explicit errors
		void add(K key, V value);
		void set(K key, V value);
		V& get(K key) const;
		
		void remove(K key);

		bool contains(K key) const;

		~HashTable();

		// overloaded operators for dictionary style access
		V& operator [] (K key) const { return get(key); };
};

inline bool is_prime(int n) {
	for (int i = 2; i < n; i++) {
		if (n % i == 0) return false;
	}
	return true;
}

// find the first prime number at or above start
inline int find_closest_prime(int num) {
	while (!is_prime(num)) {
		num++;
	}
	return num;
}

template <typename K, typename V>
HashTable<K, V>::HashTable() {
	capacity = MIN_TABLE_CAPACITY;
	num_elements = 0;

	entries = new HashTableEntry<K, V>*[capacity]();
}

template <typename K, typename V>
void HashTable<K, V>::free_table() {
	for (int i = 0; i < capacity; i++) {
		if (entries[i] != nullptr) {
			delete entries[i];
			entries[i] = nullptr;
		}
	}
}

template <typename K, typename V>
void HashTable<K, V>::resize_table() {
	capacity *= find_closest_prime(capacity * 2);
	// parenthesis are really important here, they initialize the list to nullptrs
	HashTableEntry<K, V> **new_entries = new HashTableEntry<K, V>*[capacity]();

	// move entries until we've moved all of them instead of iterating over every entry. Degenerates to the equivalent.
	// num_elements - 1 since we increment num_elements before but don't add final element
	int search_index = 0;
	int moved = 0;
	while (moved < num_elements - 1) {
		HashTableEntry<K, V> *entry = entries[search_index];
		search_index++;
		if (entry == nullptr) continue;
		int new_index = find_open_index_in_entry_list(new_entries, entry->key);
		new_entries[new_index] = entry;
		moved++;
	}

	if (entries != nullptr) delete [] entries;
	entries = new_entries;
}

template <typename K, typename V>
int HashTable<K, V>::hash(K key) const {
	std::hash<K> hasher;
	return hasher(key) % capacity;
}

// method for finding free index in a variable entries. Used for copying
template <typename K, typename V>
int HashTable<K, V>::find_open_index_in_entry_list(HashTableEntry<K, V> ** const& entry_list, K key) const {
	int start_index = hash(key);
	int current_index = start_index;
	int num_collisions = 0;
	// rather than looking for open indices, specifically look for key
	while (entry_list[current_index] != nullptr && entry_list[current_index]->key != key) {
		// use quadratic probing to find next index. c1 = 0, c2 = 1
		num_collisions++;
		current_index = probe(start_index, num_collisions);
	}
	return current_index;
}

template <typename K, typename V>
int HashTable<K, V>::find_open_index(K key) const {
	return find_open_index_in_entry_list(entries, key);
}

template <typename K, typename V>
int HashTable<K, V>::find_key_index(K key) const {
	int start_index = hash(key);
	int current_index = start_index;
	int num_checks = 0;
	while (num_checks < MAX_CHECKS) {
		if (entries[current_index] != nullptr && entries[current_index]->key == key) return current_index;

		num_checks++;
		current_index = probe(start_index, num_checks);
	}

	// if probing fails for MAX_CHECKS, switch to checking every value. Not the most efficient, but ensures it will always work.
	for (int i = 0; i < capacity; i++) {
		if (entries[i] != nullptr && entries[i]->key == key) return i;
	}

	throw std::invalid_argument("failed to find key in hash table.");
}

template <typename K, typename V>
void HashTable<K, V>::add(K key, V value) {
	int index = find_open_index(key);

	if (entries != nullptr && entries[index] != nullptr) throw std::invalid_argument("attempting to add entry to table with key that already exists");

	num_elements++;

	if (num_elements > capacity * LOAD_FACTOR) {
		resize_table();
		// update index with new hash function
		index = find_open_index(key);
	}

	entries[index] = new HashTableEntry<K, V>();
	entries[index]->key = key;
	entries[index]->value = value;
}

template <typename K, typename V>
void HashTable<K, V>::set(K key, V value) {
	int index = find_key_index(key);

	if (entries[index] == nullptr) throw std::invalid_argument("attempting to set entry of table with key that doesn't exist");

	entries[index]->value = value;
}

template <typename K, typename V>
V& HashTable<K, V>::get(K key) const {
	auto entry = entries[find_key_index(key)];
	if (entry == nullptr) throw std::invalid_argument("attempting to find entry wth key that doesn't exist in table");
	return entry->value;
}

template <typename K, typename V>
void HashTable<K, V>::remove(K key) {
	int index = find_key_index(key);

	if (entries[index] == nullptr) throw std::invalid_argument("attempting to remove key that doesn't exist in table");

	num_elements--;
	delete entries[index];
	entries[index] = nullptr;
}

template <typename K, typename V>
bool HashTable<K, V>::contains(K key) const {
	try {
		find_key_index(key);
	}
	catch (std::invalid_argument &exception) {
		return false;
	}
	return true;
}

template <typename K, typename V>
HashTable<K, V>::~HashTable() {
	free_table();
	if (entries != nullptr) delete [] entries;
}

#endif
