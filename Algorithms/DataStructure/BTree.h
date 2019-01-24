#pragma once

#include <iostream>
#include <string>
#include <algorithm>
#include <functional>
#include <memory>
#include <array>

template<typename T, int S>
class BTree {
private:
	struct Node {
		std::array<T, 2 * S - 1> data;
		std::array<std::unique_ptr<Node>, 2 * S> child;
		int size = 0;
		bool leaf = true;
		Node() {}

		void insertInLeaf(const T& d, int index) {
			for (int i = size; i > index; --i)
				data[i] = data[i - 1];
			data[index] = d;
			++size;
		}
		
		void insert(const T& d, int index_d, std::unique_ptr<Node> c, int index_c) {
			for (int i = size + 1; i > index_c; --i)
				child[i] = std::move(child[i - 1]);
			child[index_c] = std::move(c);
			insertInLeaf(d, index_d);
		}

		void removeInLeaf(int index) {
			for (int i = index; i < size - 1; ++i)
				data[i] = data[i + 1];
			--size;
		}

		void remove(int index_d, int index_c) {
			for (int i = index_c; i < size; ++i)
				child[i] = std::move(child[i + 1]);
			removeInLeaf(index_d);
		}
	};

	std::unique_ptr<Node> root = std::make_unique<Node>();
	std::function<bool(T, T)> comp;

	// Return the minimum index i such that d <= n->data[i]
	int minGreaterIndex(const Node* n, const T& d) {
		int i = 0;
		while (i < n->size && comp(n->data[i], d))
			++i;
		return i;
	}

	bool lookupRec(const Node* n, const T& d) {
		if (n == nullptr)
			return false;
		int i = minGreaterIndex(n, d);
		if (i < n->size && !comp(d, n->data[i])) {
			return true;
		} else {
			return lookupRec(n->child[i].get(), d);
		}
	}

	// Split full n->child[index] in two empty children n->child[index] and n->child[index + 1]
	void splitChild(Node* n, int index) {
		Node* left = n->child[index].get();
		std::unique_ptr<Node> right = std::make_unique<Node>();
		right->leaf = left->leaf;
		left->size = right->size = S - 1;
		for (int i = 0; i < S - 1; ++i)
			right->data[i] = left->data[S + i];
		if (!left->leaf) {
			for (int i = 0; i < S; ++i)
				right->child[i] = std::move(left->child[S + i]);
		}
		n->insert(left->data[S - 1], index, std::move(right), index + 1);
	}

	// Insert data d in non-full node n
	void insertRec(Node* n, const T& d) {
		int i = minGreaterIndex(n, d);
		if (i == n->size || (i < n->size && comp(d, n->data[i]))) { // d not in n
			if (n->leaf) {
				n->insertInLeaf(d, i);
			} else {
				if (n->child[i]->size == 2 * S - 1) {
					splitChild(n, i);
					if (comp(d, n->data[i])) {
						insertRec(n->child[i].get(), d);
					} else if (comp(n->data[i], d)) {
						insertRec(n->child[i + 1].get(), d);
					}
				} else {
					insertRec(n->child[i].get(), d);
				}
			}
		}
	}

	// Merge empty n->child[index] and empty n->child[index + 1] of non-empty node n
	void mergeChild(Node* n, int index) {
		Node* left = n->child[index].get();
		Node* right = n->child[index + 1].get();
		left->size = 2 * S - 1;
		left->data[S - 1] = n->data[index];
		for (int i = 0; i < S - 1; ++i)
			left->data[S + i] = right->data[i];
		if (!left->leaf) {
			for (int i = 0; i < S; ++i)
				left->child[S + i] = std::move(right->child[i]);
		}
		n->remove(index, index + 1);
	}

	void moveData(Node* n, int index, Node* to, int to_d, int to_c, 
			Node* from, int from_d, int from_c) {
		to->insert(n->data[index], to_d, std::move(from->child[from_c]), to_c);
		n->data[index] = from->data[from_d];
		from->remove(from_d, from_c);
	}

	// Take data from the non-empty right neighbour of n->child[index] with n empty
	void moveDataFromRight(Node* n, int index) {
		Node* to = n->child[index].get();
		moveData(n, index, to, to->size, to->size + 1, n->child[index + 1].get(), 0, 0);
	}

