#ifndef PIECE_H
#define PIECE_H

class Piece {
	Model* model;
	mat4 matM;
	GLuint *texture, *textureSpecular;
	float xDistance, yDistance, xCoord, yCoord;
	bool onBoard, moving;
	int rot;

public:
	Square *current, *target;
	int color;
	char type;
	vector<Square*> possibleMoves;

	void draw(ShaderProgram*);
	void move(Chessboard*);
	void promotion(Model*);

	void setTarget(Square*);
	void setOnBoard(bool);
	bool getOnBoard();
	bool getMoving();
	void setMoving(bool);
	void setPossibleMoves(Chessboard*);

	Piece findPiece(Square* move);

	Piece(Model*, GLuint*, GLuint*, Square*, char);
};

#endif