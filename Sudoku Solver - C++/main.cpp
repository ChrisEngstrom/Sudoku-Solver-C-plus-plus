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

// Printing functions
void printGameBoard(int gameBoardArray[][SUDOKU_SIZE]);
void printGameBoard(int gameBoardArray[][SUDOKU_SIZE], ofstream &outputFile);
void printNumberCount(int numberCountArray[], int &numsRemaining);
void printPossibleNumbers(bool possibleNumsArray[][SUDOKU_SIZE]);

// Solving functions
void eliminateKnownNumbers(int gameBoardArray[][SUDOKU_SIZE], bool possibleNumsArray[][SUDOKU_SIZE]);
void clearSolvedNumFromRow(bool possibleNumsArray[][SUDOKU_SIZE], int currentRow, int currentCell, int cellValue);
void clearSolvedNumFromColumn(bool possibleNumsArray[][SUDOKU_SIZE], int currentColumn, int currentCell, int cellValue);
void clearSolvedNumFromBlock(bool possibleNumsArray[][SUDOKU_SIZE], int currentRow, int currentColumn, int currentCell, int cellValue);
void findNextNumber(int gameBoardArray[][SUDOKU_SIZE], int numberCountArray[], int &numsRemaining);

// Maintenance functions
int markCellsAsSolved(int gameBoardArray[][SUDOKU_SIZE], bool possibleNumsArray[][SUDOKU_SIZE], int numberCountArray[]);

int main ()
{
	char buffer[BUFFER_SIZE];									// Char array to store current line from the input file
	int cellsRemaining = TOTAL_CELLS,							// Total number of cells remaining that need to be solved											
		gameBoard[SUDOKU_SIZE][SUDOKU_SIZE],					// Array to hold the current game board contents
		numberCount[SUDOKU_SIZE] = {0},							// Array to hold the count of each individual number solved in the puzzle
		numberOfCellsSolved = 0,
		passesCount = 0;										// Holds the number of passes the program takes to solve the sudoku (for informational purposes only)
	bool possibleNumbers[TOTAL_CELLS][SUDOKU_SIZE];				// Parallel array to gameBoard with possible numbers as booleans: true = still a possibility

	// Initialize possibleNumbers[][] to true to show all the numbers are still possible in all the cells
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
						// Increase the number count for the number being placed on the board
						numberCount[currentNumber - 1]++;

						// Traverse through the possible numbers list 
						for(int i = 0; i < SUDOKU_SIZE; i++)
						{
							// Mark all numbers and no longer possible except for
							//	the number that is being placed on the board
							if(i != currentNumber - 1)
							{
								possibleNumbers[cellIndex][i] = false;
							}
						}

						cellsRemaining--;
					}

					// Place the current number on the board
					gameBoard[rowCount][columnIndex] = currentNumber;

					// Increased the indexes
					columnIndex++;
					cellIndex++;
					bufferIndex++;
				}
			}

			// Get the next line in the text file
			inFile.getline(buffer, BUFFER_SIZE, '\n');

			// Increase the row count to the next row
			rowCount++;
		}

		// Close the input file
		inFile.close();
	}

	// Print the current state
	printGameBoard(gameBoard);
	cout << endl;
	printNumberCount(numberCount, cellsRemaining);

	// Solve cells until either all are solved or until using the basic methods can't solve any more
	do 
	{
		// Set this pass' number of cells solved back to 0
		numberOfCellsSolved = 0;

		// Increase the number of passes count
		passesCount++;

		// Remove numbers no longer possible in all the game board cells
		eliminateKnownNumbers(gameBoard, possibleNumbers);

		// Check the possibleNumbers and place solved numbers on the gameBoard while counting
		//	how many have been placed. Return that number to keep track of whether any numbers
		//	had been solved or not.
		numberOfCellsSolved = markCellsAsSolved(gameBoard, possibleNumbers, numberCount);

		// Decrease the count of cells remaining by how many cells were solved this pass
		cellsRemaining -= numberOfCellsSolved;

		//findNextNumber(gameBoard, numberCount, cellsRemaining);

		// Print the current state
		printGameBoard(gameBoard);

		cout << endl 
			 << "Pass #: " << passesCount << endl << endl
			 << "Cells solved this pass: " << numberOfCellsSolved
			 << endl << endl;

		printNumberCount(numberCount, cellsRemaining);
		//printPossibleNumbers(possibleNumbers);
	} while ((numberOfCellsSolved > 0) && (cellsRemaining > 0));

	// If there are any cells remaining, let us know we failed
	if(cellsRemaining > 0)
	{
		cout << "Not all of the cells were solved using the current methods." << endl;
	}

	// Print the final state of the board to the output file
	printGameBoard(gameBoard, outFile);

	// Close the output file
	outFile.close();

	return 0;
}

