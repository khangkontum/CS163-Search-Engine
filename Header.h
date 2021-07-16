#ifndef myfunction
#define myfunction

#include<bits/stdc++.h>
using namespace std;
// structure
struct Trie {
	Trie* child[38];  // a->z : 0->25 , 0->9 : 26->35 , # $ : 36->37
	map<string, vector<int> > fileArr;
	Trie() {
		for (int i = 0; i < 38; ++i)
			child[i] = nullptr;
	}
	void insert(string word, string fileName, int pos);
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
int letterToInt(char a);
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
//function 2
map<string, double> function_2(string doc);
// function 3
map<string, double> function_3(string doc);
// function 4
map<string, double> function_4(string doc);

void function_5(Trie* root);
void function_6(Trie* root);

// function 7
map<string, double> function_7(string doc);
// function 8
map<string, double> function_8(string doc);

void function_9(Trie* root);
void function_10(Trie* root);
void function_11(Trie* root);
void function_12(Trie* root);

#endif 