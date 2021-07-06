#ifndef myfunction
#define myfunction

#include<bits/stdc++.h>
using namespace std;

struct Trie {
	Trie* child[95];  // from "33" to "128"
	std::set<string> fileName;
	int cnt = 0;
	Trie() {
		for (int i = 0; i < 128; ++i)
			child[i] =nullptr;
	}
	void insert(string word, string fileName);
};

bool loadData(Trie* root);
void function_1(Trie* root);
void function_2(Trie* root);
void function_3(Trie* root);
void function_4(Trie* root);
void function_5(Trie* root);
void function_6(Trie* root);
void function_7(Trie* root);
void function_8(Trie* root);
void function_9(Trie* root);
void function_10(Trie* root);
void function_11(Trie* root);
void function_12(Trie* root);

#endif myfunction