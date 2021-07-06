#ifndef _myfunction_
#define _myfunction_

#include<bits/stdc++.h>
using namespace std;

struct Trie {
	Trie* child[128];
	vector<string> list_file;
	int cnt;
	Trie() {
		for (int i = 0; i <= 127; i++)
			child[i] = NULL;
		cnt = 0;
	}
};

bool loadData(Trie* dataRoot, Trie* stopwordsRoot, Trie* thesaurusRoot);
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