#include"Header.h"
string wordIgnore(string t) {
	string result = "";
	for (int i = 0; i < t.length(); i++) {
		if (int(t[i] - 33) * (int(t[i]) - 127) <= 0)
			result += t[i];
	}
	return result;
}
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

bool loadData(Trie* dataRoot, Trie* stopwordsRoot, Trie* thesaurusRoot) {

	// load data
	ifstream fin("Search Engine-Data/___index.txt");
	if (!fin.is_open())
		return false;
	while (!fin.eof()) {
		string fileName; getline(fin, fileName, '\n');
		ifstream fin1("Search Engine-Data/" + fileName);
		if (!fin1.is_open())
			return false;
		cout << fileName << endl;
		while (!fin1.eof()) {
			string word; fin1 >> word;
			dataRoot->insert(wordIgnore(word), fileName);
		}
	}


	// load stopwords

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