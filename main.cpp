#include"Header.h"

int main() {
	// loading data
	cout << "Data loading...\n";
	Trie* root = new Trie;
	Trie* stopwordsRoot = new Trie;
	Trie* thesaurusRoot = new Trie;
	bool check = loadData(root, stopwordsRoot, thesaurusRoot);
	//system("cls");   //clear the screen

	if (check)
		cout << "Loading successfully\n\n";
	else {
		cout << "The system can't access to your data!\n\n";
		exit(1);
	}

	
	// searhing menu
	do{
		cout << "Input keyword (0 to exit): ";
		pair<int, string> command = getInput(); // command.first = commmand | command.second = keyword
		string keyword = command.second;
		switch (command.first)
		{
			case -1:	//Normal keyword

				break;
			case 0:
				cout << "Program Stop!";
				break;
			case 1:
				break;
			case 2:
				break;
			case 3:		//	-
				leaveOut(root, keyword);
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
	}while(true);

	return 0;
}