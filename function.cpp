#include"Header.h"

// Input function
pair<int, string> getInput() {
	string keyword;
	cin >> keyword;
	//1

	//2

	//3
	for (int i = 0; i < keyword.length(); i++) {
		if (keyword[i] == '-' && i == 0)
			return pair<int, string>(13, keyword);
		if (keyword[i] == '-' && keyword[i - 1] == ' ')
			return pair<int, string>(3, keyword);
	}

	//4

	//5

	//6

	//7

	//8

	//9

	//10

	//11

	//12

	return pair<int, string>(-1, keyword); //Normal keyword
}

// Trie function
void Trie::insert(string word, string fileName) {
	Trie* pCur = this;
	for (int i = 0; i < word.length(); ++i) {
		if (!pCur->child[word[i] - ' '])
			pCur->child[word[i] - ' '] = new Trie();
		pCur = pCur->child[word[i] - ' '];
	}
	pCur->fileName.push_front(fileName);
	pCur->cnt++;
}
bool Trie::isExist(string word) {
	Trie* pCur = this;
	int x;
	for (int i = 0; i < word.length(); i++) {
		x = word[i] - ' ';
		if (!pCur->child[x])
			return false;
		pCur = pCur->child[x];
	}
	return true;
}

// Load data function
string wordIgnore(string t) {
	string result = "";
	for (int i = 0; i < t.length(); i++) {
		if (int(t[i] - 33) * (int(t[i]) - 127) <= 0)
			result += t[i];
	}
	return result;
}
bool loadData(Trie* dataRoot, Trie* stopwordsRoot, Trie* thesaurusRoot) {
	// load stopwords
	ifstream input("Database/Stopwords/stopwords.txt");
	string stopword;
	while(input >> stopword) {
		stopwordsRoot->insert(stopword, "");
	}
	input.close();

	// load data
	ifstream fin("Database/Search Engine-Data/___index.txt");
	if (!fin.is_open())
		return false;
	while (!fin.eof()) {
		string fileName; 
		getline(fin, fileName, '\n');
		ifstream fin1("Database/Search Engine-Data/" + fileName);
		if (fin1.is_open() == false) {
			cout << "Failed load file " << fileName  << "\n" << "\n";
			return false;
		}
		while (!fin1.eof()) {
			string word; 
			fin1 >> word;
			if (stopwordsRoot->isExist(wordIgnore(word))) //Ignore stopwords
				continue;
			dataRoot->insert(wordIgnore(word), fileName);
		}
		fin1.close();
	}
	fin.close();

	// load thesaurus


	return true;
}
bool isStopWords(Trie* stopWordsTrie, string s) {
	return true;
}


void function_1(Trie* root) {

}
void function_2(Trie* root) {

}
void function_3(Trie* root) {

}
void function_4(Trie* root) {

}
void function_5(Trie* root) {

}
void function_6(Trie* root) {

}
void function_7(Trie* root) {

}
void function_8(Trie* root) {

}
void function_9(Trie* root) {

}
void function_10(Trie* root) {

}
void function_11(Trie* root) {

}
void function_12(Trie* root) {

}