#include"Header.h"

int main() {
	// loading data
	cout << "Data loading!";
	Trie* dataRoot = new Trie;
	Trie* stopwordsRoot = new Trie;
	Trie* thesaurusRoot = new Trie;
	bool check = loadData(dataRoot, stopwordsRoot, thesaurusRoot);
	system("clear");
	if (check)
		cout << "Loading successfully\n\n";
	else {
		cout << "The system can't access to your data!\n\n";
		exit(1);
	}
	

	// searhing menu
	cout << "Please choose one of the following queries: \n";
	
	cout <<  "1. AND \n2. OR \n3. Manchester-united \n4. Intitle \n5. Peanut Butter and Jam\n6. Filetype\n7. Search for a price\n8. Search hashtags\n9. Search for an exact match\n10. Search for wildcards or unknown words\n11. Search within a range of numbers\n12. Synonyms search\n0. Exit\n";
	
	cout << "Your query: ";
	int choose; 
	while (cin >> choose && choose) {
		switch (choose)
		{
		case 1:
			function_1(dataRoot);
			break;
		case 2:
			function_2(dataRoot);
			break;
		case 3:
			function_3(dataRoot);
			break;
		case 4:
			function_4(dataRoot);
			break;
		case 5:
			function_5(dataRoot);
			break;
		case 6:
			function_6(dataRoot);
			break;
		case 7:
			function_7(dataRoot);
			break;
		case 8:
			function_8(dataRoot);
			break;
		case 9:
			function_9(dataRoot);
			break;
		case 10:
			function_10(dataRoot);
			break;
		case 11:
			function_11(dataRoot);
			break;
		case 12:
			function_12(dataRoot);
			break;
		default:
			break;
		}
	}
	return 0;
}