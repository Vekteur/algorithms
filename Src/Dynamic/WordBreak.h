#pragma once

#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <memory>

namespace {
int wordBreak(const std::string& text, const std::vector<std::string>& words) {
	int n = text.size();
	const char minChar = 'a', maxChar = 'z';
	struct Trie {
		int count = 0;
		std::array<std::unique_ptr<Trie>, int(maxChar - minChar) + 1> children;
		std::unique_ptr<Trie>& next(int c) {
			return children[c - minChar];
		}
	};

	Trie trie;
	auto insertTrieReverse = [&trie](const std::string & word) {
		Trie* node = &trie;
		for (int i = int(word.size()) - 1; i >= 0; --i) {
			char c = word[i];
			if (node->next(c) == nullptr)
				node->next(c) = std::make_unique<Trie>();
			node = node->next(c).get();
		}
		++node->count;
	};

	for (const std::string& word : words)
		insertTrieReverse(word);

	std::vector<int> d(n + 1, 0);
	d[0] = 1;
	for (int i = 1; i <= n; ++i) {
		Trie* node = &trie;
		for (int j = i - 1; j >= 0; --j) {
			char c = text[j];
			node = node->next(c).get();
			if (node == nullptr)
				break;
			d[i] += d[j] * node->count;
		}
	}
	return d[n];
}
}