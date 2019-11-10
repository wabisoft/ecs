#pragma once

#include <stack>
#include <vector>

#include "algorithm.hpp"

// Data Structures
template <typename T>
struct Stack : public std::stack<T, std::vector<T>> {
	T rpop() {
		T t = this->top();
		this->pop();
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


template <typename K, typename T, size_t N>
struct Set {
	Set(K(*l)(const T&)) : lookupProp(l) { }

	T& lookup(const K& key) {
		T* search = binary_find(key, entries, size_, lookupProp);
		if(search) { return *search; }
		else { throw "Key error: Not found in set"; }
	}

	T& operator[](const K& key) {
		return lookup(key);
	}
	T& operator[](K&& key) {
		return lookup(key);
	}
	T& do_add(const T& t) {
		if(size_ == N-1) { throw "Set out of capacity"; }
		return *sorted_insert(t, entries, size_, [this](const T& t1, const T& t2){ return lookupProp(t1) < lookupProp(t2); });
	}
	T& add(const T& t) { return do_add(t); }
	T& add(T&& t) { return do_add(t); }
	size_t size() const { return size_; }

// private:
	T entries[N];
	size_t capacity = N;
	size_t size_ = 0;
	K(*lookupProp)(const T&) = nullptr;
};


template <typename T, size_t N>
struct slot_set {
	// I don't know what you call this data structure
	// but I designed to be very fast for random access and almost as fast to traverse
	// it pays for that by being pretty slow to insert and remove from
	// as well as having a static size, so it's not super memory friendly
	// This structure is suitable for my purpose of accessing elements of vary sets sharing a commond id
	// I use the common id as the index into the set
	// the slotted set maintains a fragmented array of T, each active element of the array
	// is also a sorted linked list of all other active elements allowing quick traversal of the list
	// this structure is probably not suitable for sets larger than 1000
	// Althought this struct defines and iterator type and has a begin and an end, I wouldn't use
	// the interface for much more than looping, viz. don't go using the stl iterator algorithms
	// without extensively testing them first.
	// NOTE: this struct could very easily become a dynamically sized but then pointers to elements
	// would be subject to invalidation when the underlying array gets reallocated and copied during a grow
	// for this reason the struct is static.

	struct wrapper_ {
		T t;
		wrapper_* next = nullptr;
		bool active = false;
	};

	struct iterator {
		typedef iterator self_type;
		typedef T value_type;
		typedef T& reference;
		typedef wrapper_* pointer;
		typedef std::forward_iterator_tag iterator_category;
		typedef size_t difference_type;
		iterator(pointer ptr) : ptr_(ptr) { }
		self_type operator++() { ptr_ = ptr_->next; return *this; } // PREFIX
		self_type operator++(int) { self_type i = *this; ptr_ = ptr_->next; return i; } // POSTFIX
		T& operator*() { return ptr_->t; }
		T* operator->() { return &ptr_->t; }
		bool operator==(const self_type& rhs) { return ptr_ == rhs.ptr_; }
		bool operator!=(const self_type& rhs) { return ptr_ != rhs.ptr_; }

	private:
		pointer ptr_;
	};

	slot_set(size_t(*idx)(const T& t)) : idx_(idx) { }

	T& operator[](size_t index) {
		assert(index < capacity_);
		wrapper_& w = data[index];
		assert(w.active); // item must be in set;
		return w.t;
	}

	T& do_add(const T& t) {
		size_t idx = idx_(t);
		assert(idx < capacity_);
		wrapper_& w = data[idx];
		w = {t, nullptr, true};
		if(front_ == nullptr) { // add to empty list
			front_ = &w;
			back_ = front_;
		} else if(idx < idx_(front_->t)) { // add to front of list (only begin changes)
			w.next = front_;
			front_ = &w;
		} else {
			wrapper_* cur = front_;
			while(cur) {
				if(idx_(cur->t) <= idx && (cur->next == nullptr || idx < idx_(cur->next->t))) {
					auto tmp = cur->next;
					cur->next = &w;
					w.next = tmp;
					if(tmp == nullptr) { back_ = &w; }
					break;
				}
				cur = cur->next;
			}
		}
		++size_;
		return w.t;
	}

	T& add(const T& t) { return do_add(t); }
	T& add(T&& t) { return do_add(t); }

	void rem(size_t index) {
		if(size_ == 0 ){ return; } // list empty
		wrapper_& w = data[index];
		if(!w.active) { return; } // can't remove element not that is active
		if(front_ == &w) { front_ = w.next;  return; }
		assert(front_->next != nullptr);
		wrapper_* prev = front_;
		wrapper_* cur = prev->next;
		while(cur) {
			if(cur == &w) {
				prev->next = cur->next;
				if(cur->next == nullptr) {
					back_ = &prev;
				}
				break;
			}
			prev = cur;
			cur = cur->next;
		}
		--size_;
		w.t.~T();
		w.next = nullptr;
		w.active = false;
	}

	size_t capacity() const { return capacity_; }
	size_t size() const { return size_; }
	iterator begin() { return iterator(front_); }
	iterator end() { return iterator(nullptr); }
	T& front() { return front_->t; }
	T& back() { return back_->t; }
	wrapper_* i_know_what_im_doing_give_me_your_data_() {
		return data;
	}

private:
 	wrapper_ data[N];
	const size_t capacity_ = N;
	size_t size_ = 0;
	wrapper_ * front_ = nullptr;
	wrapper_ * back_ = nullptr;
	size_t(*idx_)(const T& t) = nullptr;
};
