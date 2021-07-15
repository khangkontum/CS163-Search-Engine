#ifndef myfunction
#define myfunction

#include<bits/stdc++.h>
using namespace std;
// structure
struct Trie {
	Trie* child[96];  // from "33" to "128"
	map<string, int> fileArr;
	int cnt;
	Trie() {
		for (int i = 0; i < 96; ++i)
			child[i] = nullptr;
		cnt = 0;
	}
	void insert(string word, string fileName);
	int wordInFile(string word, string fileName);
};
extern int totalWord;
extern Trie* dataRoot;
extern Trie* stopwordsRoot;
extern Trie* thesaurusRoot;
extern map<string, int> maxwd;


// input function
pair<int, string> getInput();
// load function
string wordIgnore(string t);
bool loadData();

// implement function
double tf(string word, string fileName);
double idf(string word);
double tfidf(string word, string fileName);
// function 1
Trie* getLeaf(string word);
void rankAndDisplay(map<string, double> score);
map<string, double> function_1(string doc);

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