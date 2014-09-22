#ifndef PGN_H
#define PGN_H

using namespace std;

struct Move {
	int col, row;
	int pcol;		//previous column
	int color;      //side
	char piece, promotion;
	bool capture, promote;
	int castling;	//1 - short, 2 - queen
	int check;		//1- check, 2 - checkmate

	int pcolumn, prow, pieceno, pieceno2;	//data to reverse moves
};

class Match {
public:
	string name, place, date, round;
	string whitePlayer, blackPlayer, whiteElo, blackElo;
	string result, eco, annotator;

	vector<string> moves;
	vector<Move> postMoves;
	vector<Move> reMoves;
	void write(Match*);

	Match();
};



#endif