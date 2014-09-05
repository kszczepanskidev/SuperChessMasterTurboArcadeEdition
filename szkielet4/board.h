#ifndef BOARD_H
#define BOARD_H

struct Square {
	float x, y;
	char rank, file;
};


class Chessboard{
	Model* model;
	mat4 matM;
	GLuint *texture, *textureSpecular;

public:																					
	Square squares[8][8];

	void draw(ShaderProgram*);

	Chessboard(Model*, GLuint*, GLuint*);
};

#endif