#include"Header.h"

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
	if (keyword.find(" OR ") != string::npos)
		return is(2, keyword);
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
	if (keyword.find("+") != string::npos)
		return is(5, keyword);
	//6
	//7
	if (keyword.find("$") != string::npos)
		return is(7, keyword);
	//8
	if (keyword[0] == '#')
		return is(8, keyword);
	//9
	if (keyword[0] == '"' && keyword[keyword.size() - 1] == '"')
		return is(9, keyword);

	//10 done in 9
	//11

	//12

	return is(-1, keyword); //Normal keyword
}

// Trie function

void Trie::insert(string word, string fileName, int pos) {
	Trie* pCur = this;
	for (int i = 0; i < word.length(); ++i) {
		if (letterToInt(word[i]) != -1) {
			if (!pCur->child[letterToInt(word[i])])
				pCur->child[letterToInt(word[i])] = new Trie();
			pCur = pCur->child[letterToInt(word[i])];
		}
	}
	pCur->fileArr[fileName].push_back(pos);
	maxwd[fileName] = (int)max(size_t(maxwd[fileName]), pCur->fileArr[fileName].size());
}

int Trie::wordInFile(string word, string fileName) {
	Trie* pCur = this;
	int x;
	for (int i = 0; i < word.length(); i++) {
		x = letterToInt(word[i]);
		if (x == -1)
			return 0;
		if (!pCur->child[x])
			return 0;
		pCur = pCur->child[x];
	}
	return pCur->fileArr[fileName].size();
}

// Load data function
int letterToInt(char a) {
	int x = (int)a;
	if ((x - 48) * (x - 57) <= 0) // 0->9 thi tu 26->35;
		return x - 48 + 26;
	if ((x - 65) * (x - 90) <= 0) // A->Z thi tu 0->25
		return x - 65;
	if ((x - 97) * (x - 122) <= 0) // a->z thi tu 0->25
		return x - 97;
	if ((x - 35) * (x - 36) == 0) // 36 37
		return x + 1;
	if ((x - 42) == 0)
		return 38;
	return -1;
}
string wordIgnore(string t) {
	string result = "";
	for (int i = 0; i < t.length(); i++) {
		if (int(t[i] - 33) * (int(t[i]) - 127) <= 0)
			result += t[i];
	}
	return result;
}
string standardString(string s) {
	string result = "";
	for (int i = 0; i < s.length(); i++) {
		if (int(s[i] - 33) * (int(s[i]) - 127) > 0)
			continue;
		if (isalpha(s[i]))
			result += tolower(s[i]);
		else if (isalnum(s[i]))
			result += s[i];
		else if (s[i] == '#' || s[i] == '$' || s[i] == '*' || s[i] == ' ')
			result += s[i];
	}
	return result;
}

bool loadData() {
	// load stopwords
	ifstream input("Database/Stopwords/stopwords.txt");
	string stopword;
	while (input >> stopword)
		stopwordsRoot->insert(stopword, "", 0);

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
			if (stopwordsRoot->wordInFile(standardString(word), "")) //Ignore stopwords
				continue;
			dataRoot->insert(standardString(word), fileName, pos);
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
	Trie* leaf = getLeaf(word);
	if (leaf == NULL)
		return 0;
	int ftd = leaf->fileArr[fileName].size();
	return double(ftd) / double(maxwd[fileName]);
}

double idf(string word) {
	string fileName;
	int fileCount = 0;
	int numOfFile = 0;
	Trie* leaf = getLeaf(word);
	if (leaf == NULL)
		return 0;
	ifstream fin("Database/Search Engine-Data/___index.txt");
	while (fin >> fileName) {
		numOfFile++;
		if (leaf->fileArr[fileName].size())
			fileCount++;
	}

	return log(double((numOfFile) / (1 + fileCount)));
}

bool compare(ds a, ds b) {
	return a.fi > b.fi;
}
vector<string> sortFile(vector<string> wordList, vector<string> fileNameList) {
	double D = fileNameList.size();
	double d = 0.0;
	
	Trie* leaf;
	vector<ds> trace;
	map<string, double> IDF;

	for (auto word : wordList) {
		leaf = getLeaf(word);
		if (leaf == NULL) {
			IDF[word] = 0;
			continue;
		}
		d = 0.0;
		for (auto fileName : fileNameList) {
			if (leaf->fileArr[fileName].size())
				d++;
		}
		IDF[word] = log(double(D / (1 + d)));
	}
	
	double sum;
	for (auto fileName : fileNameList) {
		sum = 0;
		for (auto word : wordList) {
			sum += tf(word, fileName) * (IDF[word] > 0 ? IDF[word] : 1) ;
		}
		trace.pb(ds(sum, fileName));
	}

	sort(trace.begin(), trace.end(), compare);
	/**/

	vector<string> tmp;
	for (auto x : trace) {
		tmp.pb(x.se);
	}

	return tmp;
	
}

