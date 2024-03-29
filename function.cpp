#include"Header.h"

// Input function
is getInput() {
	string keyword;
	do {
		getline(cin, keyword, '\n');
	} while(keyword.size() == 0);
	if (keyword == "0")
		return is(0, "");

	//9
	if (keyword[0] == '"' && keyword[keyword.size() - 1] == '"')
		return is(9, keyword);

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
	int id = keyword.find("filetype:");
	if (id != string::npos && id == 0)
		return is(6, keyword);
	//11
	for (int i = 0; i < keyword.size(); i++)
	{
		if (keyword[i] == '$') {
			int j = i + 1;
			while (j < keyword.size() && keyword[j] - '0' >= 0 && keyword[j] - '0' <= 9)
				j++;
			if (j >= keyword.size() - 3)
				break;
			if (keyword[j] != '.')
				break;
			if (keyword[j] == '.' && keyword[j + 1] == '.' && keyword[j + 2] == '$')
				return is(11, keyword);
		}
	}
	//7
	if (keyword.find("$") != string::npos)
		return is(7, keyword);
	//8
	if (keyword[0] == '#')
		return is(9, '"' + keyword + '"');

	//12
	if (keyword[0] == '~')
		return is(12, keyword);

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
	pCur->isTail = true;
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
	if ((x - 48) * (x - 57) <= 0) // 0->9: 26->35;
		return x - 48 + 26;
	if ((x - 65) * (x - 90) <= 0) // A->Z: 0->25
		return x - 65;
	if ((x - 97) * (x - 122) <= 0) // a->z: 0->25
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
		else if (s[i] == '#' || s[i] == '$' || s[i] == '*' || s[i] == ' ' || s[i] == '~')
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
	while (!fin.eof() && FILE_NUM--) {
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
			dataRoot->insert(standardString(word), fileName, pos);
			totalWord++;
		}
		fin1.close();
	}
	fin.close();

	// load thesaurus
	fin.open("Database/Thesaurus/en_thesaurus.jsonl");
	json j;
	string s;
	int cnt = 10;
	while (getline(fin, s)) {
		j = json::parse(s);
		string word = standardString(j["word"].dump());
		for (auto sy : j["synonyms"]) {
			if (sy.dump().find(" ") != string::npos)
				continue;
			thesaurusRoot->insert(word, standardString(sy.dump()), 0);
		}
	}
	fin.close();

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
		if (leaf->fileArr.find(fileName) != leaf->fileArr.end())
			fileCount++;
	}
	return log(double((numOfFile) / (1 + fileCount)));
}

void display(string keyword, vector<string> fileNameList) {
	if (fileNameList.size() == 0) {
		cout << "Your search did not match any documents.\n";
		cout << "Suggestions:\n";
		cout << "- Make sure that all words are spelled correctly.\n";
		cout << "- Try different keywords.\n";
		cout << "- Try more general keywords.\n";
		cout << "- Keyword with only stopwords will not get any result!\n";
		return;
	}

	vector<pair<string, bool> > wordArray;
	vector<string> keywordArr = split(keyword);
	for (int i = 0; i < keywordArr.size(); i++) {
		keywordArr[i] = standardString(keywordArr[i]);
	}
	ignoreStopwords(keywordArr);
	string word;
	int cnt = 0;
	for (auto fileName : fileNameList) {
		cnt++;
		cout << '[' << cnt << "] " << fileName << "\n";
		ifstream fin("Database/Search Engine-Data/" + fileName);
		while (fin >> word) {
			bool isKey = false;
			string tmp = standardString(word);
			for (auto keyword : keywordArr) {
				if (tmp == keyword) {
					isKey = true;
					break;
				}
			}
			wordArray.pb(pair<string, bool>(word, isKey));
		}
		fin.close();

		bool* marked = new bool[keywordArr.size() + 5];
		for (int i = 0; i < keywordArr.size(); i++)
			marked[i] = false;

		int seqLength;
		int starting;
		int maxWord = 100;
		for (int id = 0; id < wordArray.size(); id++) {
			seqLength = 10;
			starting = -1;
			string word = standardString(wordArray[id].fi);
			if (wordArray[id].se == false)
				continue;

			for (int i = 0; i < keywordArr.size(); i++) {
				if (keywordArr[i] == word) {
					if (marked[i] == true)
						break;
					else {
						starting = max(id - 1, 0);
						marked[i] = true;
					}
				}
			}
			if (starting == -1)
				continue;
			cout << "...";
			while (seqLength--) {
				if (wordArray[starting].se) {
					wordArray[starting].fi = "\033[32m" + wordArray[starting].fi + "\033[m";
					seqLength++;
				}
				cout << wordArray[starting].fi << " ";
				maxWord--;
				starting++;
				if (starting == wordArray.size())
					break;
			}
			if (maxWord <= 0)
				break;
		}
		cout << "...\n\n";

		if (cnt == 5)
			break;

		wordArray.clear();
	}
	int command = 0;
	cout << "Enter file number to preview (0 to continue search): ";
	cin >> command;
	while (command < 0 || command > min(size_t(5), fileNameList.size())) {
		cout << "Please enter correctly (0-5): ";
		cin >> command;
	}
	if (command == 0)
		return;
	cout << "--------------------------------------\n";
	string fileName = fileNameList[command - 1];
	ifstream fin("Database/Search Engine-Data/" + fileName);
	string line;
	while (getline(fin, line)) {
		stringstream linestream(line);
		while(linestream >> word) {
			bool isKey = false;
			string tmp = standardString(word);
			for (auto keyword : keywordArr) {
				if (tmp == keyword) {
					isKey = true;
					break;
				}
			}
			if (isKey) {
				cout << "\033[32m" + word + "\033[m ";
			}
			else 
				cout << word << ' ';
		}
		cout << '\n';
	}
	fin.close();
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
			sum += tf(word, fileName) * (IDF[word] > 0 ? IDF[word] : 1);
		}
		trace.pb(ds(sum, fileName));
	}

	sort(trace.begin(), trace.end(), compare);

	vector<string> tmp;
	for (auto x : trace) {
		tmp.pb(x.se);
	}

	return tmp;

}