	// Take data from the non-empty left neighbour of n->child[index] with n empty
	void moveDataFromLeft(Node* n, int index) {
		Node* from = n->child[index - 1].get();
		moveData(n, index - 1, n->child[index].get(), 0, 0, from, from->size - 1, from->size);
	}

	// Make empty n->child[index] non - empty using left child
	// Return the new index of the child
	int preprocessLeft(Node* n, int index) {
		if (n->child[index - 1]->size == S - 1) {
			mergeChild(n, index - 1);
			return index - 1;
		} else {
			moveDataFromLeft(n, index);
			return index;
		}
	}

	// Make empty n->child[index] non-empty using right child
	// Return the new index of the child
	int preprocessRight(Node* n, int index) {
		if (n->child[index + 1]->size == S - 1)
			mergeChild(n, index);
		else
			moveDataFromRight(n, index);
		return index;
	}

	// Make empty n->child[index] non-empty
	// Return the new index of the child
	int preprocess(Node* n, int index) {
		if (index > 0)
			return preprocessLeft(n, index);
		else
			return preprocessRight(n, index);
	}

	T min(const Node* n) {
		if (n->leaf)
			return n->data[0];
		return min(n->child[0].get());
	}

	// n is non-empty
	void removeMinRec(Node* n) {
		if (n->leaf) {
			n->removeInLeaf(0);
		} else {
			if (n->child[0]->size == S - 1)
				preprocessRight(n, 0);
			removeMinRec(n->child[0].get());
		}
	}

	// Remove data[index] in non-empty node n
	void removeInRoot(Node* n, int index) {
		if (n->leaf) {
			n->removeInLeaf(index);
		} else {
			// Replace n->data[index] by the minimum of n->child[index + 1]
			// We know that the right child has a left neighbour
			if (n->child[index + 1]->size == S - 1) {
				if (n->child[index]->size == S - 1) {
					mergeChild(n, index);
					removeInRoot(n->child[index].get(), S - 1);
				} else {
					moveDataFromLeft(n, index + 1);
					removeInRoot(n->child[index + 1].get(), 0);
				}
			} else {
				n->data[index] = min(n->child[index + 1].get());
				removeMinRec(n->child[index + 1].get());
			}
		}
	}

	// Remove data d in node n
	// n must be non-empty except if it is the root with at least one non-empty child
	void removeRec(Node* n, const T& d) {
		int i = minGreaterIndex(n, d);
		if (i == n->size || (i < n->size && comp(d, n->data[i]))) { // d not in n
			if (!n->leaf) {
				int nextIndex = i;
				if (n->child[i]->size == S - 1)
					nextIndex = preprocess(n, i);
				removeRec(n->child[nextIndex].get(), d);
			}
		} else { // d in n
			removeInRoot(n, i);
		}
	}

	int sizeRec(const Node* n) {
		int res = n->size;
		if (n->leaf)
			return res;
		for (int i = 0; i <= n->size; ++i)
			res += sizeRec(n->child[i].get());
		return res;
	}

	void printRec(const Node* n, const std::string& space) {
		if (n == nullptr)
			return;
		printRec(n->child[n->size].get(), space + ' ');
		for (int i = n->size - 1; i >= 0; --i) {
			std::cout << space << n->data[i] << '\n';
			printRec(n->child[i].get(), space + ' ');
		}
	}

public:
	BTree(std::function<bool(T, T)> comp = std::less<T>()) : comp(comp) {}

	bool lookup(const T& d) {
		return lookupRec(root.get(), d);
	}

	void insert(const T& d) {
		if (root->size == 2 * S - 1) {
			std::unique_ptr<Node> newRoot = std::make_unique<Node>();
			newRoot->leaf = false;
			newRoot->child[0] = std::move(root);
			splitChild(newRoot.get(), 0);
			root = std::move(newRoot);
		}
		insertRec(root.get(), d);
	}

	void remove(const T& d) {
		if (root->size == 1 && !root->leaf &&
			root->child[0]->size == S - 1 && root->child[1]->size == S - 1) {
			mergeChild(root.get(), 0);
			root = std::move(root->child[0]);
		}
		removeRec(root.get(), d);
	}

	int size() {
		return sizeRec(root.get());
	}

	void print() {
		printRec(root.get(), "");
	}
};