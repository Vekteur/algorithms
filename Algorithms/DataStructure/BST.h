#pragma once

#include <iostream>
#include <string>
#include <algorithm>
#include <functional>
#include <memory>

template<typename T>
class BST {
private:
	struct Node {
		T data;
		std::unique_ptr<Node> l, r;
		Node(T data, std::unique_ptr<Node> l = nullptr, std::unique_ptr<Node> r = nullptr)
			: data(data), l(std::move(l)), r(std::move(r)) {}
	};

	std::unique_ptr<Node> root;
	std::function<bool(T, T)> comp;

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
			return std::make_unique<Node>(data);
		if (comp(data, n->data))
			n->l = insertRec(data, std::move(n->l));
		else if (comp(n->data, data))
			n->r = insertRec(data, std::move(n->r));
		return n;
	}

	T min(Node* n) {
		if (n->l == nullptr)
			return n->data;
		return min(n->l.get());
	}

	T max(Node* n) {
		if (n->r == nullptr)
			return n->data;
		return max(n->l.get());
	}

	std::unique_ptr<Node> removeMinRec(std::unique_ptr<Node> n) {
		if (n->l == nullptr)
			return std::move(n->r);
		n->l = removeMinRec(std::move(n->l));
		return n;
	}

	std::unique_ptr<Node> removeRoot(std::unique_ptr<Node> n) {
		if (n->l == nullptr)
			return std::move(n->r);
		if (n->r == nullptr)
			return std::move(n->l);
		n->data = min(n->r.get());
		n->r = removeMinRec(std::move(n->r));
		return n;
	}

	std::unique_ptr<Node> removeRec(T data, std::unique_ptr<Node> n) {
		if (n == nullptr)
			return n;
		if (comp(data, n->data))
			n->l = removeRec(data, std::move(n->l));
		else if (comp(n->data, data))
			n->r = removeRec(data, std::move(n->r));
		else
			return removeRoot(std::move(n));
		return n;
	}

	void printRec(Node* n, std::string space) {
		if (n == nullptr)
			return;
		printRec(n->r.get(), space + ' ');
		std::cout << space << n->data << '\n';
		printRec(n->l.get(), space + ' ');
	}
public:
	BST(std::function<bool(T, T)> comp = std::less<T>()) : comp(comp) {}

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