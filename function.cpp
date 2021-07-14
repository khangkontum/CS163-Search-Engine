#include"Header.h"

typedef pair<int, string> is;

// Input function
is getInput() {
	string keyword;
	cin >> keyword;

	//0
	if (keyword.size() == 1 && keyword[0] == '0')
		return is(0, "");

	//1

	//2

	//3
	for (int i = 0; i < keyword.length(); i++) {
		if (keyword[i] == '-' && i == 0)
			return is(13, keyword);
		if (keyword[i] == '-' && keyword[i - 1] == ' ')
			return is(3, keyword);
	}

	//4
	string intitle = "intitle:";
	for (int i = 0; i < intitle.length(); i++) {
		if (intitle[i] == keyword[i]) {
			if(i == intitle.length() - 1)
				return is(4, keyword);
			else
				continue;
		}
		else 
			break;
	}

	//5

	//6

	//7

	//8

	//9

	//10

	//11

	//12

	return is(-1, keyword); //Normal keyword
}

// Trie function
void Trie::insert(string word, string fileName) {
	Trie* pCur = this;
	for (int i = 0; i < word.length(); ++i) {
		if (!pCur->child[word[i] - ' '])
			pCur->child[word[i] - ' '] = new Trie();
		pCur = pCur->child[word[i] - ' '];
	}
	pCur->fileArr[fileName]++;
	maxwd[fileName] = max(maxwd[fileName], pCur->fileArr[fileName]); 
	pCur->cnt++;
}

int Trie::wordInFile(string word, string fileName) {
	Trie* pCur = this;
	int x;
	for (int i = 0; i < word.length(); i++) {
		x = word[i] - ' ';
		if (!pCur->child[x])
			return 0;
		pCur = pCur->child[x];
	}
	return pCur->fileArr[fileName];
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

bool loadData() {
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
			if (stopwordsRoot->wordInFile(wordIgnore(word), "")) //Ignore stopwords
				continue;
			dataRoot->insert(wordIgnore(word), fileName);
			totalWord++;
		}
		fin1.close();
	}
	fin.close();

	// load thesaurus


	return true;
}

//TF-IDF
double tf(string word, string fileName) {
	int ftd = dataRoot->wordInFile(word, fileName);
	return double(ftd) / double(maxwd[fileName]);
}

double idf(string word) {
	string fileName;
	int fileCount = 0;
	int numOfFile = 0;
	ifstream fin("Database/Search Engine-Data/___index.txt");
	while (fin >> fileName) {
		numOfFile++;
		if (dataRoot->wordInFile(word, fileName))
			fileCount++;
	}

	return log(double(numOfFile / (1 + fileCount)));
}

double tfidf(string word, string fileName) {
	return tf(word, fileName) * idf(word);
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