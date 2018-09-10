/* coinswap.cpp

Program #6, CoinSwap Undo.

Class: CS 141, Fall 2016.
System : Windows 10, Microsoft Visual Studio 2015
Lab : Tuesday 8am lab,
TA : Guixiang Ma
Author : Michal Bochnak

Make moves to solve the puzzle where the objective is to swap the place of
the pieces on either side of the board.  X can only move to the right into
an empty square, or can jump to the right over an O into an empty square.
Conversely O can only move to the left into an empty square, or can jump
to the left over an X into an empty square.
*/

#include <iostream>
using namespace std;


// Global constants
const int BoardSize = 5;


// Stores game information for each move
struct GameInfo {
	char board[BoardSize + 1];	// extra space for NULL
	int moveNumber;
	GameInfo *pNext;
};


// Functions declarations
void DisplayIdentifyingInformation();
void DisplayInstructions();
void AddNodeWithGameInfo(char board[], int moveNumber, GameInfo *&pHead, GameInfo *&pTemp);
void RemoveNodeWithGameInfo(char board[], GameInfo *&pHead);
void DisplayListOfMoves(GameInfo *pHead);
bool NotDone(char board[]);
void DisplayBoard(char board[]);
void PromptForAndGetMove(char board[], int &moveNumber, int &source, int &destination, GameInfo *&pHead);
bool MoveNotValid(char board[], int source, int destination);




//--------------------------------------------------------------------------------
int main()
{
	char board[BoardSize + 1] = "XX OO";	// extra character for the NULL
	int source, destination;				// source and destination spots
	int moveNumber = 1;						// stores move number
	GameInfo *pHead = NULL;					// pointer to the head of the list
	GameInfo *pTemp;						// temporary pointer

	// Display identifying information, the instructions, and the initial board
	DisplayIdentifyingInformation();
	DisplayInstructions();
	DisplayBoard(board);
	// Create a node with initial set up of the board
	AddNodeWithGameInfo(board, moveNumber, pHead, pTemp);
	// Display moves made so far
	DisplayListOfMoves(pHead);
	cout << endl;

	// Game play loop
	while (NotDone(board)) {
		PromptForAndGetMove(board, moveNumber, source, destination, pHead);
		// verify if move is valid
		if (MoveNotValid(board, source, destination)) {
			cout << "Invalid move, please retry. \n";
			continue;
		}

		// Make move.
		board[destination] = board[source];
		board[source] = ' ';
		moveNumber++;

		// prepend new node with game information
		AddNodeWithGameInfo(board, moveNumber, pHead, pTemp);
		// Display board and moves made so far
		DisplayBoard(board);
		DisplayListOfMoves(pHead);
		cout << endl;
	}

	// Display message about win
	cout << "\nCongratulations, you did it! \n"
		<< "\n"
		<< "Exiting program ...\n";
	return 0;         
}//end main()




//-----------------------------------------------------------------------------
// Functions Definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Display name and program information
void DisplayIdentifyingInformation()
{
	printf("\n");
	printf("Author: Michal Bochnak          \n");
	printf("Program: #6, Coin Swap     \n");
	printf("TA: Guixiang Ma, Tuesday 8am \n");
	printf("Nov 28, 2016               \n");
	printf("\n");
}//end displayIdentifyingInformation()


//-----------------------------------------------------------------------------
// Display instructions
void DisplayInstructions()
{
	cout << "Welcome to the coin swap puzzle.                                 \n"
		<< "Make moves to solve the puzzle where the objective is to swap the \n"
		<< "place of the pieces on either side of the board.  X can only move \n"
		<< "to the right into an empty square, or can jump to the right over  \n"
		<< "an O into an empty square. Conversely O can only move to the left \n"
		<< "into an empty square, or can jump to the left over an X into an   \n"
		<< "empty square.  \n"
		<< " \n"
		<< "For each move enter the source (1..5) and destination (1..5).     \n"
		<< "Enter x to exit the program. \n"
		<< "Enter u to undo last move. \n";
}//end displayInstructions()


//-----------------------------------------------------------------------------
// Adds new node to the linked list with game information after each valid move 
void AddNodeWithGameInfo(char board[], int moveNumber, GameInfo *&pHead, GameInfo *&pTemp) {
	pTemp = new GameInfo;				// Create New GameInfo
	strcpy(pTemp->board, board);		// Store actual board into new GameInfo
	pTemp->moveNumber = moveNumber;		// Save move number into new GameInfo
	pTemp->pNext = pHead;				// Set the pointer of new GameInfo to pHead
	pHead = pTemp;						// Update head of the list
}


