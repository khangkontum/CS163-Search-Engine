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

	// searhing menu
	do {
		cout << "Input keyword (0 to exit): ";
		pair<int, string> command = getInput(); // command.first = commmand | command.second = keyword
		string keyword = command.second;
		switch (command.first)
		{
		case -1:	//Normal keyword

			break;
		case 0:
			return cout << "Program Stop!", 0;
			break;
		case 1:
			rankAndDisplay(function_1(command.second));
			break;
		case 2:
			break;
		case 3:		//	-
			break;
		case 4:		// intitle
			break;
		case 5:
			break;
		case 6:
			break;
		case 7:
			break;
		case 8:
			break;
		case 9:
			break;
		case 10:
			break;
		case 11:
			break;
		case 12:
			break;
		default:
			cout << "Input invalid or could not found keyword.\n";
			break;
		}
	} while (true);

	return 0;
}