// Print out the contents of the gameBoard array in the form of a sudoku puzzle to the console
void printGameBoard(int gameBoardArray[][SUDOKU_SIZE])
{
	// Traverse the whole game board
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

// Print out the contents of the gameBoard array in the form of a sudoku puzzle to an output file
void printGameBoard(int gameBoardArray[][SUDOKU_SIZE], ofstream &outputFile)
{
	// Traverse the whole game board
	for(int row = 0; row < SUDOKU_SIZE; row++)
	{
		for(int column = 0; column < SUDOKU_SIZE; column++)
		{
			// Output the cell number
			outputFile << gameBoardArray[row][column];

			// Space the numbers out so the board is easier to read
			if(column != (SUDOKU_SIZE - 1))
			{
				outputFile << ' ';
			}

			// Separate the three vertical sections
			if(!((column + 1) % 3) && column != (SUDOKU_SIZE - 1))
			{
				outputFile << "| ";
			}
		}

		// Separate the three horizontal sections
		if(!((row + 1) % 3) && row != (SUDOKU_SIZE - 1))
		{
			outputFile << endl << "---------------------";
		}

		outputFile << endl;
	}
}

// Display the number counts
void printNumberCount(int numberCountArray[], int &numsRemaining)
{
	// Display the possible numbers and a separator
	cout << " 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9" << endl
		 << "----------------------------------" << endl;

	// Traverse the number count array
	for(int number = 0; number < SUDOKU_SIZE; number++)
	{
		// Display the number
		cout  << ' ' << numberCountArray[number];

		// Display a separator after each number but the last
		if(number != (SUDOKU_SIZE - 1))
		{
			cout << " |";
		}
	}

	// Display some total statistics
	cout << endl << endl
		 <<"Numbers found: " << (TOTAL_CELLS - numsRemaining) << endl
		 << "Numbers remaining: " << numsRemaining
		 << endl << endl << endl << endl;
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

	// Traverse the whole game board
	for(int row = 0; row < SUDOKU_SIZE; row++)
	{
		for(int column = 0; column < SUDOKU_SIZE; column++)
		{
			// Find all the cells that are solved
			if(gameBoardArray[row][column] != 0)
			{
				// Get the number of the cell from the game board
				int cellValue = gameBoardArray[row][column];
				
				// Remove number from current row
				clearSolvedNumFromRow(possibleNumsArray, row, currentCell, cellValue);

				// Remove number from current column
				clearSolvedNumFromColumn(possibleNumsArray, column, currentCell, cellValue);

				// Remove number from current 3x3 block
				clearSolvedNumFromBlock(possibleNumsArray, row, column, currentCell, cellValue);
			}

			currentCell++;
		}
	}
}

// Remove a solved number as a possible number from the current row
void clearSolvedNumFromRow(bool possibleNumsArray[][SUDOKU_SIZE], int currentRow, int currentCell, int cellValue)
{
	// Calculate and store the row that the current cell is in
	int currentRowPosition = currentRow * SUDOKU_SIZE;

	// Traverse the possibleNums Array
	for(int i = 0; i < SUDOKU_SIZE; i++)
	{
		// Go to every cell that isn't the current cell
		if(currentRowPosition != currentCell)
		{
			// Remove the number as a possibility for that cell
			possibleNumsArray[currentRowPosition][cellValue - 1] = false;
		}

		currentRowPosition++;
	}
}

// Remove a solved number as a possible number from the current column
void clearSolvedNumFromColumn(bool possibleNumsArray[][SUDOKU_SIZE], int currentColumn, int currentCell, int cellValue)
{
	// Calculate and store the column that the current cell is in
	int currentColumnPosition = currentColumn % SUDOKU_SIZE;

	// Traverse the possibleNums Array
	for(int i = 0; i < SUDOKU_SIZE; i++)
	{
		// Go to every cell that isn't the current cell
		if(currentColumnPosition != currentCell)
		{
			// Remove the number as a possibility for that cell
			possibleNumsArray[currentColumnPosition][cellValue - 1] = false;
		}

		currentColumnPosition += 9;
	}
}

// Remove a solved number as a possible number from the current 3x3 block
void clearSolvedNumFromBlock(bool possibleNumsArray[][SUDOKU_SIZE], int currentRow, int currentColumn, int currentCell, int cellValue)
{
	// Calculate the highest row in the current 3x3 block
	int blockTopRow = currentRow - (currentRow % 3);
	// Calculate the leftmost column in the current 3x3 block
	int blockLeftColumn = currentColumn - (currentColumn % 3);

	// Traverse the whole 3x3 block
	for(int i = blockTopRow; i < (blockTopRow + 3); i++)
	{
		for(int j = blockLeftColumn; j < (blockLeftColumn + 3); j++)
		{
			// Go to every cell that isn't the current cell
			if(((SUDOKU_SIZE * i) + j) != currentCell)
			{
				// Remove the number as a possibility for that cell
				possibleNumsArray[((SUDOKU_SIZE * i) + j)][cellValue - 1] = false;
			}
		}
	}
}

int markCellsAsSolved(int gameBoardArray[][SUDOKU_SIZE], bool possibleNumsArray[][SUDOKU_SIZE], int numberCountArray[])
{
	int cellsSolved = 0,
		currentPosition = 0,
		possibleNumCount,
		solvedCellValue = 0;

	// Traverse the whole game board
	for(int row = 0; row < SUDOKU_SIZE; row++)
	{
		for(int column = 0; column < SUDOKU_SIZE; column++)
		{
			// Only concentrate on the cells that are currently unsolved
			if(gameBoardArray[row][column] == 0)
			{
				possibleNumCount = 0;

				// Get the number of the current position
				currentPosition = ((SUDOKU_SIZE * row) + column);

				// Traverse through each bool in possibleNumsArray for that cell position
				int i = 0;
				while((possibleNumCount < 2) && (i < SUDOKU_SIZE))
				{
					if(possibleNumsArray[currentPosition][i] == true)
					{
						solvedCellValue = i + 1;
						possibleNumCount++;
					}

					i++;
				}

				// If there is only one value remaining, that is the solution to the cell
				if(possibleNumCount == 1)
				{
					gameBoardArray[row][column] = solvedCellValue;

					numberCountArray[solvedCellValue - 1]++;

					cellsSolved++;
				}
			}
		}
	}

	return cellsSolved;
}

// Find the next number in the puzzle
void findNextNumber(int gameBoardArray[][SUDOKU_SIZE], int numberCountArray[], int &numsRemaining)
{


	numsRemaining--;
}