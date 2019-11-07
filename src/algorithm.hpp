#pragma once

#include <algorithm>

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
		std::upper_bound<std::vector<T>::iterator, T>(v.begin(), v.end(), item, predicate),
#endif
		item
	);
}

template <typename T>
typename std::vector<T>::iterator sorted_insert(std::vector<T>& v, T item) {
	return sorted_insert<T>(v, item, [] (const T& a, const T& b) { return a < b; });
}