Trie* getLeaf(string word) {
	Trie* temp = dataRoot;
	for (int i = 0; i < word.length(); i++) {
		int x = letterToInt(word[i]);
		temp = temp->child[x];
		if (!temp)
			return nullptr;
	}
	return temp;
}
void highlightLine(string get, vector<string> wordArr) {
	stringstream iss(get);
	string word;
	while (iss >> word) {
		bool check = true;
		for (int i = 0; i < wordArr.size(); i++) {
			if (wordArr[i] == standardString(word)) {
				check = false;
				cout << "\033[32m" << word << "\033[m" << " ";
				break;
			}
		}
		if (check)
			cout << word << " ";
	}
}
void rankAndDisplay(map<string, double> score, vector<string> wordArr) {
	// sort and output
	map<string, double>::iterator it;
	vector<pair<double, string> > scoreArr;
	for (it = score.begin(); it != score.end(); it++)
		scoreArr.push_back(make_pair(it->second, it->first));
	sort(scoreArr.begin(), scoreArr.end());
	bool run = false;
	for (int i = scoreArr.size() - 1; i >= (int)scoreArr.size() - 5; i--) {
		if (i >= 0) {
			run = true;
			cout << "LINK: " << scoreArr[i].second << endl;
			ifstream fin("Database/Search Engine-Data/" + scoreArr[i].second);
			if (fin.is_open()) {
				string get;
				while (!fin.eof()) {
					getline(fin, get, '.');
					for (int i = 0; i < wordArr.size(); i++)
						if (standardString(get).find(wordArr[i]) != string::npos) {
							highlightLine(get, wordArr);
							cout << endl;
							break;
						}
				}
			}
			fin.close();
			cout << "\n" << endl;
		}
	}
	if (!run) {
		cout << "Your search did not match any documents." << endl;
		cout << "Suggestions:\n-Make sure that all words are spelled correctly. \n-Try different keywords.\nTry more general keywords.\n\n";
	}
}
vector<string> loadWordArr(string doc, int x) {
	if (x == 3) {
		string mainDoc, deleteWord, minorDoc;
		stringstream iss(doc);
		system("cls");
		getline(iss, mainDoc, '-');
		iss >> deleteWord;
		getline(iss, minorDoc, '\n');
		doc = standardString(mainDoc) + standardString(minorDoc);
	}
	else if (x == 4) {
		string mainDoc;
		stringstream iss(doc);
		getline(iss, mainDoc, ':'); // ignore titile
		getline(iss, mainDoc, '\n');
		doc = mainDoc;
	}
	vector<string> wordArr;
	string word;
	stringstream iss(doc);
	system("cls");
	while (iss >> word)
		if (!stopwordsRoot->wordInFile(standardString(word), ""))
			wordArr.push_back(standardString(word));
	return wordArr;
}

