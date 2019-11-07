#pragma once

#include <stack>
#include <vector>

#include "algorithm.hpp"

// Data Structures
template <typename T>
struct Stack : std::stack<T> {
	T rpop() {
		T t = top();
		pop();
		return t;
	}
};


template <typename K, typename T> // T must be default initializable
struct record_{
	K key;
	T value;
};


template <typename K, typename T>
bool operator<(const record_<K, T>& r1, const record_<K, T>& r2) {
	return r1.key < r2.key;
}

template <typename K, typename T>
struct Hash {

	T& lookup_or_insert_(const K& key) {
		auto first = records_.begin();
		auto last = records_.end();
		first = std::lower_bound(first, last, key, &lookup_predicate);
		if(first != last && key == first->key) {
			return first->value;
		} else {
			return sorted_insert<record_<K, T>>(records_, {key, T()})->value;
		}
	}

	T& operator[](const K& key) {
		return lookup_or_insert_(key);
	}

	T& operator[](K && key) {
		return lookup_or_insert_(key);
	}

	bool has(const K& key) {
		auto search = std::lower_bound(records_.begin(), records_.end(), key, &lookup_predicate);
		return search != records_.end();
	}

	void reserve(const size_t size) {
		records_.reserve(size);
	}

private:
	std::vector<record_<K, T>> records_;
	static bool lookup_predicate(const record_<K, T>& _record, const K& _key) {
		bool ret = _record.key < _key;
		return ret;
	}
};
