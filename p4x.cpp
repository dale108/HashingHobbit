// p4.cpp
// TODO: Complete all the code that is marked as TODO in the file
// TODO: add functional documentation (and inline comments, as necessary)

#include "WordCounterx.h"
#include "CommonWordList.h"

#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <algorithm>
using namespace std;


struct LoadedPassage {
	string passageTitle;
	WordCounter * wordCounter;
};
// Struct that holds the title and WordCounter object for all loaded passages.


void welcome();
// Prints welcome message.

void goodbye();
// Prints goodbye message.

void help();
// Prints help menu.

bool processLine(string, vector<LoadedPassage *> &);
// Process the line entered into the prompt.

void loadFileCmd(vector<string>, vector<LoadedPassage *> &);
// Loads the given file into the system. The words are processed and counted.
// The results of the load can be displayed with the list command. Last char
// in text must not be hyphen

void listCmd(vector<LoadedPassage *>);
// Lists the currently loaded passages and their statistics.

void analyzeFileCmd(vector<string>, vector<LoadedPassage *>);
// Analyzes the given text or file contents.

void analyzeFile(string, vector<LoadedPassage *>);
// Analyzes the words that are read from the file instead of user entry.

void analyzeText(vector<string>, vector<LoadedPassage *>);
// Analyzes the given text against the loaded passages.

void removeCommonWords(WordCounter &);
// Iterates through all the known common words and removes them from the
// given counter.

void removeDuplicateAndCommonWords(vector<string> &);

vector<string> delimitBySpace(string &);
// Delimits (by space) the string from user input. Populates & returns vector of
// the vector arguments.

string cleanWord(string);
// Removes any special characters that are not hyphen or apostrophe. Checks that
// a letter/number precedes apostophe and a letter/number precedes and succeeds
// the hyphen. Returns the clean result.


const char apostropheChar = '\''; 	// apostrophe character used in cleanWord
const char hyphenChar = '-'; 			// hyphen/dash character used in cleanWord
const char plusChar = '+';				// plus character
const char spaceChar = ' ';			// space character


int main() {
	// declare variables
	vector<LoadedPassage *> passages;
	string line;

	// populate common words
	CommonWordList::populateWords();

	// welcome message
	welcome();

	// process commands
	do {
		cout << "\nanalyzer> ";
		getline(cin, line);
	} while (processLine(line, passages));

	// deallocate memory from objects in vector
	for(int i = 0; i < passages.size(); ++i ) {
		delete passages[i];
	}

	// goodbye message
	goodbye();
}

bool processLine(string line, vector<LoadedPassage *> &passages) {
	// check that command was provided
	vector<string> commands = delimitBySpace(line);
	if (commands.size() == 0) {
		cout << "Error: no command given.";
		return false;
	}

	// get command from line
	string cmd = commands[0];

	// process user input
	if (cmd == "help")
		help();
	else if (cmd == "load")
		loadFileCmd(commands, passages);
	else if (cmd == "analyze")
		analyzeFileCmd(commands, passages);
	else if (cmd == "list")
		listCmd(passages);
	else if (cmd == "quit")
		return false;
	else
		cout << "Error: unrecognized command: " << cmd << endl;

	return true;
}

void loadFileCmd(vector<string> cmds, vector<LoadedPassage *> &passages) {
	// check if enough load commands provided
	if (cmds.size() != 2) {
		cout << "Error: Hash Table size and/or file not provided.\n";
		return;
	}

	// declare & populate variables
	string filename = cmds[1];
	string word;
	ifstream infile;
	LoadedPassage * lp = new LoadedPassage;
	lp->passageTitle = filename;
	lp->wordCounter = new WordCounter;

	// read from file and populate
	infile.open(filename);
	if (infile) {
		while (infile >> word) {
			// since hyphen can never grammatically be ther last character in a
			// document, this solution is reasonable
			if(word.back() == hyphenChar && isalnum(word[word.length()-1])) {
				cout << "the inital word is: " << word << endl;
				string temp = word;
				infile >> word;
				word  = temp + word;
				cout << "then after concatination: " << word;
			}
			word = cleanWord(word);
			if (word.length() > 0)
				lp->wordCounter->addWord(word);
		}
		cout << "file " << filename << " loaded" << endl;
	} else {
		cout << "Error: File could not be opened.\n";
		return;
	}

	// close file
	infile.close();

	// remove common words
	removeCommonWords(*lp->wordCounter);

	// add to vector
	passages.push_back(lp);
}

void listCmd(vector<LoadedPassage *> passages) {
	// check if any passages are loaded
	if (passages.size() == 0) {
		cout << "Error: There are no loaded passages to list.\n";
		return;
	}

	// print out passages and statistics
	cout << "  ID   #Unique   #Total   Load F.   Passage Description  \n"
		  << "+----+---------+--------+---------+---------------------+\n";

	for (unsigned i = 0; i < passages.size(); i++) {
		LoadedPassage * lp = passages[i];
		printf("%4d", i + 1);
		printf("%10d", (lp->wordCounter)->getUniqueWordCount());
		printf("%9d", (lp->wordCounter)->getTotalWordCount());
		double lf = static_cast<double>((lp->wordCounter)->getUniqueWordCount())
								/ (lp->wordCounter)->getCapacity();
		printf("%9.3f", lf);
		cout << "    " << lp->passageTitle;
		cout << endl;
	}
}