//-----------------------------------------------------------------------------
// Removes the last node with game informations when user want to undo a move
void RemoveNodeWithGameInfo(char board[], GameInfo *&pHead) {
	GameInfo *pTemp = pHead;		// assign pHead to pTemp for safety
	pHead = pHead->pNext;			// forward pHead to the next GameInfo
	free(pTemp);					// delete front GameInfo
}


//-----------------------------------------------------------------------------
// Displays the list of moves
void DisplayListOfMoves(GameInfo *pHead) {
	GameInfo *pTemp = pHead;			// copy head to avoid problems
	cout << "List: ";
	while (pTemp != NULL) {			
		cout << pTemp->moveNumber;		// display moveNumber
		if ((pTemp->moveNumber) > 1) {	
			cout << "->";				// add arrow after displaying each moveNumber 
		}
		pTemp = pTemp->pNext;			// forward pointer to next GameInfo
	}
	cout << endl;						// new line for output formatting
}


//-----------------------------------------------------------------------------
// See if board pieces have finished being swapped.  This is the case when
// the left-most two pieces are both 'O' and the right-most two pieces are 'X'
bool NotDone(char board[]) {
	return board[0] != 'O' || board[1] != 'O' || board[3] != 'X' || board[4] != 'X';
}


//-----------------------------------------------------------------------------
// Display the board
void DisplayBoard(char board[]) {
	cout << endl;						// new line for formatting
	cout << "   1 2 3 4 5\n";			// board positions
	cout << "   ";						// spaces for formatting
	for (int i = 0; i<BoardSize; i++) {	// display the board inserting spaces between elements
		cout << board[i] << " ";
	}
	cout << "     ";					// spaces for formatting
}


//-----------------------------------------------------------------------------
// Prompt user to make a move, undoing the move, upgrading GameInfo Linked List
void PromptForAndGetMove(char board[], int &moveNumber, int &source, int &destination, GameInfo *&pHead) {
	char userInput;

	// Infinite loop to handle possible multiple undo of moves
	while (true) {
		cout << moveNumber << ". Enter source and destination: ";
		cin >> userInput;
		// See if user input of x was given to exit the program
		if (userInput == 'x') {
			cout << "\n";
			cout << "Exiting program...\n";
			exit(0);
		}
		// See if user input of u was given to undo the move
		if (userInput == 'u') {
			// prevent from undoing past beginning of game
			if (moveNumber == 1) {
				cout << "*** You cannot undo past the beginning of the game. Please retry." << endl;
				continue;
			}
			cout << "* Undoing move *" << endl;
			RemoveNodeWithGameInfo(board, pHead);
			strcpy(board, pHead->board);		// update the board with data from previous move
			DisplayBoard(board);
			moveNumber--;						// decrease moveNumber
			DisplayListOfMoves(pHead);
			cout << endl;						// new line for formatting
			continue;							// Prompt to retry move	
		}
		// Input is likely numeric and is not 'x' or 'u'.  Convert to a number.
		source = userInput - '0';

		// Also get destination portion of user input
		cin >> destination;

		// Adjust user entry to compensate for 0-based indexing, rather than 1-based
		source--;
		destination--;

		// break out of enclosing loop, as we don't need to undo a move
		break;
	}
}//end promptForAndGetMove()


 //----------------------------------------------------------------------------
 // Validates each move and display appropriate error message if necessary
bool MoveNotValid(
	char board[],
	int source,
	int destination) {

	// Validate spot choice
	if ((source + 1 < 1) || (source + 1 > 5) || (destination + 1 < 1) || (destination + 1 > 5)) {
		cout << "*** You can't refer to a position off the board. ";
		return true;
	}
	// Validate if direction of moved piece is correct 
	if (((board[source] == 'X') && (source > destination)) || ((board[source] == 'O') && (source < destination))) {
		cout << "*** You can't move that piece that direction. ";
		return true;
	}
	// Validate if destination spot is empty
	if (board[destination] != ' ') {
		cout << "*** Destination square is not empty. ";
		return true;
	}
	// Validate if destination is not to far away
	if (((destination - source) > 2) || ((destination - source) < -2)) {
		cout << "*** Destination is too far away. ";
		return true;
	}
	// Validate if jumped squere is opponent
	if ((board[source] == 'X') && (board[source + 1] == 'X')) {
		cout << "*** A jumped square must have an opponent. ";
		return true;
	}
	// Validate if jumped squere is opponent
	if ((board[source] == 'O') && (board[source - 1] == 'O')) {
		cout << "*** A jumped square must have an opponent. ";
		return true;
	}

	return false;	// move is valid
}

// end of program