#ifndef PGN_H
#define PGN_H

using namespace std;

class Match {
public:
	string name;
	string place;
	string date;
	string round;
	string whitePlayer;
	string blackPlayer;
	string result;
	string eco;
	string whiteElo;
	string blackElo;
	string annotator;
	vector<string> moves;

	Match();
};

int parseMove(string);

#endif