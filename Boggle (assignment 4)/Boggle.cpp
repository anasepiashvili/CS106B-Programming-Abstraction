/*
 * File: Boggle.cpp
 * ----------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the main starter file for Assignment #4, Boggle.
 * [TODO: extend the documentation]
 */

#include <iostream>
#include "gboggle.h"
#include "grid.h"
#include "gwindow.h"
#include "lexicon.h"
#include "random.h"
#include "simpio.h"
using namespace std;

/* Constants */

const int BOGGLE_WINDOW_WIDTH = 650;
const int BOGGLE_WINDOW_HEIGHT = 350;

const string STANDARD_CUBES[16]  = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};
 
const string BIG_BOGGLE_CUBES[25]  = {
    "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
    "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
    "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
    "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
    "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};

/* Function prototypes */

void welcome();
void giveInstructions();
void setupBoard(Grid<char>& board);

// Recursive fuction that checks if word can be formed on board
bool canBeFormed(string input, Grid<char>& board, Grid<bool>& usedBoard, int i, int j) {
	if (input.length() == 0) return true;
	if (usedBoard[i][j]) return false;

	if (input[0] == board[i][j]) {
		usedBoard[i][j] = true;
		// checks all possible neighbour letters from 8 directions
		if (j + 1 <= board.numCols() - 1 && canBeFormed(input.substr(1), board, usedBoard, i, j + 1)) return true;
		if ((i + 1 <= board.numRows() - 1) && (j + 1 <= board.numCols() - 1) && canBeFormed(input.substr(1), board, usedBoard, i + 1, j + 1)) return true;
		if (i + 1 <= board.numRows() - 1 && canBeFormed(input.substr(1), board, usedBoard, i + 1, j)) return true;
		if ((i + 1 <= board.numRows() - 1) && (j - 1 >= 0) && canBeFormed(input.substr(1), board, usedBoard, i + 1, j - 1)) return true;
		if (j - 1 >= 0 && canBeFormed(input.substr(1), board, usedBoard, i, j - 1)) return true;
		if ((i - 1 >= 0) && (j - 1 >= 0) && canBeFormed(input.substr(1), board, usedBoard, i - 1, j - 1)) return true;
		if (i - 1 >= 0 && canBeFormed(input.substr(1), board, usedBoard, i - 1, j)) return true;
		if ((i - 1 >= 0) && (j + 1 <= board.numCols() - 1) && canBeFormed(input.substr(1), board, usedBoard, i - 1, j + 1)) return true;
		usedBoard[i][j] = false;
	}
		return false;
}


// Returns true if word can be formed on board and finds starting coordinates of first letter of input
bool checkBoard(string input, Grid<char>& board, Grid<bool>& usedBoard) {
	for (int i = 0; i < usedBoard.numRows(); i++) {
		for (int j = 0; j < usedBoard.numCols(); j++) {
			if (canBeFormed(input, board, usedBoard, i, j)) return true;
		}
	}
	return false;
}

// Checks if input word is valid or not
bool isValid(string input, Lexicon& words, Set<string>& used, Grid<char>& board, Grid<bool>& usedBoard) {

	return input.length() >= 4 && words.contains(input) && !used.contains(input) && checkBoard(input, board, usedBoard);
}

// Highligts cubes of found word temporarily
void highlight(Grid<bool>& usedBoard) { 
	for (int i = 0; i < usedBoard.numRows(); i++) {
		for (int j = 0; j < usedBoard.numCols(); j++) {
			highlightCube(i, j, usedBoard[i][j]);
			usedBoard[i][j] = false;
		}
	}
	pause(300);
	for (int i = 0; i < usedBoard.numRows(); i++) {
		for (int j = 0; j < usedBoard.numCols(); j++) {
			highlightCube(i, j, usedBoard[i][j]);
		}
	}
}

// player enters words and if it is valid word is recorded for player, added to used words Set and highlighted on board
void inputWord(Lexicon& words, Set<string>& used, Grid<char>& board, Grid<bool>& usedBoard) {
	while (true) {
		string input = getLine("Enter a word: ");
		if (input.length() == 0) break;
		input = toUpperCase(input);
		if (isValid(input, words, used, board, usedBoard)) {
			used += input;
			recordWordForPlayer(input, HUMAN);
			highlight(usedBoard);
		}
	}
}

// finds all words that can be formed on board
void findWord(Grid<char>& board, Grid<bool>& usedBoard, Lexicon& words, string current, 
										int i, int j, Vector<string>& result,  Set<string>& used) {
	if (current.length() >= 4 && words.contains(current) && !used.contains(current)) {
		result += current;
		used += current;
	}
	if ((usedBoard[i][j] || !words.containsPrefix(current)) || (int)current.length() > board.numCols() * board.numRows()) return;

		current += board[i][j];
		usedBoard[i][j] = true;
		
		// gets next letter of word from all possible directions
		if (j + 1 <= board.numCols() - 1) 
			findWord(board, usedBoard, words, current, i, j + 1, result, used);
		if ((i + 1 <= board.numRows() - 1) && (j + 1 <= board.numCols() - 1)) 
			findWord(board, usedBoard, words, current, i + 1, j + 1, result, used);
		if (i + 1 <= board.numRows() - 1) 
			findWord(board, usedBoard, words, current, i + 1, j, result, used);
		if ((i + 1 <= board.numRows() - 1) && (j - 1 >= 0)) 
			findWord(board, usedBoard, words, current, i + 1, j - 1, result, used);
		if (j - 1 >= 0) 
			findWord(board, usedBoard, words, current, i, j - 1, result, used);
		if ((i - 1 >= 0) && (j - 1 >= 0)) 
			findWord(board, usedBoard, words, current, i - 1, j - 1, result, used);
		if (i - 1 >= 0) 
			findWord(board, usedBoard, words, current, i - 1, j, result, used); 
		if ((i - 1 >= 0) && (j + 1 <= board.numCols() - 1)) 
			findWord(board, usedBoard, words, current, i - 1, j + 1, result, used);
		
		usedBoard[i][j] = false;
}

// gives coordinates of first letter of word to recursive function which finds all the words on board
// and records all found words for computer
void compTurn(Grid<char>& board, Lexicon& words, string current, Set<string>& used, Grid<bool>& usedBoard) {
	Vector<string> result;
	for (int i = 0; i < board.numRows(); i++) {
		for (int j = 0; j < board.numCols(); j++) {
			current = "";
			findWord(board, usedBoard, words, current, i, j, result, used);
		}
	}
	foreach(string word in result) {
		recordWordForPlayer(word, COMPUTER);
	}
}


string playAgain() {
	string answer;
	while (true) {
		cout << "Would you like to play again? ";
		answer = getLine();
		if (answer == "yes" || answer == "no") break;
	}
	return answer;
}


/* Main program */

int main() {
    GWindow gw(BOGGLE_WINDOW_WIDTH, BOGGLE_WINDOW_HEIGHT);
    initGBoggle(gw);
    welcome();
    giveInstructions();
	Lexicon words("EnglishWords.dat");
	while (true) {
		Grid<char> board;
		setupBoard(board);
		Set<string> used; // already found words are stored in this set
		Grid<bool> usedBoard(board.numRows(), board.numCols()); // before finding word on board all valuesof grid are false and 
                                                                 //after finding word coordinates of letters of words become true
		inputWord(words, used, board, usedBoard);
		compTurn(board, words, "", used, usedBoard);
		if (playAgain() == "no") break;
		initGBoggle(gw);
	}
	
    return 0;
}

/*
 * Function: welcome
 * Usage: welcome();
 * -----------------
 * Print out a cheery welcome message.
 */

void welcome() {
    cout << "Welcome!  You're about to play an intense game ";
    cout << "of mind-numbing Boggle.  The good news is that ";
    cout << "you might improve your vocabulary a bit.  The ";
    cout << "bad news is that you're probably going to lose ";
    cout << "miserably to this little dictionary-toting hunk ";
    cout << "of silicon.  If only YOU had a gig of RAM..." << endl << endl;
}

/*
 * Function: giveInstructions
 * Usage: giveInstructions();
 * --------------------------
 * Print out the instructions for the user.
 */

void giveInstructions() {
    cout << endl;
    cout << "The boggle board is a grid onto which I ";
    cout << "I will randomly distribute cubes. These ";
    cout << "6-sided cubes have letters rather than ";
    cout << "numbers on the faces, creating a grid of ";
    cout << "letters on which you try to form words. ";
    cout << "You go first, entering all the words you can ";
    cout << "find that are formed by tracing adjoining ";
    cout << "letters. Two letters adjoin if they are next ";
    cout << "to each other horizontally, vertically, or ";
    cout << "diagonally. A letter can only be used once ";
    cout << "in each word. Words must be at least four ";
    cout << "letters long and can be counted only once. ";
    cout << "You score points based on word length: a ";
    cout << "4-letter word is worth 1 point, 5-letters ";
    cout << "earn 2 points, and so on. After your puny ";
    cout << "brain is exhausted, I, the supercomputer, ";
    cout << "will find all the remaining words and double ";
    cout << "or triple your paltry score." << endl << endl;
//  cout << "Hit return when you're ready..." << endl;
//  getLine();
}

// player chooses 4x4 or 5x5 boggle
void chooseSize(Vector<string>& bigBoggleCubes, Vector<string>& standardCubes, bool& bigBoggle, Grid<char>& board) {
	cout << "You can choose standard Boggle (4x4 grid)" << endl;
		cout << "or Big Boggle (5x5)." << endl;
		cout << "Would you like Big Boggle? ";
		string answer;
		while (true) {
			answer = getLine();
			if (answer == "yes" || answer == "no") break;
			cout << "Please answer yes or no. Would you like Big Boggle? ";
		}
		if (answer == "yes") { // big boggle is chosen
			bigBoggle = true;
			for (int i = 0; i < bigBoggleCubes.size(); i++) { // randomly shuffle order of cubes in vector
				int r = randomInteger(i, bigBoggleCubes.size()-1);
				string s = bigBoggleCubes[i];
				bigBoggleCubes[i] = bigBoggleCubes[r];
				bigBoggleCubes[r] = s;
			}
			board.resize(5, 5);
			drawBoard(5, 5);
		} 

		if (answer == "no") { // small boggle is chosen
			for (int i = 0; i < standardCubes.size(); i++) { // randomly shuffle order of cubes in vector
				int r = randomInteger(i, standardCubes.size()-1);
				string s = standardCubes[i];
				standardCubes[i] = standardCubes[r];
				standardCubes[r] = s;
			}
			board.resize(4, 4);
			drawBoard(4, 4);
		} 
}

// player chooses to enter configuration or get a random one
void chooseConfiguration(Vector<string>& bigBoggleCubes, Vector<string>& standardCubes, bool& bigBoggle, Grid<char>& board) {
	cout << "Do you want to force the word configuration? ";
	string answer2;
	while (true) {
		answer2 = getLine();
		if (answer2 == "yes" || answer2 == "no") break;
		cout << "Please answer yes or no. Do you want to force the word configuration? ";
	}
	if (answer2 == "yes") { // player enters string which will be placed on cubes
		string conf;
		while (true) {
			conf = getLine("Enter your configuration: ");
			if (bigBoggle) {
				if ((int)conf.length() >= bigBoggleCubes.size()) break;
			} else {
				if ((int)conf.length() >= standardCubes.size()) break;
			}
		}
		int index = -1;
		// randomly chooses letters
		for (int i = 0; i < board.numRows(); i++) {
			for (int j = 0; j < board.numCols(); j++) {
				index++;
				string letter = "";
				letter += conf[index];
				letter = toUpperCase(letter);
				board[i][j] = letter[0];
				labelCube(i, j, letter[0]);
			}
		}
	} 

	// randomly choose letters for boggle
	if (answer2 == "no") {
		int index2 = -1;
		string cube;
		// randomly chooses letters
		for (int i = 0; i < board.numRows(); i++) {
			for (int j = 0; j < board.numCols(); j++) {
				index2++;
				if (bigBoggle){
					cube = bigBoggleCubes.get(index2);
				} else {
					cube = standardCubes.get(index2);
				}
				int r = randomInteger(0, cube.length() - 1);
				board[i][j] = cube[r];
				labelCube(i, j, cube[r]);
			}
		}
	}
}

// sets up board: graphical part and storing configurations in board grid
void setupBoard(Grid<char>& board) {
	Vector<string> bigBoggleCubes;
	bigBoggleCubes +=  "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
		"AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
		"CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
		"DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
		"FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU";

	Vector<string> standardCubes;
	standardCubes +=  "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
		"AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
		"DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
		"EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ";

	bool bigBoggle = false;
	chooseSize(bigBoggleCubes, standardCubes, bigBoggle, board);
	chooseConfiguration(bigBoggleCubes, standardCubes, bigBoggle, board);
}

