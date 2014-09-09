#include "glm/glm.hpp"

#include "shaderprogram.h"
#include "model.h"
#include "board.h"
#include "piece.h"
#include "pgn.h"
#include "tga.h"

#include "application.h"

Application::Application(ShaderProgram *shader) {
	//Init models
	models[0] = new Model("data\\models\\pawn.obj", shader);
	models[1] = new Model("data\\models\\knight.obj", shader);
	models[2] = new Model("data\\models\\queen.obj", shader);
	models[3] = new Model("data\\models\\bishop.obj", shader);
	models[4] = new Model("data\\models\\rook.obj", shader);
	models[5] = new Model("data\\models\\king.obj", shader);

	//Init textures
	textures[0] = readTexture("data\\textures\\whitepiece.tga");
	textures[1] = readTexture("data\\textures\\blackpiece.tga");
	textures[2] = readTexture("data\\textures\\board.tga");
	textures[3] = readTexture("data\\textures\\wood.tga");

	//Init chessboard
	chessBoard = new Chessboard(models[0], &textures[2], &textures[3]);
	for (int i = 0; i < 8; ++i)
		for (int j = 0; j < 8; ++j) {
			chessBoard->squares[i][j].x = (3.5 - j)*0.5f;
			chessBoard->squares[i][j].y = (-3.5 + i)*0.5f;
		}

	//Init pieces
	//Pawns
	for (int i = 0; i < 8; ++i) {
		pieces[i] = new Piece(models[0], &textures[0], &textures[3], chessBoard->squares[i][1], 'p');
		pieces[i+8] = new Piece(models[0], &textures[1], &textures[3], chessBoard->squares[i][6], 'p');
	}
	//Knights
	pieces[16] = new Piece(models[1], &textures[0], &textures[3], chessBoard->squares[1][0], 'N');
	pieces[17] = new Piece(models[1], &textures[0], &textures[3], chessBoard->squares[6][0], 'N');
	pieces[18] = new Piece(models[1], &textures[1], &textures[3], chessBoard->squares[1][7], 'N');
	pieces[19] = new Piece(models[1], &textures[1], &textures[3], chessBoard->squares[6][7], 'N');
	//Queens
	pieces[20] = new Piece(models[2], &textures[0], &textures[3], chessBoard->squares[3][0], 'Q');
	pieces[21] = new Piece(models[2], &textures[1], &textures[3], chessBoard->squares[3][7], 'Q');
	//Bishops
	pieces[22] = new Piece(models[3], &textures[0], &textures[3], chessBoard->squares[2][0], 'B');
	pieces[23] = new Piece(models[3], &textures[0], &textures[3], chessBoard->squares[5][0], 'B');
	pieces[24] = new Piece(models[3], &textures[1], &textures[3], chessBoard->squares[2][7], 'B');
	pieces[25] = new Piece(models[3], &textures[1], &textures[3], chessBoard->squares[5][7], 'B');
	//Rooks
	pieces[26] = new Piece(models[4], &textures[0], &textures[3], chessBoard->squares[0][0], 'R');
	pieces[27] = new Piece(models[4], &textures[0], &textures[3], chessBoard->squares[7][0], 'R');
	pieces[28] = new Piece(models[4], &textures[1], &textures[3], chessBoard->squares[0][7], 'R');
	pieces[29] = new Piece(models[4], &textures[1], &textures[3], chessBoard->squares[7][7], 'R');
	//Kings
	pieces[30] = new Piece(models[5], &textures[0], &textures[3], chessBoard->squares[4][0], 'K');
	pieces[31] = new Piece(models[5], &textures[1], &textures[3], chessBoard->squares[4][7], 'K');

	//Init moves
	match = new Match();
}