// function 1
map<string, double> function_1(string doc) {
	// array stored words
	vector<string> wordArr = loadWordArr(doc, 1);

	// array stored link
	map<string, int> fileNameList;
	map<string, int>::iterator _it;
	map<string, vector<int> >::iterator it;
	bool checkNull = true;

	for (int i = 0; i < wordArr.size(); i++) {
		if (getLeaf(wordArr[i])) {
			// load filename cua word nay vo fileNameList
			map<string, vector<int> > file = getLeaf(wordArr[i])->fileArr;

			for (it = file.begin(); it != file.end(); it++)
				fileNameList[it->first]++;
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
		else {
			checkNull = false;
			break;
		}
	}

	// score cac file trong bang sum cac tf-idf trong fileNameList
	map<string, double> score;
	if (checkNull)
		for (int i = 0; i < wordArr.size(); i++) {
			if (getLeaf(wordArr[i])) {
				map<string, vector<int> > file = getLeaf(wordArr[i])->fileArr;
				for (_it = fileNameList.begin(); _it != fileNameList.end(); _it++) {
					// score
					score[_it->first] += tf(wordArr[i], _it->first); // vi idf = 0 nen minh chi can 
																	 //so sanh tf la du
				}
			}
		}
	return score;
}

//function 2
map<string, double> function_2(string doc) {
	// array stored words
	vector<string> wordArr = loadWordArr(doc, 2);

	// array stored link
	map<string, int> fileNameList;
	map<string, int>::iterator _it;
	map<string, vector<int> >::iterator it;
	bool checkNull = true;

	for (int i = 0; i < wordArr.size(); i++) {
		if (getLeaf(wordArr[i])) {
			// load filename cua word nay vo fileNameList
			map<string, vector<int> > file = getLeaf(wordArr[i])->fileArr;
			for (it = file.begin(); it != file.end(); it++)
				fileNameList[it->first]++;
		}
		else {
			checkNull = false;
			break;
		}
	}

	// score cac file trong bang sum cac tf-idf trong fileNameList
	map<string, double> score;
	if (checkNull)
		for (int i = 0; i < wordArr.size(); i++) {
			if (getLeaf(wordArr[i])) {
				map<string, vector<int> > file = getLeaf(wordArr[i])->fileArr;
				for (_it = fileNameList.begin(); _it != fileNameList.end(); _it++) {
					//tf
					double TF = tf(wordArr[i], _it->first);
					// tdf
					double tdf = log(double(fileNameList.size() / (1 + file[_it->first].size())));
					// score
					score[_it->first] += TF * tdf;
				}
			}
		}
	return score;
}

// function 3
map<string, double> function_3(string doc) {
	// vi du search: manchester -united soccer, thi minh se search keyword : machester, soccer 
	// va loai tru nhung file co tu united (google no lam vay)

	// search left-hand side
	string mainDoc, deleteWord, minorDoc;
	stringstream iss(doc);
	system("cls");
	getline(iss, mainDoc, '-');
	iss >> deleteWord;
	getline(iss, minorDoc, '\n');
	map<string, double> score = function_2(standardString(mainDoc) + standardString(minorDoc));
	// cho nay la search theo OR(function_2) thi dung hon, khi nao viet xong het roi minh edit lai sau.

	// delete unwanted text
	if (getLeaf(deleteWord)) {
		map<string, vector<int> > file = getLeaf(deleteWord)->fileArr;
		map<string, vector<int> >::iterator it;
		for (it = file.begin(); it != file.end(); ++it)
			if (score.find(it->first) != score.end()) {
				score.erase(it->first);
			}

	}
	return score;
}

//function 4
map<string, double> function_4(string doc) {
	// array stored words
	vector<string> wordArr = loadWordArr(doc, 4);

	// array stored link
	map<string, int> fileNameList;
	map<string, int>::iterator _it;
	map<string, vector<int> >::iterator it;
	bool checkNull = true;

	for (int i = 0; i < wordArr.size(); i++) {
		if (getLeaf(wordArr[i])) {
			// load filename cua word nay vo fileNameList
			map<string, vector<int> > file = getLeaf(wordArr[i])->fileArr;
			for (it = file.begin(); it != file.end(); it++)
				if (it->second[0] < 150)
					fileNameList[it->first]++;
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
		else {
			checkNull = false;
			break;
		}
	}

	// score cac file trong bang sum cac tf-idf trong fileNameList
	map<string, double> score;
	if (checkNull)
		for (int i = 0; i < wordArr.size(); i++) {
			if (getLeaf(wordArr[i])) {
				map<string, vector<int> > file = getLeaf(wordArr[i])->fileArr;
				for (_it = fileNameList.begin(); _it != fileNameList.end(); _it++) {
					// score
					score[_it->first] += tf(wordArr[i], _it->first);
				}
			}
		}
	return score;
}


//String function
vector<string> split(string s) {
	vector<string> trace;
	stringstream ss(s);
	string word;
	while (ss >> word) {
		trace.push_back(word);
	}
	return trace;
}

string subtract(string s, int start, int end) {
	string tmp = "";
	for (int i = start; i <= end; i++)
		tmp += s[i];
	return tmp;
}


//exact match, return position of first character
int continuosString(vector<string> wordList, vector<int> trace, string fileName) {
	string tmp;
	ifstream fin("Database/Search Engine-Data/" + fileName);
	bool found;
	for (auto x : trace) {
		fin.clear();
		fin.seekg(x);
		found = true;
		for (auto word : wordList) {
			if (word == "*") {
				fin >> tmp;
				continue;
			}
			do {
				fin >> tmp;
				tmp = standardString(tmp);
			}while(stopwordsRoot->wordInFile(tmp, ""));

			if (tmp != word) {
				found = false;
				break;
			}
		}
		if (found) {
			fin.close();
			return x;
		}
	}
	fin.close();
	return -1;
}

vector<string> normalSearch(string keyword) {
	vector <string> wordList;
	vector <string> tmp;
	tmp = split(keyword);
	for (auto word : tmp) {
		word = standardString(word);
		if (stopwordsRoot->wordInFile(word, ""))
			continue;
		wordList.pb(word);
	}

	sd maxIdf = sd("", -1.0);
	for (int i = 0; i < wordList.size(); i++) {
		if (wordList[i] == "*")
			continue;
		int curIdf = idf(wordList[i]);
		if (curIdf > maxIdf.se) {
			maxIdf = sd(wordList[i], curIdf);
		}
	}
	
	Trie* leaf = getLeaf(maxIdf.fi);
	vector<string> fileNameList;
	for (auto fileName : leaf->fileArr) {
		if (fileName.se.size())
			fileNameList.pb(fileName.fi);
	}
	sortFile(wordList, fileNameList);

	return fileNameList;
}

void normalSearchTmp(string keyword) {
	vector<string> fileNameList = normalSearch(keyword);
	for (int i = 0; i < min(size_t(5), fileNameList.size()); i++)
		cout << fileNameList[i] << "\n";
	return;
}

void function_5(string keyword) {
	int mid = keyword.find('+');
	string normal_kw = subtract(keyword, 0, mid - 1);
	string exact_kw = subtract(keyword, mid + 1, keyword.size());

	string dummy = normal_kw + exact_kw;
	vector<string> fileNameList = normalSearch(dummy);

	vector<string> normalList;
	vector<string> exactList;
	
	vector <string> tmp;
	tmp = split(exact_kw);
	for (auto word : tmp) {
		word = standardString(word);
		if (stopwordsRoot->wordInFile(word, ""))
			continue;
		exactList.pb(word);
	}

	vector<string> result;
	for (auto fileName : fileNameList) {
		int begin = isSequenceInFile(exactList, fileName);
		if (begin != -1) {
			result.pb(fileName);
		}
	}
	for (int i = 0; i < min(size_t(5), result.size()); i++)
		cout << result[i] << "\n";
}
void function_6(Trie* root) {

}
// function 7
map<string, double> function_7(string doc) {
	return function_2(doc);
}
// function 8
map<string, double> function_8(string doc) {
	return function_2(doc);
}

int isSequenceInFile(vector<string> wordList, string fileName) {
	string word;
	for (auto x : wordList) {
		if (!stopwordsRoot->wordInFile(x, "")) {
			word = x;
			break;
		}
	}
	Trie* leaf = getLeaf(word);
	if (leaf == NULL)
		return -1;
	vector<int> trace = leaf->fileArr[fileName];

	return continuosString(wordList, trace, fileName);
}

//function 9 NOT DONE
void exactMatch(string keyword) {
	keyword = subtract(keyword, 1, keyword.size() - 2);
	
	vector<string> tmpWord = split(keyword);
	vector<string> wordList;
	for (auto word : tmpWord) {
		word = standardString(word);
		if (stopwordsRoot->wordInFile(word, ""))
			continue;
		wordList.pb(word);
	}

	sd maxIdf = sd("", -1.0);
	
	for (int i = 0; i < wordList.size(); i++) {
		if (wordList[i] == "*")
			continue;
		int curIdf = idf(wordList[i]);
		if (curIdf > maxIdf.se) {
			maxIdf = sd(wordList[i], curIdf);
		}
	}
	
	Trie* leaf = getLeaf(maxIdf.fi);
	vector<string> fileNameList;
	for (auto fileName : leaf->fileArr) {
		if (fileName.se.size())
			fileNameList.pb(fileName.fi);
	}
	
	vector<si> result;
	for (auto fileName : fileNameList) {
		int begin = isSequenceInFile(wordList, fileName);
		if (begin != -1)
			result.pb(si(fileName, begin));
	}

	
	vector<string> tmp;
	for (auto x : result) {
		tmp.pb(x.fi);
	}
	vector<string> trace = sortFile(wordList, tmp);
	for (auto x : trace) 
		cout << x << "\n";
	/**/
}

void function_10(Trie* root) {

}
void function_11(Trie* root) {

}
void function_12(Trie* root) {

}