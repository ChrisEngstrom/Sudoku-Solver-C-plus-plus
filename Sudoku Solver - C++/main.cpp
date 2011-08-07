///////////////////////////
///	  Chris Engstrom	///
///	 Sudoku Solver C++	///
///////////////////////////

#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

int main () {

	const int BUFFER_SIZE = 19;
	char buffer[BUFFER_SIZE];
	int rowCount = 0,
		gameBoard[9][9],
		numberCount[9] = {0};

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

			cout << buffer << endl;

			while(buffer[bufferIndex] != ';')
			{
				// Skip spaces
				while(buffer[bufferIndex] == ' ' && buffer[bufferIndex] != ';')
				{
					bufferIndex++;
				}

				if(buffer[bufferIndex] != ';')
				{
					int currentNumber = buffer[bufferIndex] - '0';

					if(currentNumber != 0)
					{
						numberCount[currentNumber - 1]++;
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

		cout << endl << endl;

		// Print out the contents of the gameBoard array in the form of a sudoku puzzle
		for(int row = 0; row < 9; row++)
		{
			for(int column = 0; column < 9; column++)
			{
				// Output the cell number
				cout << gameBoard[row][column];

				// Space the numbers out so the board is easier to read
				if(column != 8)
				{
					cout << ' ';
				}

				// Separate the three vertical sections
				if(!((column + 1) % 3) && column != 8)
				{
					cout << "| ";
				}
			}

			// Separate the three horizontal sections
			if(!((row + 1) % 3) && row != 8)
			{
				cout << endl << "---------------------";
			}

			cout << endl;
		}

		// Display the number counts
		cout << endl << endl
			 << " 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9" << endl
			 << "----------------------------------" << endl;

		for(int number = 0; number < 9; number++)
		{
			cout  << ' ' << numberCount[number];

			if(number != 8)
			{
				cout << " |";
			}
		}

		cout << endl;
		
		// Close the input file
		inFile.close();
	}

	// Close the output file
	outFile.close();

	return 0;
}