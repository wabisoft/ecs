#pragma once

#include <algorithm>
#include <cassert>
#include <cmath>

// insertion sort && sorted insert on vectors
template <typename T, typename Iter, typename Pred>
void insertion_sort(Iter begin, Iter end, Pred predicate) {
	for (auto it = begin; it != end; ++it) {
		std::rotate(std::upper_bound(begin, it, *it, predicate), it, std::next(it));
	}
}

template <typename T, typename Pred>
typename std::vector<T>::iterator sorted_insert(std::vector<T>& v, T item, Pred predicate) {
#ifdef _DEBUG
	auto it = std::upper_bound<std::vector<T>::iterator, T>(v.begin(), v.end(), item, predicate);
#endif
	return v.insert(
#ifdef _DEBUG
		it,
#else
		std::upper_bound<typename std::vector<T>::iterator, T>(v.begin(), v.end(), item, predicate),
#endif
		item
	);
}

template <typename T>
typename std::vector<T>::iterator sorted_insert(std::vector<T>& v, T item) {
	return sorted_insert<T>(v, item, [] (const T& a, const T& b) { return a < b; });
}



// insertion sort and sorted insert on arrays
template<typename T, typename Pred>
inline void insertion_sort(T* t, const int count, Pred predicate) {
	for(int i = 1; i<count; ++i) {
		for(int j = i; predicate(t[j], t[j-1]) && j>0; --j) {
			T swap = t[j];
			t[j] = t[j-1];
			t[j-1] = swap;
		}
	}
}

template<typename T>
inline void insertion_sort(T* t, const int count) {
	return insertion_sort(t, count, [](T a, T b) { return a<b; });
}

template <typename T, typename Pred>
inline T* sorted_insert(T t_item, T* t, size_t& count, Pred predicate) {
	if (count == 0) {
		t[0] = t_item;
		++count;
		return &t[0];
	}
	for(size_t i = 0; i<count; ++i){
		if(predicate(t[i],t_item)) {
			if (i == count - 1) { // at the end of array
				t[i+1] = t_item;
				++count;
				return &t[i-1];
			}
			continue;
		} else {
			T swap = t[i];
			t[i] = t_item;
			for (size_t j = i+1; j<count+1; ++j){
				T swap_swap = swap;
				swap = t[j];
				t[j] = swap_swap;
			}
			++count;
			return &t[i];
		}
	}
	assert(false); // should not reach here
	return nullptr;
}

template <typename T>
inline T* sorted_insert(T t_item, T* t, size_t& count) {
	return sorted_insert(t_item, t, count, [](T a, T b) { return a < b; });
}



template <typename T, typename U, typename Prop>
inline
T* binary_find(
	U query, // U must correspond to the return type of prop
    T* const t,  // t must be an array sorted with respect to the result of Prop
	const int count, // size of the array
	Prop prop = [](const T& t_) { return t_; }) // what to compare in type T, return type must match U
{
	int left = 0;
	int right = count-1;
	while (left <= right) {
		int middle = (int)std::floor((left + right) / 2);
		U propRes = prop(t[middle]);
		if (propRes < query) {
			left = middle + 1;
		} else if (propRes > query) {
			right = middle - 1;
		} else {
			return &t[middle];
		}
	}
	return nullptr;
}

template <unsigned long N>
struct NaryHash {
	static unsigned hash(unsigned a, unsigned b) {
		assert(a < N && b < N);
		return (unsigned)(a + b * N);
	}

	static void unhash(unsigned hash, unsigned& a, unsigned& b) {
		a = (unsigned)hash % N;
		b = (unsigned)hash / N;
	}
};

