#include"Header.h"
void Trie::insert(string word,string fileName) {
	Trie* pCur = this;
	for (int i = 0; i < word.length(); ++i) {
		if(!pCur->child[word[i] - ' '])
			pCur->child[word[i] - ' '] = new Trie();
		pCur = pCur->child[word[i] - ' '];
	}
	pCur->fileName.insert(fileName);
	pCur->cnt++;
}

bool loadData(Trie* root) {
	ifstream fin("Search Engine-Data\\___index.txt");
	if (!fin.is_open())
		return false;
	while (!fin.eof()) {
		string fileName; getline(fin,fileName,'\n');
		ifstream fin1("Search Engine-Data\\" + fileName);
		if (!fin1.is_open())
			return false;
		while (!fin1.eof()) {
			string word; fin1 >> word;
			root->insert(word,fileName);
		}
	}
	return true;
}
void function_1(Trie* root){

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