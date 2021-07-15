#include"Header.h"

typedef pair<int, string> is;

// Input function
is getInput() {
	string keyword;
	getline(cin, keyword, '\n');

	//0
	if (keyword.size() == 1 && keyword[0] == '0')
		return is(0, "");

	//1
	if (keyword.find(" AND ") != string::npos)
		return is(1, keyword);
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
			if (i == intitle.length() - 1)
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
void Trie::insert(string word, string fileName, int pos) {
	Trie* pCur = this;
	for (int i = 0; i < word.length(); ++i) {
		if (!pCur->child[word[i] - ' '])
			pCur->child[word[i] - ' '] = new Trie();
		pCur = pCur->child[word[i] - ' '];
	}
	pCur->fileArr[fileName].push_back(pos);
	maxwd[fileName] = (int)max(size_t(maxwd[fileName]), pCur->fileArr[fileName].size());
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
	return pCur->fileArr[fileName].size();
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
	while (input >> stopword) {
		stopwordsRoot->insert(stopword, "", 0);
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
			cout << "Failed load file " << fileName << "\n" << "\n";
			return false;
		}
		while (!fin1.eof()) {
			string word;
			int pos = fin1.tellg();
			fin1 >> word;
			if (stopwordsRoot->wordInFile(wordIgnore(word), "")) //Ignore stopwords
				continue;
			dataRoot->insert(wordIgnore(word), fileName, pos);
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


// function 1
//
//
Trie* getLeaf(string word) {
	Trie* temp = dataRoot;
	for (int i = 0; i < word.length(); i++) {
		int x = word[i] - ' ';
		temp = temp->child[x];
		if (!temp)
			break;
	}
	return temp;
}
void rankAndDisplay(map<string, double> score) {
	// sort and output
	map<string, double>::iterator it;
	vector<pair<double, string>> scoreArr;
	for (it = score.begin(); it != score.end(); it++)
		scoreArr.push_back(make_pair(it->second, it->first));

	sort(scoreArr.begin(), scoreArr.end());
	bool run = false;
	for (int i = scoreArr.size() - 1; i >= (int)scoreArr.size() - 5; i--) {
		if (i >= 0) {
			run = true;
			cout << " " << scoreArr[i].first << " " << scoreArr[i].second << endl;
		}
	}

	if (!run) {
		cout << "Your search did not match any documents." << endl;
		cout << "Suggestions:\n-Make sure that all words are spelled correctly. \n-Try different keywords.\nTry more general keywords.\n\n";
	}
}
map<string, double> function_1(string doc) {
	// array stored words
	vector<string> wordArr;
	string word;
	stringstream iss(doc);
	/*system("cls");*/
	while (iss >> word)
		if (!stopwordsRoot->wordInFile(wordIgnore(word), "") && word != "AND")
			wordArr.push_back(word);

	// array stored link
	map<string, int> fileNameList;
	map<string, int>::iterator _it;
	for (int i = 0; i < wordArr.size(); i++) {
		if (getLeaf(wordArr[i])) {
			// load filename cua word nay vo fileNameList
			map<string, vector<int> > file = getLeaf(wordArr[i])->fileArr;
			for (_it = file.begin(); _it != file.end(); _it++)
				fileNameList[_it->first]++;
			// neu file nay k bao gom tat ca word truoc, bi loai
			vector<string> del;
			for (_it = fileNameList.begin(); _it != fileNameList.end(); _it++) {
				if (_it->second != i + 1) {
					del.push_back(_it->first);
				}
			}
			// delete
			for (int i = 0; i < del.size(); i++)
				fileNameList.erase(del[i]);
		}
	}

	// score cac file trong bang sum cac tf-idf trong fileNameList
	map<string, double> score;
	for (int i = 0; i < wordArr.size(); i++) {
		if (getLeaf(wordArr[i])) {
			map<string, int> file = getLeaf(wordArr[i])->fileArr;
			for (_it = fileNameList.begin(); _it != fileNameList.end(); _it++) {
				//tf
				double tf = file[_it->first] * 1.0 / maxwd[_it->first];
				// tdf
				double tdf = 1; // vi so van ban chua word nay = tong so van ban minh dang xet.
				// score
				score[_it->first] += tf * tdf;
			}
		}
	}
	return score;
}

//
//
//
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