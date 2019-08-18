#pragma once

#include <vector>
#include <map>
#include <string>
#include <queue>
#include <algorithm>
#include <functional>

#include "Constants.h"

// See https://cp-algorithms.com/string/suffix-automaton.html
struct SuffixAutomaton {
	struct State {
		int len, link;
		bool isClone = false;
		int firstPos;
		std::map<char, int> next;
		std::vector<int> invLink;
	};

	std::vector<State> st;
	std::vector<int> cnt;
	std::vector<int> nbPaths;
	std::vector<int> toposort;
	int last = 0;

	SuffixAutomaton(const std::string& s = "") {
		st.push_back({ 0, -1 });
		for (char c : s)
			extend(c);

		computeToposort();
		computeCnt();
		computeNbPaths();
		for (int u = 1; u < int(st.size()); ++u) {
			st[st[u].link].invLink.push_back(u);
		}
	}

	void extend(char c) {
		int cur = st.size();
		st.push_back({ st[last].len + 1 });
		st[cur].firstPos = st[last].len;
		int p = last;
		while (p != -1 && !st[p].next.count(c)) {
			st[p].next[c] = cur;
			p = st[p].link;
		}
		if (p == -1) {
			st[cur].link = 0;
		}
		else {
			int q = st[p].next[c];
			if (st[p].len + 1 == st[q].len) {
				st[cur].link = q;
			}
			else {
				int clone = st.size();
				st.push_back({ st[p].len + 1, st[q].link, true });
				st[clone].firstPos = st[q].firstPos;
				st[clone].next = st[q].next;
				while (p != -1 && st[p].next[c] == q) {
					st[p].next[c] = clone;
					p = st[p].link;
				}
				st[q].link = st[cur].link = clone;
			}
		}
		last = cur;
	}

	void computeToposort() {
		std::vector<bool> vis(st.size(), false);
		std::function<void(int)> dfs = [&dfs, this, &vis](int u) {
			if (vis[u])
				return;
			vis[u] = true;
			for (auto p : st[u].next)
				dfs(p.second);
			toposort.push_back(u);
		};
		dfs(0);
	}

	void computeCnt() {
		cnt.assign(st.size(), 0);
		for (int v = 0; v < int(st.size()); ++v) {
			if (!st[v].isClone)
				cnt[v] = 1;
		}
		for (int u : toposort) {
			if (u != 0)
				cnt[st[u].link] += cnt[u];
		}
	}

	void computeNbPaths() {
		nbPaths.assign(st.size(), 1);
		for (int u : toposort) {
			for (auto p : st[u].next) {
				nbPaths[u] += nbPaths[p.second];
			}
		}
	}

	int distinctSubstrings() {
		return nbPaths[0] - 1;
	}

	int distinctSubstrings2() {
		int cnt = 0;
		for (int v = 1; v < int(st.size()); ++v)
			cnt += st[v].len - st[st[v].link].len;
		return cnt;
	}

	int follow(const std::string& pattern) {
		int v = 0;
		for (char c : pattern) {
			if (!st[v].next.count(c))
				return -1;
			v = st[v].next[c];
		}
		return v;
	}

	int cntMatch(const std::string& pattern) {
		int v = follow(pattern);
		return (v == -1) ? 0 : cnt[v];
	}

	std::vector<int> match(const std::string& pattern) {
		std::vector<int> res;
		std::function<void(int)> dfs = [&dfs, this, &res, &pattern](int u) {
			if (!st[u].isClone)
				res.push_back(st[u].firstPos - int(pattern.size()) + 1);
			for (int v : st[u].invLink)
				dfs(v);
		};
		int v = follow(pattern);
		dfs(v);
		return res;
	}

	std::string kthDistinctSubstring(int k) {
		std::string res;
		int u = 0;
		for (; k != 0; --k) {
			for (auto p : st[u].next) {
				if (k <= nbPaths[p.second]) {
					u = p.second;
					res.push_back(p.first);
					break;
				}
				k -= nbPaths[p.second];
			}
		}
		return res;
	}

	std::string longestCommonSubstring(const std::string& s) {
		int v = 0, l = 0, bestLen = 0, bestPos = 0;
		for (int i = 0; i < int(s.size()); ++i) {
			while (v != 0 && !st[v].next.count(s[i])) {
				v = st[v].link;
				l = st[v].len;
			}
			if (st[v].next.count(s[i])) {
				v = st[v].next[s[i]];
				++l;
				if (l > bestLen) {
					bestLen = l;
					bestPos = i;
				}
			}
		}
		return s.substr(bestPos - bestLen + 1, bestLen);
	}

	// The automaton has to be constructed with the smallest string to ensure linear complexity
	std::string longestCommonSubstring(const std::vector<std::string>& strs) {
		std::vector<bool> allVis(st.size(), true);
		std::vector<int> allMinLen(st.size(), INF);
		std::vector<int> indices(st.size(), -1); // Only used with last string

		for (const std::string& s : strs) {
			std::vector<int> maxLen(st.size(), -1);
			// maxLen[v] will contain the length of the longest
			// string in the state v that is also a substring of s
			int v = 0, l = 0;
			for (int i = 0; i < int(s.size()); ++i) {
				while (v != 0 && !st[v].next.count(s[i])) {
					v = st[v].link;
					l = st[v].len;
				}
				if (st[v].next.count(s[i])) {
					v = st[v].next[s[i]];
					++l;
					// l = length of longest string s]i-l:i] that appears in the automaton
					if (l > maxLen[v]) {
						maxLen[v] = l;
						indices[v] = i;
					}
				}
			}

			// Visit the states that are suffixes of already visited longest strings with DP
			for (int u : toposort) {
				if (u != 0 && maxLen[u] != -1) {
					int sfx = st[u].link;
					maxLen[sfx] = st[sfx].len;
					indices[sfx] = indices[u];
				}
			}

			for (int u = 0; u < int(st.size()); ++u) {
				allVis[u] = allVis[u] && (maxLen[u] != -1);
				if (allVis[u]) {
					allMinLen[u] = std::min(allMinLen[u], maxLen[u]);
				}
			}
		}

		int maxL = -1;
		int bestIndex = -1;
		for (int u = 0; u < int(st.size()); ++u) {
			if (allVis[u] && allMinLen[u] > maxL) {
				maxL = allMinLen[u];
				bestIndex = indices[u];
			}
		}
		return strs.back().substr(bestIndex - maxL + 1, maxL);
	}
};