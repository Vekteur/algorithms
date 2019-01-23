#pragma once

#include <vector>
#include <algorithm>
#include <functional>

template<typename T>
class Heap {
private:
	std::vector<T> a;
	std::function<bool(T, T)> comp;

	int left(int i) { return 2 * i + 1; }
	int right(int i) { return 2 * i + 2; }
	int parent(int i) { return i / 2; }

	void heapify(int i) {
		while (true) {
			int l = left(i), r = right(i), n = int(a.size());
			int largest = i;
			if (l < n && comp(a[largest], a[l]))
				largest = l;
			if (r < n && comp(a[largest], a[r]))
				largest = r;

			if (largest != i) {
				std::swap(a[i], a[largest]);
				i = largest;
			} else {
				break;
			}
		};
	}

public:
	Heap(const std::vector<int>& a = {}, std::function<bool(T, T)> comp = std::less<T>())
		: a(a), comp(comp) {

		for (int i = parent(int(a.size()) - 1); i >= 0; --i)
			heapify(i);
	}

	T top() {
		return a[0];
	}

	void push(T data) {
		a.push_back(data);
		int i = int(a.size()) - 1;
		while (i > 0 && comp(a[parent(i)], data)) {
			std::swap(a[i], a[parent(i)]);
			i = parent(i);
		}
	}

	void pop() {
		a[0] = a.back();
		a.pop_back();
		heapify(0);
	}
};