Trie* getLeaf(string word, Trie*& root) {
	Trie* temp = root;
	for (int i = 0; i < word.length(); i++) {
		int x = letterToInt(word[i]);
		if (temp->child[x] == nullptr)
			return nullptr;
		temp = temp->child[x];
	}
	if (temp->isTail == true)
		return temp;
	else
		return nullptr;
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
	vector<string> fileNameList;
	for (int i = scoreArr.size() - 1; i >= (int)scoreArr.size() - 5; i--)
		if (i >= 0)
			fileNameList.push_back(scoreArr[i].second);
	string s = "";
	for (int i = 0; i < (int)wordArr.size(); i++) {
		s += wordArr[i] + " ";
	}
	display(s, fileNameList);
}
vector<string> loadWordArr(string doc, int x) {
	if (x == 3) {
		string mainDoc, deleteWord, minorDoc;
		stringstream iss(doc);
		//system("cls");
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
	//system("cls");
	while (iss >> word)
		if (!stopwordsRoot->wordInFile(standardString(word), ""))
			wordArr.push_back(standardString(word));
	return wordArr;
}

// function 1: AND
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

//function 2: OR
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

// function 3: Manchester �united
map<string, double> function_3(string doc) {
	// vi du search: manchester -united soccer, thi minh se search keyword : machester, soccer 
	// va loai tru nhung file co tu united (google no lam vay)

	// search left-hand side
	string mainDoc, deleteWord, minorDoc;
	stringstream iss(doc);
	//system("cls");
	getline(iss, mainDoc, '-');
	iss >> deleteWord;
	getline(iss, minorDoc, '\n');
	map<string, double> score = function_2(standardString(mainDoc) + standardString(minorDoc));

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

//function 4: intitle:hammer nails
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

	while (ss >> word)
		trace.push_back(word);

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
			fin >> tmp;
			if (standardString(tmp) != word) {
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
	Trie* leaf;
	tmp = split(keyword);

	for (auto word : tmp) {
		word = standardString(word);
		leaf = getLeaf(word, stopwordsRoot);
		if (leaf != nullptr)
			continue;
		wordList.pb(word);
	}

	sd maxIdf = sd("", -1.0);
	for (int i = 0; i < wordList.size(); i++) {
		if (wordList[i] == "*")
			continue;
		int curIdf = idf(wordList[i]);
		if (curIdf > maxIdf.se)
			maxIdf = sd(wordList[i], curIdf);
	}

	if (maxIdf.fi == "")
		return {};

	leaf = getLeaf(maxIdf.fi);
	if (leaf == NULL)
		return {};

	vector<string> fileNameList;
	for (auto fileName : leaf->fileArr)
		if (fileName.se.size())
			fileNameList.pb(fileName.fi);

	sortFile(wordList, fileNameList);

	return fileNameList;
}

void normalSearchTmp(string keyword) {
	vector<string> fileNameList = normalSearch(keyword);
	display(keyword, fileNameList);
	return;
}

// function 5: Peanut Butter +and Jam
void function_5(string keyword) {
	int mid = keyword.find('+');
	string normal_kw = subtract(keyword, 0, mid - 1);
	string exact_kw = subtract(keyword, mid + 1, keyword.size());

	string dummy = normal_kw + " " + exact_kw;
	vector<string> fileNameList = normalSearch(dummy);

	vector<string> normalList;
	vector<string> exactList;

	exactList = split(exact_kw);

	vector<string> result;
	Trie* leaf;
	for (auto fileName : fileNameList) {
		bool ok = true;
		for (auto word : exactList) {
			leaf = getLeaf(standardString(word));
			if (leaf == NULL) {
				display("", {});
				return;
			}
			if (leaf->fileArr.find(fileName) == leaf->fileArr.end()) {
				ok = false;
				break;
			}
		}
		if (ok)
			result.push_back(fileName);
	}
	/**/
	display(normal_kw + " " + exact_kw, result);
}

// function 6: filetype:txt
void function_6(string keyword) {
	string fileType = subtract(keyword, 9, keyword.size());
	string fileName;
	ifstream fin("Database/Search Engine-Data/___index.txt");
	int cnt = 0;
	while (fin >> fileName) {
		int id = fileName.size() - fileType.size() + 1;
		if (id < 0)
			continue;
		string tmp = subtract(fileName, id, fileName.size());
		if (tmp == fileType) {
			cnt++;
			string file = subtract(fileName, 0, id - 1);
			string tail = subtract(fileName, id, fileName.size());
			cout << file << "\033[32m" << tail << "\033[m" << "\n";
		}
		if (cnt == 5)
			return;
	}
	if (cnt == 0)
		display("",{});
}

// function 7: Search for a price
map<string, double> function_7(string doc) {
	return function_1(doc);
}

// function 8: Search hashtag
map<string, double> function_8(string doc) {
	return function_2(doc);
}

int isSequenceInFile(vector<string> wordList, string fileName) {
	Trie* leaf = getLeaf(wordList[0]);

	if (leaf == NULL || leaf->fileArr.find(fileName) == leaf->fileArr.end())
		return -1;

	return continuosString(wordList, leaf->fileArr[fileName], fileName);
}

void ignoreStopwords(vector<string> &wordList) {
	vector<string> result;
	Trie* leaf;
	for (auto word : wordList) {
		leaf = getLeaf(word, stopwordsRoot);
		if (leaf == NULL)
			result.pb(word);
	}
	wordList = result;
}

//function 9: Search for exact math 
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
	if (leaf == nullptr) {
		display("", {});
		return;
	}
	vector<string> fileNameList;
	for (auto fileName : leaf->fileArr) {
		if (fileName.se.size())
			fileNameList.pb(fileName.fi);
	}

	vector<string> result;
	for (auto fileName : fileNameList) {
		int begin = isSequenceInFile(wordList, fileName);
		if (begin != -1)
			result.pb(fileName);
	}

	vector<string> trace = sortFile(wordList, result);
	display(keyword, trace);
}

// function 11: Search within a range of numbers
void function_11(string keyword) {
	string item = "";
	int minx = 0;
	int maxx = 0;
	for (int i = 0; i < keyword.size(); i++)
	{
		if (keyword[i] == '$') {
			int j = i + 1;
			while (j < keyword.size() && keyword[j] - '0' >= 0 && keyword[j] - '0' <= 9) {
				minx = minx * 10 + (keyword[j] - '0');
				j++;
			}
			if (j >= keyword.size() - 3)
				break;
			if (keyword[j] != '.')
				break;
			if (keyword[j] == '.' && keyword[j + 1] == '.' && keyword[j + 2] == '$') {
				string tmp = subtract(keyword, j + 3, keyword.size() - 1);
				for (auto x : tmp) {
					maxx = maxx * 10 + (x - '0');
				}
				break;
			}
		}
		else item += keyword[i];

	}
	map<string, double> tfScore;
	vector<string> wordArr;
	// load tfScore to Array
	wordArr.push_back(item);
	for (int i = minx; i <= maxx; i++) {
		string price = to_string(i);
		wordArr.push_back(" $" + price);
		auto s = function_1(item + " $" + price);
		for (auto it : s) {
			tfScore[it.fi] += it.se;
		}
	}
	rankAndDisplay(tfScore, wordArr);
}

// function 12
void function_12(string keyword) {
	keyword = subtract(keyword, 1, keyword.size());
	keyword = standardString(keyword);

	Trie* leaf = getLeaf(keyword, thesaurusRoot);
	if (leaf == NULL) {
		cout << "There are no synonyms for this word.\n";
		return;
	}
	keyword = "";
	for (auto word : leaf->fileArr) {
		keyword += " " + word.fi;
	}
	normalSearchTmp(keyword);
}