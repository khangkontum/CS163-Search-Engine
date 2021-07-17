#ifndef myfunction
#define myfunction

//#include<bits/stdc++.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <string.h>
#include <string>
#include <fstream>
#include <cstdio>
#include <cmath> 
#include <iterator>
#include <map>
#include <algorithm>
#include <type_traits>
#include "nlohmann/json.hpp"


using json = nlohmann::json;
using namespace std;
//

typedef pair<int, string> is;
typedef pair<string, int> si;
typedef pair<string, string> ss;
typedef pair<string, vector<int> > svec;
typedef pair<int, int> ii;
typedef pair<int, ii> iii;
typedef pair<double, string> ds;
typedef pair<string, double> sd;
#define pb push_back
#define fi first
#define se second

// structure
struct Trie {
	Trie* child[39];  // a->z : 0->25 , 0->9 : 26->35 , # $ *: 36->38
	map<string, vector<int> > fileArr;
	Trie() {
		for (int i = 0; i < 39; ++i)
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
vector<string> split(string s);
string subtract(string s, int start, int end);
vector<string> loadWordArr(string doc, int x);
Trie* getLeaf(string word, Trie* &root);
Trie* getLeaf(string word);
void highlightLine(string get, vector<string> wordArr);
void rankAndDisplay(map<string, double> score, vector<string> wordArr);
int isSequenceInFile(vector<string> wordList, string fileName);

// function 1
map<string, double> function_1(string doc);
//function 2
map<string, double> function_2(string doc);
// function 3
map<string, double> function_3(string doc);
// function 4
map<string, double> function_4(string doc);

void normalSearchTmp(string keyword);
vector<string> normalSearch(string keyword);
void function_5(string keyword);
void function_6(Trie* root);

// function 7
map<string, double> function_7(string doc);
// function 8
map<string, double> function_8(string doc);

void exactMatch(string keyword);
void function_10(Trie* root);
void function_11(Trie* root);
void function_12(string keyword);

#endif 