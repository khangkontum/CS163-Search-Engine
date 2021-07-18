#include <iostream>
#include <vector>
#include <sstream>
#include <string.h>
#include <fstream>
#include <cstdio>
#include <cmath> 
#include <iterator>
#include <map>
#include <algorithm>
#include "nlohmann/json.hpp"

#define pb push_back
#define fi first
#define se second

using namespace std;
using json = nlohmann::json;

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

vector<string> split(string s) {
	vector<string> trace;
	stringstream ss(s);
	string word;
	while (ss >> word) {
		trace.push_back(word);
	}
	return trace;
}

void display(string keyword, vector<string> fileNameList) {
    vector<pair<string, bool> > wordArray;
    vector<string> keywordArr = split(keyword);
    for (int i = 0; i < keywordArr.size(); i++) {
        keywordArr[i] = standardString(keywordArr[i]);\
    }
    string word;
    int cnt = 0;
    for (auto fileName : fileNameList) {
        cout << cnt + 1 << ". " << fileName << "\n";
        cnt++;
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

        
        bool* marked = new bool[keywordArr.size() + 5];
        for (int i = 0; i < keywordArr.size(); i++)
            marked[i] = false;

        int seqLen;
        int starting;
        for (int id = 0; id < wordArray.size(); id++) {
            seqLen = 10;
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
            while(seqLen--) {
                if(wordArray[starting].se) {
                    wordArray[starting].fi = "\033[32m" + wordArray[starting].fi + "\033[m";
                    seqLen++;
                }
                cout << wordArray[starting].fi << " ";
                starting++;
                if (starting == wordArray.size())
                    break;
            }
            cout << "...";
        }
        cout << "\n";

        if (cnt == 5)
            return;

        wordArray.clear();
    }
}

int main() {
    string s = "little hurricane a thorn";

    vector<string> trace{"test.txt"};

    display(s, trace);

    return 0;
}