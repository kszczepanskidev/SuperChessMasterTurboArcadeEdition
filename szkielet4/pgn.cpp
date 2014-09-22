#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

#include "pgn.h"

using namespace std;

void readBasicInfo(string in, Match* game) {
	string type, value;

	type = in.substr(1, in.find(char(32)) - 1);
	value = in.substr(in.find(char(34)) + 1, (in.length() - in.find(char(34)) - 3));

	if (type == "Event")	game->name = value;
	if (type == "Site")		game->place = value;
	if (type == "Date")		game->date = value;
	if (type == "Round")	game->round = value;
	if (type == "White")	game->whitePlayer = value;
	if (type == "Black")	game->blackPlayer = value;
	if (type == "Result")	game->result = value;
	if (type == "ECO")		game->eco = value;
	if (type == "WhiteElo") game->whiteElo = value;
	if (type == "BlackElo") game->blackElo = value;
	if (type == "Annotator")game->annotator = value;
}


void readMoves(string line, Match* game) {
	string temp;
	stringstream ss(line);

	while (ss >> temp) {
		if (temp.find('.') != -1)
			continue;
		else
			game->moves.push_back(temp);
	}

}


void Match::write(Match* game) {
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

	/*if (!game->moves.empty()) {
		cout << endl << "Moves:" << endl;
		for (string move : game->moves)
			cout << move << endl;
	}*/
	for (unsigned int i = 0; i < game->moves.size(); i++)
		cout << game->moves[i] << "\t" << game->postMoves[i].col << "-" << game->postMoves[i].row << endl;
}


void parseMoves(Match* game) {
	Move temp;

	for (unsigned int m = 0; m < game->moves.size(); m++) {

	temp.col = temp.row = 0;
	temp.pcol = -1;
	temp.check = temp.castling = 0;
	temp.promote = temp.capture = false;


		if (game->moves[m] == "O-O")
			temp.castling = 1;				//short castling
		if (game->moves[m] == "O-O-O")
			temp.castling = 2;				//queen castling
		if (game->moves[m].find("+") != string::npos)
			temp.check = 1;					//check
		if (game->moves[m].find("#") != string::npos)
			temp.check = 2;					//checkmate		
		if (game->moves[m].find("=") != string::npos) {
			temp.promote = true;			//promotion
			if (!temp.check)
				temp.promotion = game->moves[m].back();
			else
				temp.promotion = game->moves[m][game->moves[m].size()-2];
		}
		if (game->moves[m].find("x") != string::npos)
			temp.capture = true;			//capture

		if (game->moves[m][0] > 96 && game->moves[m][0] < 105 && !temp.castling) {				//pawn move
			temp.piece = 'p';
			if (!temp.capture) {												//no capture
				temp.col = (int)game->moves[m][0] - 97;
				temp.row = (int)game->moves[m][1] - 48;
			}
			else {																//capture
				temp.pcol = (int)game->moves[m][0] - 97;
				temp.col = (int)game->moves[m][2] - 97;
				temp.row = (int)game->moves[m][3] - 48;
			}
		}

		else if (game->moves[m][0] > 64 && game->moves[m][0] < 91 && !temp.castling) {			//other piece move
			if (!temp.capture)	{												//no capture
				if (game->moves[m][2] > 48 && game->moves[m][2] < 57) {			//no prev column
					temp.piece = game->moves[m][0];
					temp.col = (int)game->moves[m][1] - 97;
					temp.row = (int)game->moves[m][2] - 48;
				}
				else {
					temp.piece = game->moves[m][0];
					temp.pcol = (int)game->moves[m][1] - 97;
					temp.col = (int)game->moves[m][2] - 97;
					temp.row = (int)game->moves[m][3] - 48;
				}
			}
			else {
				if (game->moves[m][3] > 48 && game->moves[m][3] < 57) {			//no prev column
					temp.piece = game->moves[m][0];
					temp.col = (int)game->moves[m][2] - 97;
					temp.row = (int)game->moves[m][3] - 48;
				}
				else {
					temp.piece = game->moves[m][0];
					temp.pcol = (int)game->moves[m][1] - 97;
					temp.col = (int)game->moves[m][3] - 97;
					temp.row = (int)game->moves[m][4] - 48;
				}
			}

		}
		temp.row--;
		if (!m % 2)
			temp.color = 1;
		else
			temp.color = -1;

		game->postMoves.push_back(temp);
	}
}


Match::Match() {
	string filename,temp;
	ifstream pgnfile;
	//cout << "Enter .PGN file name: ";
	//cin >> filename;
	filename = "test";
	pgnfile.open("data\\" + filename + ".pgn", ios::in);
	if (pgnfile.fail()){
		cout << "Error: could not open file" << endl;
		exit(1);
	}
	cout << "Loading PGN" << endl;
	while (getline(pgnfile, temp) && !temp.empty()) {
		readBasicInfo(temp, this);
	}
	while (getline(pgnfile, temp))
		readMoves(temp, this);

	if (moves.back().find("-") > 0)			//deletion score from moves
	moves.pop_back();

	//reverse(moves.begin(), moves.end());
	parseMoves(this);
	reverse(postMoves.begin(), postMoves.end());

	cout << "PGN loaded" << endl;

	pgnfile.close();

	//write(this);
}