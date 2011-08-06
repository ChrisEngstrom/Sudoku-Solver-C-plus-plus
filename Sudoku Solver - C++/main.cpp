///////////////////////////
///	  Chris Engstrom	///
///	 Sudoku Solver C++	///
///////////////////////////

#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

int main () {

	char buffer[81];

	ifstream inFile("puzzleInput.txt");
	ofstream outFile;
	outFile.open("puzzleOutput.txt");

	// Read first line of the inFile
	inFile.getline(buffer, 80, '\n');

	if(!inFile)
		cout << "Cannot locate file." << endl;
	else
	{
		while(!inFile.eof())
		{
			outFile << buffer << endl;

			inFile.getline(buffer, 80, '\n');
		}

	}

	cout << "This will fun!" << endl;

	return 0;
}