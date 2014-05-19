#include <iostream>
#include <fstream>
#include <string>

#include "pgn.h"

using namespace std;

int main(int argc, char* argv[]) {
	string filename,temp;
	ifstream pgnfile;
	Match newgame;

	cout << "Enter .PGN file name: ";
	cin >> filename;

	pgnfile.open(filename + ".pgn", ios::in);
	if (pgnfile.fail()){
		cout << filename << ".pgn could not be opened." << endl;
		exit(1);
	}
	else {
		cout << "Success" << endl;
		system("pause");
		while (getline(pgnfile, temp)) {
			cout << temp << endl;
		}
		pgnfile.close();
	}
	system("pause");
	return 0;
}