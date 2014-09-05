#ifndef PIECE_H
#define PIECE_H

struct Square {
	float x, z;
};

class Piece {
	Model* model;
	mat4 matM;
	GLuint* texture;
	GLuint* textureSpecular;
	float xDistance, zDistance;
	bool onBoard;

	Square current, target;

public:
	void draw(ShaderProgram*);
	void setTarget(Square);
	void move();
	void promotion(Model*);

	void setOnBoard(bool);

	Piece(Model*, GLuint*, GLuint*, Square);
};

#endif