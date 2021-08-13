#include"Header.h"

int totalWord = 0;
Trie* dataRoot = new Trie;
Trie* stopwordsRoot = new Trie;
Trie* thesaurusRoot = new Trie;
map<string, int> maxwd;		//the most word occur in file 

int main() {


	cout.precision(16);
	// loading data
	cout << "Data loading...\n";
	bool check = loadData();
	//system("cls");   //clear the screen

	if (check)
		cout << "Loading successfully\n\n" << "Total word: " << totalWord << "\n";
	else {
		cout << "The system can't access to your data!\n\n";
		exit(1);
	}

	// Searching Menu
	do {
		cout << "-----------------------------------------------\n";
		cout << "Input keyword (0 to exit): ";
		pair<int, string> command = getInput(); // command.first = commmand | command.second = keyword
		string keyword = command.second;
		
		switch (command.first)
		{
		case -1:	//Normal keyword
			normalSearchTmp(command.se);
			break;
		case 0:
			return cout << "Program Stop!", 0;
			break;
		case 1:
			rankAndDisplay(function_1(command.second), loadWordArr(command.second, 1));
			break;
		case 2:
			rankAndDisplay(function_2(command.second), loadWordArr(command.second, 2));
			break;
		case 3:
			rankAndDisplay(function_3(command.second), loadWordArr(command.second, 3));
			break;
		case 4:
			rankAndDisplay(function_4(command.second), loadWordArr(command.second, 4));
			break;
		case 5:
			function_5(command.se);
			break;
		case 6:
			function_6(command.se);
			break;
		case 7:
			rankAndDisplay(function_7(command.second), loadWordArr(command.second, 2));
			break;
		case 8:
			rankAndDisplay(function_8(command.second), loadWordArr(command.second, 2));
			break;
		case 9:
			exactMatch(command.se);
			break;
		case 11:
			function_11(command.se);
			break;
		case 12:
			function_12(command.se);
			break;
		default:
			cout << "Input invalid or could not found keyword.\n";
			break;
		}
	} while (true);

	return 0;
}