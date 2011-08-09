///////////////////////////
///   Chris Engstrom    ///
///  Sudoku Solver C++  ///
///////////////////////////

#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

// Constants
const int BUFFER_SIZE = 81;
const int SUDOKU_SIZE = 9;
const int TOTAL_CELLS = 81;

void printGameBoard(int gameBoardArray[][SUDOKU_SIZE]);
void printNumberCount(int numberCountArray[], int &numsRemaining);
void printPossibleNumbers(bool possibleNumsArray[][SUDOKU_SIZE]);

void eliminateKnownNumbers(int gameBoardArray[][SUDOKU_SIZE], bool possibleNumsArray[][SUDOKU_SIZE]);
void clearSolvedNumFromRow(bool possibleNumsArray[][SUDOKU_SIZE], int currentRow, int currentCell, int cellValue);
void clearSolvedNumFromColumn(bool possibleNumsArray[][SUDOKU_SIZE], int currentColumn, int currentCell, int cellValue);
void findNextNumber(int gameBoardArray[][SUDOKU_SIZE], int numberCountArray[], int &numsRemaining);

int main ()
{
	char buffer[BUFFER_SIZE];									// Char array to store current line from the input file
	int cellsRemaining = TOTAL_CELLS,							// Total number of cells remaining that need to be solved											
		gameBoard[SUDOKU_SIZE][SUDOKU_SIZE],
		numberCount[SUDOKU_SIZE] = {0};
	bool possibleNumbers[TOTAL_CELLS][SUDOKU_SIZE];				// Parallel array to gameBoard with possible numbers as booleans

	for(int i = 0; i < TOTAL_CELLS; i++)
	{
		for(int j = 0; j < SUDOKU_SIZE; j++)
		{
			possibleNumbers[i][j] = true;
		}
	}

	ifstream inFile("sudokuInput.txt");

	ofstream outFile("sudokuOutput.txt", ios::trunc);

	if(!inFile)
		cout << "Cannot locate file." << endl;
	else
	{
		// Read first line of the inFile
		inFile.getline(buffer, BUFFER_SIZE, '\n');

		int cellIndex = 0,
			rowCount = 0;

		while(!inFile.eof())
		{
			int bufferIndex = 0,
				columnIndex = 0;

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

						for(int i = 0; i < SUDOKU_SIZE; i++)
						{
							if(i != currentNumber - 1)
							{
								possibleNumbers[cellIndex][i] = false;
							}
						}

						cellsRemaining--;
					}

					gameBoard[rowCount][columnIndex] = currentNumber;
					columnIndex++;
					cellIndex++;
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
	//while(numbersRemaining != 0)
	//{
		eliminateKnownNumbers(gameBoard, possibleNumbers);

		findNextNumber(gameBoard, numberCount, cellsRemaining);

		// Print the current state
		printGameBoard(gameBoard);
		printNumberCount(numberCount, cellsRemaining);
		printPossibleNumbers(possibleNumbers);
	//}

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
		 << "Numbers found: " << (TOTAL_CELLS - numsRemaining) << endl
		 << "Numbers remaining: " << numsRemaining << endl;
}

void printPossibleNumbers(bool possibleNumsArray[][SUDOKU_SIZE])
{
	cout << endl;

	// Go through each cell of the sudoku
	for(int cell = 0; cell < TOTAL_CELLS; cell++)
	{
		if((cell + 1) < 10)
		{
			cout << " ";
		}

		cout << (cell + 1) << ": ";

		// Go through each number possibility for the current cell
		for(int possibleNumber = 0; possibleNumber < SUDOKU_SIZE; possibleNumber++)
		{
			// Check to see if the current number is still a possibility
			if(possibleNumsArray[cell][possibleNumber])
			{
				cout << (possibleNumber + 1) << ' ';
			}
		}

		cout << endl;
	}
}

// Remove known numbers from their corresponding row and column
void eliminateKnownNumbers(int gameBoardArray[][SUDOKU_SIZE], bool possibleNumsArray[][SUDOKU_SIZE])
{
	int currentCell = 0;	// Keep track of the current cell to use in the possibleNumsArray

	// Go through the whole game board
	for(int row = 0; row < SUDOKU_SIZE; row++)
	{
		for(int column = 0; column < SUDOKU_SIZE; column++)
		{
			// Find all the cells that are solved
			if(gameBoardArray[row][column] != 0)
			{
				int cellValue = gameBoardArray[row][column];
				
				// Remove number from current row
				clearSolvedNumFromRow(possibleNumsArray, row, currentCell, cellValue);

				// Remove number from current column
				clearSolvedNumFromColumn(possibleNumsArray, column, currentCell, cellValue);

				// Remove this number as a possible number from the current 3x3 block
				int blockTopRow = row - (row % 3);
				int blockLeftColumn = column - (column % 3);
				for(int i = blockTopRow; i < (blockTopRow + 3); i++)
				{
					for(int j = blockLeftColumn; j < (blockLeftColumn + 3); j++)
					{


						if(((SUDOKU_SIZE * i) + j) != currentCell)
						{
							possibleNumsArray[((SUDOKU_SIZE * i) + j)][cellValue - 1] = false;
						}
					}
				}
			}

			currentCell++;
		}
	}
}

// Remove a solved number as a possible number from the current row
void clearSolvedNumFromRow(bool possibleNumsArray[][SUDOKU_SIZE], int currentRow, int currentCell, int cellValue)
{
	int currentRowPosition = currentRow * SUDOKU_SIZE;

	for(int i = 0; i < SUDOKU_SIZE; i++)
	{
		if(currentRowPosition != currentCell)
		{
			possibleNumsArray[currentRowPosition][cellValue - 1] = false;
		}

		currentRowPosition++;
	}
}

// Remove a solved number as a possible number from the current column
void clearSolvedNumFromColumn(bool possibleNumsArray[][SUDOKU_SIZE], int currentColumn, int currentCell, int cellValue)
{
	int currentColumnPosition = currentColumn % SUDOKU_SIZE;

	for(int i = 0; i < SUDOKU_SIZE; i++)
	{
		if(currentColumnPosition != currentCell)
		{
			possibleNumsArray[currentColumnPosition][cellValue - 1] = false;
		}

		currentColumnPosition += 9;
	}
}

// Find the next number in the puzzle
void findNextNumber(int gameBoardArray[][SUDOKU_SIZE], int numberCountArray[], int &numsRemaining)
{


	numsRemaining--;
}