#include "glm/glm.hpp"

#include "shaderprogram.h"
#include "model.h"
#include "board.h"
#include "piece.h"
#include "pgn.h"
#include "tga.h"
#include "chessboard.h"

float *vert = chessboardVertices;
float *norms = chessboardNormals;
float *texc = chessboardTexCoords;
int vcount = chessboardVertexCount;

#include "application.h"

Application::Application(ShaderProgram *shader) {
	//Init models
	models[0] = new Model("data\\models\\pawn.obj", shader);
	models[1] = new Model("data\\models\\knight.obj", shader);
	models[2] = new Model("data\\models\\queen.obj", shader);
	models[3] = new Model("data\\models\\bishop.obj", shader);
	models[4] = new Model("data\\models\\rook.obj", shader);
	models[5] = new Model("data\\models\\king.obj", shader);
	models[6] = new Model(vert, norms, texc, vcount, shader);

	//Init textures
	textures[0] = readTexture("data\\textures\\blackpiece2.tga");
	textures[1] = readTexture("data\\textures\\whitepiece.tga");
	textures[2] = readTexture("data\\textures\\chessboardSpec3.tga"); //board.tga
	textures[3] = readTexture("data\\textures\\glass2.tga"); //wood.tga
	textures[4] = readTexture("data\\textures\\chessboard.tga");


	//Init chessboard
	chessBoard = new Chessboard(models[6], &textures[4], &textures[2]);
	for (int i = 0; i < 8; ++i)
		for (int j = 0; j < 8; ++j) {
			chessBoard->squares[i][j].x = (3.5 - j)*0.5f;
			chessBoard->squares[i][j].y = (-3.5 + i)*0.5f;
		}

	for (int i = 0; i < 8; i++) {
		chessBoard->squares[i][0].piece = -1;
		chessBoard->squares[i][1].piece = -1;
		chessBoard->squares[i][6].piece = 1;
		chessBoard->squares[i][7].piece = 1;
	}

	//Init pieces
	//Pawns
	for (int i = 0; i < 8; ++i) {
		pieces[i] = new Piece(models[0], &textures[0], &textures[3], &chessBoard->squares[i][1], 'p');
		pieces[i+16] = new Piece(models[0], &textures[1], &textures[3], &chessBoard->squares[i][6], 'p');
	}
	//Knights
	pieces[8] = new Piece(models[1], &textures[0], &textures[3], &chessBoard->squares[1][0], 'N');
	pieces[9] = new Piece(models[1], &textures[0], &textures[3], &chessBoard->squares[6][0], 'N');
	pieces[24] = new Piece(models[1], &textures[1], &textures[3], &chessBoard->squares[1][7], 'N');
	pieces[25] = new Piece(models[1], &textures[1], &textures[3], &chessBoard->squares[6][7], 'N');
	//Queens
	pieces[10] = new Piece(models[2], &textures[0], &textures[3], &chessBoard->squares[3][0], 'Q');
	pieces[26] = new Piece(models[2], &textures[1], &textures[3], &chessBoard->squares[3][7], 'Q');
	//Bishops
	pieces[11] = new Piece(models[3], &textures[0], &textures[3], &chessBoard->squares[2][0], 'B');
	pieces[12] = new Piece(models[3], &textures[0], &textures[3], &chessBoard->squares[5][0], 'B');
	pieces[27] = new Piece(models[3], &textures[1], &textures[3], &chessBoard->squares[2][7], 'B');
	pieces[28] = new Piece(models[3], &textures[1], &textures[3], &chessBoard->squares[5][7], 'B');
	//Rooks
	pieces[13] = new Piece(models[4], &textures[0], &textures[3], &chessBoard->squares[0][0], 'R');
	pieces[14] = new Piece(models[4], &textures[0], &textures[3], &chessBoard->squares[7][0], 'R');
	pieces[29] = new Piece(models[4], &textures[1], &textures[3], &chessBoard->squares[0][7], 'R');
	pieces[30] = new Piece(models[4], &textures[1], &textures[3], &chessBoard->squares[7][7], 'R');
	//Kings
	pieces[15] = new Piece(models[5], &textures[0], &textures[3], &chessBoard->squares[4][0], 'K');
	pieces[31] = new Piece(models[5], &textures[1], &textures[3], &chessBoard->squares[4][7], 'K');

	for (int i = 0; i < 32; i++) {
		if (i<16)
			pieces[i]->color = -1;
		else if (i>15)
			pieces[i]->color = 1;
		pieces[i]->setPossibleMoves(chessBoard);
	}


	//Init moves
	match = new Match();
}