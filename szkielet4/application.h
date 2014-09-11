#ifndef APPLICATION_H
#define APPLICATION_H

#include "shaderprogram.h"
#include "model.h"
#include "board.h"
#include "piece.h"
#include "pgn.h"

class Application {
public:
	//ShaderProgram *shader;					//Wskaünik na obiekt reprezentujπcy program cieniujπcy.

	GLuint textures[5];						//array of texture handles

	Model* models[7];						//array of pointers to models
	Chessboard* chessBoard;
	Piece* pieces[35];						//0-31 all pieces | 32-33 pieces to move | 34 piece to beat
	Match* match;

	Application(ShaderProgram*);
};

#endif