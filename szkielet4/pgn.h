#ifndef PGN_H
#define PGN_H

using namespace std;

class Match {
public:
	string name, place, date, round;
	string whitePlayer, blackPlayer, whiteElo, blackElo;
	string result, eco, annotator;

	vector<string> moves;

	Match();
};

int parseMove(string);

#endif