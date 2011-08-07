///////////////////////////
///	  Chris Engstrom	///
///	 Sudoku Solver C++	///
///////////////////////////

#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

const int SUDOKU_SIZE = 9;

void printGameBoard(int gameBoardArray[][SUDOKU_SIZE]);
void printNumberCount(int numberCountArray[], int &numsRemaining);
void findNextNumber(int gameBoardArray[][SUDOKU_SIZE], int numberCountArray[], int &numsRemaining);

int main ()
{
	const int BUFFER_SIZE = 19;
	char buffer[BUFFER_SIZE];
	int numbersRemaining = 81,
		rowCount = 0,
		gameBoard[SUDOKU_SIZE][SUDOKU_SIZE],
		numberCount[SUDOKU_SIZE] = {0};

	ifstream inFile("sudokuInput.txt");

	ofstream outFile("sudokuOutput.txt", ios::trunc);

	if(!inFile)
		cout << "Cannot locate file." << endl;
	else
	{
		// Read first line of the inFile
		inFile.getline(buffer, BUFFER_SIZE, '\n');

		while(!inFile.eof())
		{
			int bufferIndex = 0,
				columnIndex = 0;

			// Display this line of the original input
			cout << buffer << endl;

			// Process until we hit the end of the current line
			while(buffer[bufferIndex] != ';')
			{
				// Skip extraneous spaces
				while(buffer[bufferIndex] == ' ' && buffer[bufferIndex] != ';')
				{
					bufferIndex++;
				}

				// Make sure the current character is a number
				if(buffer[bufferIndex] != ';')
				{
					// Get integer value of the current character
					int currentNumber = buffer[bufferIndex] - '0';

					// Only add to numberCount if the number is not 0
					if(currentNumber != 0)
					{
						numberCount[currentNumber - 1]++;

						numbersRemaining--;
					}

					gameBoard[rowCount][columnIndex] = currentNumber;
					columnIndex++;
					bufferIndex++;
				}
			}

			outFile << buffer << endl;

			inFile.getline(buffer, BUFFER_SIZE, '\n');
			rowCount++;
		}
		
		// Close the input file
		inFile.close();
	}

	// Solve the puzzle until no numbers remain to be found
	while(numbersRemaining != 0)
	{
		findNextNumber(gameBoard, numberCount, numbersRemaining);

		// Print the current state
		printGameBoard(gameBoard);
		printNumberCount(numberCount, numbersRemaining);
	}

	// Close the output file
	outFile.close();

	return 0;
}

// Print out the contents of the gameBoard array in the form of a sudoku puzzle
void printGameBoard(int gameBoardArray[][SUDOKU_SIZE])
{
	cout << endl << endl;
	for(int row = 0; row < SUDOKU_SIZE; row++)
	{
		for(int column = 0; column < SUDOKU_SIZE; column++)
		{
			// Output the cell number
			cout << gameBoardArray[row][column];

			// Space the numbers out so the board is easier to read
			if(column != (SUDOKU_SIZE - 1))
			{
				cout << ' ';
			}

			// Separate the three vertical sections
			if(!((column + 1) % 3) && column != (SUDOKU_SIZE - 1))
			{
				cout << "| ";
			}
		}

		// Separate the three horizontal sections
		if(!((row + 1) % 3) && row != (SUDOKU_SIZE - 1))
		{
			cout << endl << "---------------------";
		}

		cout << endl;
	}
}

// Display the number counts
void printNumberCount(int numberCountArray[], int &numsRemaining)
{
	cout << endl << endl
			<< " 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9" << endl
			<< "----------------------------------" << endl;

	for(int number = 0; number < SUDOKU_SIZE; number++)
	{
		cout  << ' ' << numberCountArray[number];

		if(number != (SUDOKU_SIZE - 1))
		{
			cout << " |";
		}
	}

	cout << endl << endl
		 << "Numbers found: " << (81 - numsRemaining) << endl
		 << "Numbers remaining: " << numsRemaining << endl;
}

// Find the next number in the puzzle
void findNextNumber(int gameBoardArray[][SUDOKU_SIZE], int numberCountArray[], int &numsRemaining)
{


	numsRemaining--;
}