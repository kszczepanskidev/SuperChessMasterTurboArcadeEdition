#ifndef PIECE_H
#define PIECE_H

class Piece {
	Model* model;
	char type;
	mat4 matM;
	GLuint *texture, *textureSpecular;
	float xDistance, yDistance, xCoord, yCoord;
	vector<Square> possibleMoves;
	bool onBoard, moving;
	Square current, target;

public:
	void draw(ShaderProgram*);
	void move();
	void promotion(Model*);

	void setTarget(Square);
	void setOnBoard(bool);
	bool getOnBoard();
	bool getMoving();
	void setMoving(bool);
	void setPossibleMoves();

	Piece(Model*, GLuint*, GLuint*, Square, char);
};

#endif