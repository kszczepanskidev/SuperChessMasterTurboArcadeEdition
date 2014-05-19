#include <iostream>
#include <fstream>
#include <string>

#include "pgn.h"

using namespace std;

int main(int argc, char* argv[]) {
	string filename,temp;
	ifstream pgnfile;
	Match *newGame = new Match();

	cout << "Enter .PGN file name: ";
	cin >> filename;

	pgnfile.open(filename + ".pgn", ios::in);
	if (pgnfile.fail()){
		cout << filename << ".pgn could not be opened." << endl;
		exit(1);
	}
	else {
		while (getline(pgnfile, temp) && !temp.empty()) {
			read(temp, newGame);
		}
		pgnfile.close();
	}
	write(newGame);
	system("pause");
	return 0;
}


void read(string in, Match* game) {
	string type,value;

	type = in.substr(1, in.find(char(32)) - 1);
	value = in.substr(in.find(char(34)) + 1, (in.length() - in.find(char(34)) - 3));

	if (type == "Event") game->name = value;
	if (type == "Site") game->place = value;
	if (type == "Date") game->date = value;
	if (type == "Round") game->round = value;
	if (type == "White") game->whitePlayer = value;
	if (type == "Black") game->blackPlayer = value;
	if (type == "Result") game->result = value;
	if (type == "ECO") game->eco = value;
	if (type == "WhiteElo") game->whiteElo = value;
	if (type == "BlackElo") game->blackElo = value;
	if (type == "Annotator") game->annotator = value;
}

void write(Match* game) {
	cout << endl << endl;
	if (!game->name.empty())
		cout << "Event name: " << game->name << endl;
	if (!game->place.empty())
		cout << "Event place: " << game->place << endl;
	if (!game->date.empty())
		cout << "Event date: " << game->date << endl;
	if (!game->round.empty())
		cout << "Round number: " << game->round << endl;
	if (!game->whitePlayer.empty())
		cout << "White player's name: " << game->whitePlayer << endl;
	if (!game->blackPlayer.empty())
		cout << "Black player's name: " << game->blackPlayer << endl;
	if (!game->result.empty())
		cout << "Game result: " << game->result << endl;
	if (!game->eco.empty())
		cout << "Kod debiutowy: " << game->eco << endl;
	if (!game->whiteElo.empty())
		cout << "White pawns's player's Elo: " << game->whiteElo << endl;
	if (!game->blackElo.empty())
		cout << "Black pawns's player's Elo: " << game->blackElo << endl;
	if (!game->annotator.empty()) 
		cout << "Annotator's name: " << game->annotator << endl;
}