void analyzeFileCmd(vector<string> cmds, vector<LoadedPassage *> passages) {
	// check if enough analyze commands were provided
	if (passages.size() == 0) {
		cout << "Error: There are no loaded passages to analyze.\n";
		return;
	}
	if (cmds.size() == 1) {
		cout << "Error: The analyze type was not provided.\n";
		return;
	}
	if (cmds.size() == 2) {
		cout << "Error: Nothing was provided to analyze.\n";
		return;
	}

	// get type of analysis and run analysis
	string typeAnalysis = cmds[1];
	if (typeAnalysis == "text") {
		// remove first 2 elements from vector before analysis
		cmds.erase(cmds.begin(), cmds.begin() + 2);
		analyzeText(cmds, passages);
	} else if (typeAnalysis == "file") {
		string filename = cmds[2];
		analyzeFile(filename, passages);
	} else
		cout << "Error: Unknown analysis type.\n";
}

void analyzeFile(string filename, vector<LoadedPassage *> passages) {
	ifstream infile;
	vector<string> words;
	string word;

	// read from file and populate
   infile.open(filename);
   if (infile) {
      while (infile >> word) {
         word = cleanWord(word);
         if (word.length() > 0)
            words.push_back(word);
      }

		// analysize words
		analyzeText(words, passages);
   } else {
      cout << "Error: File could not be opened.\n";
      return;
   }

	// close file
	infile.close();
}

void analyzeText(vector<string> words, vector<LoadedPassage *> passages) {
	// remove duplicates and common words
	removeDuplicateAndCommonWords(words);

	// check if there are any words to analyze
	if (words.size() == 0) {
		cout << "Error: All words provided were common words.\n";
	}

	// print analysis table heading
	vector<int> titleLengths;
	titleLengths.push_back(14);
	printf("%16s", "given word");
	for (LoadedPassage * entry : passages) {
		titleLengths.push_back(entry->passageTitle.length());
		cout << "   " + entry->passageTitle;
	}
	cout << endl << plusChar;
	for (int i : titleLengths) {
		for (int j = 0; j < i + 2; j++)
			cout << hyphenChar;
		cout << plusChar;
	}
	cout << endl;

	// print analysis table results
	for (string &word : words) {
		printf("%16s", word.c_str());
		for (LoadedPassage * entry : passages) {
			int count = entry->wordCounter->getWordCount(word);
			for (unsigned i = 0; i < entry->passageTitle.length(); i++)
				cout << spaceChar;
			printf("%3d", count);
		}
		cout << endl;
	}
}

void removeCommonWords(WordCounter &wc) {
	for (string &s : CommonWordList::getWords())
		wc.removeWord(s);
}

void removeDuplicateAndCommonWords(vector<string> &words) {
	vector<string>::iterator it;
	vector<string> updatedWords;
	vector<string> commonWords = CommonWordList::getWords();
	string clean;

	// remove common, duplicates, and cleans
	for (string &s : words) {
		clean = cleanWord(s);
		it = find(commonWords.begin(), commonWords.end(), clean);
		if (it == commonWords.end()) {
			it = find(updatedWords.begin(), updatedWords.end(), clean);
			if (it == updatedWords.end())
				updatedWords.push_back(clean);
		}
	}
	words.clear();
	words = updatedWords;
}

vector<string> delimitBySpace(string &line) {
	vector<string> result;
	stringstream ss(line);
	string str;
	while (getline(ss, str, ' '))
		result.push_back(str);
	return result;
}

string cleanWord(string s) {
	string result = "";
	unsigned i = 0, length = s.length();
	for (; i < length; i++) {
		if (isalnum(s[i]))
			result += tolower(s[i]);
		else if (s[i] == apostropheChar) {
			if (i > 0 && isalnum(s[i-1]))
				result += tolower(s[i]);
		} else if (s[i] == hyphenChar) {
			if (i > 0 && i < (length-1) && isalnum(s[i-1]) && isalnum(s[i+1]))
				result += tolower(s[i]); // changed these from s[i] to tolower(s[i])
		}
	}
	return result;

}

void welcome() {
	cout << "\n\nWelcome to the Word Analyzes program!" << endl;
	cout << "This program will analyze a .txt file and display information";
	cout << " about it's contents." << endl;
	cout << "Input 'help' and press enter at any time to see a list of " << endl;
	cout << "commands and how to use them" << endl;
}

void goodbye() {
	// TODO: update goodbye message
	cout << "\n\nThank you for using the Word Analysis program!\n\n";
}

void help() {
	cout << "load <hash-table-size> <file-name>   \n" <<
"           Creates a Hash Table with the given size, reads the words from \n" <<
"           the file, and counts the occurrences to be analyzed later\n\n" <<
"analyze text <text-entry>\n" <<
"           Displays the entered words and shows their frequencies in each\n" <<
"           passage. Words that are considered too common are skipped\n\n" <<
"analyze file <file-name>\n" <<
"           Displays the words loaded from file and shows their frequencies\n" <<
"           in each passage. Words that are too common are skipped\n\n" <<
"list       Displays the list of all loaded passages in this format:\n" <<
"             ID   #Unique   #Total   Load F.   Passage Description  \n" <<
"           +----+---------+--------+---------+---------------------+\n\n" <<
"help       Displays this help menu.\n\n" <<
"quit       Exits the program.\n";
}
