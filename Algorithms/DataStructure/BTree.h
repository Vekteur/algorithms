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
		bool leaf = false;
		Node() {}

		void insertInLeaf(const T& d, int index) {
			for (int i = size; i > index; --i)
				data[i] = data[i - 1];
			data[index] = d;
			++size;
		}
		
		void insert(const T& d, std::unique_ptr<Node> c, int index) {
			for (int i = size + 1; i > index + 1; --i)
				child[i] = std::move(child[i - 1]);
			child[index + 1] = std::move(c);
			insertInLeaf(d, index);
		}

		void removeInLeaf(int index) {
			for (int i = index; i < size - 1; ++i)
				data[i] = data[i + 1];
			--size;
		}

		void remove(int index) {
			for (int i = index + 1; i < size; ++i)
				child[i] = std::move(child[i + 1]);
			removeInLeaf(index);
		}
	};

	std::unique_ptr<Node> root;
	std::function<bool(T, T)> comp;

	bool lookup(const Node* n, const T& d) {
		if (n == nullptr)
			return false;
		int i = 0;
		while (i < n->size && comp(n->data[i], d))
			++i;
		if (i < n->size && !comp(d, n->data[i])) {
			return true;
		} else {
			return lookup(n->child[i].get(), d);
		}
	}

	void splitChild(Node* n, int index) {
		Node* left = n->child[index].get();
		std::unique_ptr<Node> right = std::make_unique<Node>();
		right->leaf = left->leaf;
		left->size = right->size = S - 1;
		// Transfer right part of left to right
		for (int i = 0; i < S - 1; ++i)
			right->data[i] = left->data[S + i];
		if (!left->leaf) {
			for (int i = 0; i < S; ++i)
				right->child[i] = std::move(left->child[S + i]);
		}
		n->insert(left->data[S - 1], std::move(right), index);
	}

	void insertRec(Node* n, const T& d) {
		int i = 0;
		while (i < n->size && comp(n->data[i], d))
			++i;
		if (i == n->size || (i < n->size && comp(d, n->data[i]))) {
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

	void mergeChild(Node* n, int index) {
		Node* left = n->child[index].get();
		Node* right = n->child[index + 1].get();
		left->size = 2 * S - 1;
		left->data[S - 1] = n->data[index];
		for (int i = 0; i < S - 1; ++i)
			left->data[S + i] = right->data[i];
		for (int i = 0; i < S; ++i)
			left->child[S + i] = std::move(right->child[i]);
		n->remove(index);
	}

	void moveDataFromRight(Node* n, int index) {

	}

	void removeMinRec(Node* n, const T& d) {

	}

	void removeRoot(Node* n, const T& d) {

	}

	void removeRec(Node* n, const T& d) {

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
		return lookup(root.get(), d);
	}

	void insert(const T& d) {
		if (root == nullptr) {
			root = std::make_unique<Node>();
			root->leaf = true;
		}
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

	}

	void print() {
		printRec(root.get(), "");
	}
};