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

	//array of moves or sth
};

void read(string temp, Match* game);
void write(Match* game);

#endif