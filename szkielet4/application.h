#ifndef APPLICATION_H
#define APPLICATION_H

#include "glm/glm.hpp"

#include "shaderprogram.h"
#include "board.h"
#include "piece.h"
#include "model.h"
#include "pgn.h"

class Application {
public:
	ShaderProgram *shader;					//Wskaünik na obiekt reprezentujπcy program cieniujπcy.

	GLuint textures[4];						//array of texture handles

	Model* models[6];						//array of pointers to models
	Chessboard* chessBoard;
	Piece* pieces[35];						//0-31 all pieces | 32-33 pieces to move | 34 piece to beat
	Match* match;

	Application();
};

#endif