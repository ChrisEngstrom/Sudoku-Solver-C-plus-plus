///////////////////////////
///	  Chris Engstrom	///
///	 Sudoku Solver C++	///
///////////////////////////

#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

int main () {

	char buffer[18],
		 gameBoard[9][9];
	int rowCount = 0;

	ifstream inFile;
	inFile.open("sudokuInput.txt");
	ofstream outFile;
	outFile.open("sudokuOutput.txt", ios::out | ios::trunc);

	// Read first line of the inFile
	inFile.getline(buffer, 17, '\n');

	if(!inFile)
		cout << "Cannot locate file." << endl;
	else
	{
		while(!inFile.eof())
		{
			int bufferIndex = 0,
				columnIndex = 0;

			cout << buffer << endl;

			while(buffer[bufferIndex] != '\n')
			{
				//***************
				//* skip spaces *
				//***************
				while(buffer[bufferIndex] == ' ' && buffer[bufferIndex] != '\n')
				{
					bufferIndex++;
				}

				if(buffer[bufferIndex] != '\n')
				{
					gameBoard[rowCount][columnIndex] = buffer[bufferIndex];
					columnIndex++;
					bufferIndex++;
				}
			}

			outFile << buffer << endl;

			inFile.getline(buffer, 17, '\n');
			rowCount++;
		}

		cout << endl << endl;

		// Print out the contents of the gameBoard array in the form of a sudoku puzzle
		for(int row = 0; row < 9; row++)
		{
			for(int column = 0; column < 9; column++)
			{
				cout << gameBoard[row][column];

				if(column != 8)
				{
					cout << ' ';
				}

				if(!((column + 1) % 3) && column != 8)
				{
					cout << "| ";
				}
			}

			if(!((row + 1) % 3) && row != 8)
			{
				cout << endl << "---------------------";
			}

			cout << endl;
		}
		

	}

	return 0;
}