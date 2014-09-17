#ifndef BOARD_H
#define BOARD_H

#include "model.h"

struct Square {
	float x, y;
	int row, col;								
	int piece;								//side: -1 black | 1 white | 0 empty
};


class Chessboard {
	Model *model;
	mat4 matM;
	GLuint *texture, *textureSpecular;

public:																					
	Square squares[8][8];

	void draw(ShaderProgram*);

	Chessboard(Model*, GLuint*, GLuint*);
};

#endif