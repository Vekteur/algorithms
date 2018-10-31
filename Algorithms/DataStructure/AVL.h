#pragma once

#include <iostream>
#include <string>
#include <algorithm>
#include <functional>
#include <memory>

template<typename T>
class AVL {
protected:
	struct Node {
		T data;
		int height;
		std::unique_ptr<Node> l, r;
		Node(T data, int height, std::unique_ptr<Node> l = nullptr, std::unique_ptr<Node> r = nullptr)
			: data(data), height(height), l(std::move(l)), r(std::move(r)) {}
	};

	std::unique_ptr<Node> root;
	std::function<bool(T, T)> comp;

	int height(const Node* n) {
		int left = (n->l == nullptr) ? 0 : n->l->height;
		int right = (n->r == nullptr) ? 0 : n->r->height;
		return 1 + std::max(left, right);
	}

	void adjustHeight(Node* n) {
		n->height = height(n);
	}

	int findBalance(const Node* n) {
		int left = (n->l == nullptr) ? 0 : n->l->height;
		int right = (n->r == nullptr) ? 0 : n->r->height;
		return right - left;
	}

	std::unique_ptr<Node> rotateLeft(std::unique_ptr<Node> n) {
		std::unique_ptr<Node> nr = std::move(n->r);
		std::unique_ptr<Node> midSubtree = std::move(nr->l);
		nr->l = std::move(n);
		nr->l->r = std::move(midSubtree);
		adjustHeight(nr->l.get());
		adjustHeight(nr.get());
		return nr;
	}

	std::unique_ptr<Node> rotateRight(std::unique_ptr<Node> n) {
		std::unique_ptr<Node> nl = std::move(n->l);
		std::unique_ptr<Node> midSubtree = std::move(nl->r);
		nl->r = std::move(n);
		nl->r->l = std::move(midSubtree);
		adjustHeight(nl->r.get());
		adjustHeight(nl.get());
		return nl;
	}

	std::unique_ptr<Node> balance(std::unique_ptr<Node> n) {
		int bal = findBalance(n.get());
		if (bal == 2) {
			if (findBalance(n->r.get()) >= 0) {
				return rotateLeft(std::move(n));
			} else {
				n->r = rotateRight(std::move(n->r));
				return rotateLeft(std::move(n));
			}
		} else if (bal == -2) {
			if (findBalance(n->l.get()) <= 0) {
				return rotateRight(std::move(n));
			} else {
				n->l = rotateLeft(std::move(n->l));
				return rotateRight(std::move(n));
			}
		}
		adjustHeight(n.get());
		return n;
	}

	bool lookup(T data, const Node* n) {
		if (n == nullptr)
			return false;
		if (comp(data, n->data))
			return lookup(data, n->l.get());
		else if (comp(n->data, data))
			return lookup(data, n->r.get());
		return true;
	}

	std::unique_ptr<Node> insertRec(T data, std::unique_ptr<Node> n) {
		if (n == nullptr)
			return std::make_unique<Node>(data, 1);
		if (comp(data, n->data)) {
			n->l = insertRec(data, std::move(n->l));
			n = balance(std::move(n));
		} else if (comp(n->data, data)) {
			n->r = insertRec(data, std::move(n->r));
			n = balance(std::move(n));
		}
		return n;
	}

	T min(const Node* n) {
		if (n->l == nullptr)
			return n->data;
		return min(n->l.get());
	}

	T max(const Node* n) {
		if (n->r == nullptr)
			return n->data;
		return max(n->l.get());
	}

	std::unique_ptr<Node> removeMinRec(std::unique_ptr<Node> n) {
		if (n->l == nullptr)
			return std::move(n->r);
		n->l = removeMinRec(std::move(n->l));
		n = balance(std::move(n));
		return n;
	}

	std::unique_ptr<Node> removeRoot(std::unique_ptr<Node> n) {
		if (n->l == nullptr)
			return std::move(n->r);
		if (n->r == nullptr)
			return std::move(n->l);
		n->data = min(n->r.get());
		n->r = removeMinRec(std::move(n->r));
		n = balance(std::move(n));
		return n;
	}

	std::unique_ptr<Node> removeRec(T data, std::unique_ptr<Node> n) {
		if (n == nullptr)
			return n;
		if (comp(data, n->data)) {
			n->l = removeRec(data, std::move(n->l));
			n = balance(std::move(n));
		} else if (comp(n->data, data)) {
			n->r = removeRec(data, std::move(n->r));
			n = balance(std::move(n));
		} else {
			return removeRoot(std::move(n));
		}
		return n;
	}

	void printRec(const Node* n, const std::string& space) {
		if (n == nullptr)
			return;
		printRec(n->r.get(), space + ' ');
		std::cout << space << n->data << '\n';
		printRec(n->l.get(), space + ' ');
	}

public:
	AVL(std::function<bool(T, T)> comp = std::less<T>()) : comp(comp) {}

	bool lookup(T data) {
		return lookup(data, root.get());
	}

	void insert(T data) {
		root = insertRec(data, std::move(root));
	}

	void remove(T data) {
		root = removeRec(data, std::move(root));
	}

	void print() {
		printRec(root.get(), "");
	}
};