#ifndef myfunction
#define myfunction

#include<bits/stdc++.h>
using namespace std;

struct Trie {
	Trie* child[96];  // from "33" to "128"
	list<string> fileName;
	int cnt;
	Trie() {
		for (int i = 0; i < 96; ++i)
			child[i] = nullptr;
		cnt = 0;
	}
	void insert(string word, string fileName);
	bool isExist(string word);
	bool isExistInFile(string word, string fileName);
};

bool loadData(Trie* dataRoot, Trie* stopwordsRoot, Trie* thesaurusRoot);
bool isStopWords(Trie* stopWordsTrie, string s);

pair<int, string> getInput();
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

#endif 