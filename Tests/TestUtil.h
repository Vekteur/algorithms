#pragma once

#include <iostream>
#include <vector>

template<typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& v) {
	for (const T& el : v)
		out << el << ' ';
	return out;
}