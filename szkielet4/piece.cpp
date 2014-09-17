#include <iostream>
#include <string>
#include <vector>

#include "model.h"
#include "board.h"
#include "piece.h"

#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace glm;

void Piece::draw(ShaderProgram* shader) {
	glUniformMatrix4fv(shader->getUniformLocation("M"), 1, false, value_ptr(matM));
	glUniform1f(shader->getUniformLocation("shininess"), 10);
	
	glBindVertexArray(model->VAO);	

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->Indices);

	glActiveTexture(GL_TEXTURE0);										
	glBindTexture(GL_TEXTURE_2D, *texture);
	glActiveTexture(GL_TEXTURE1);										
	glBindTexture(GL_TEXTURE_2D, *textureSpecular);

	glDrawElements(GL_TRIANGLES, model->IndicesCount, GL_UNSIGNED_SHORT, NULL);
	glBindVertexArray(0);
}

void Piece::move(Chessboard* board) {
	if (fabs(target->x - xCoord) < 0.00001 && fabs(target->y - yCoord) < 0.00001)	{
		moving = false;
		current->piece = 0;
		target->piece = color;
		current = target;
		setPossibleMoves(board);
	}
	else {
		switch (rot) {
		case 0:
			matM = rotate(matM, 90.0f, vec3(0.0f, 1.0f, 0.0f));
			break;
		case 1:
			matM = rotate(matM, 180.0f, vec3(0.0f, 1.0f, 0.0f));
			break;
		case 2:
			matM = rotate(matM, -90.0f, vec3(0.0f, 1.0f, 0.0f));
			break;
		}

		matM = translate(matM, vec3(float(xDistance / 25.0f), 0.0f, float(yDistance / 25.0f)));
		xCoord += xDistance / 25.0f;
		yCoord += yDistance / 25.0f;

		switch (rot) {
		case 0:
			matM = rotate(matM, -90.0f, vec3(0.0f, 1.0f, 0.0f));
			break;
		case 1:
			matM = rotate(matM, -180.0f, vec3(0.0f, 1.0f, 0.0f));
			break;
		case 2:
			matM = rotate(matM, 90.0f, vec3(0.0f, 1.0f, 0.0f));
			break;
		}
	}
	
}

bool Piece::getMoving() {
	return moving;
}
void Piece::setMoving(bool m) {
	moving = m;
}

void Piece::promotion(Model* m) {
	model = m;
}

void Piece::setTarget(Square* t) {
	if (!moving) {
		target = t;
		xDistance = target->x - current->x;
		yDistance = target->y - current->y;
		moving = true;
	}
}

void Piece::setOnBoard(bool ingame){
	onBoard = ingame;
}

bool Piece::getOnBoard() {
	return onBoard;
}

void Piece::setPossibleMoves(Chessboard* board) {	
	int x = current->row;
	int y = current->col;
	int a, b;
	possibleMoves.clear();

	switch (type) {
	case 'K':
		if (!board->squares[y + 1][x + 1].piece || board->squares[y + 1][x + 1].piece != color)
			possibleMoves.push_back(board->squares[y + 1][x + 1]);
		if (!board->squares[y + 1][x].piece || board->squares[y + 1][x].piece != color)
			possibleMoves.push_back(board->squares[y + 1][x]);
		if (!board->squares[y + 1][x - 1].piece || board->squares[y + 1][x - 1].piece != color)
			possibleMoves.push_back(board->squares[y + 1][x - 1]);
		if (!board->squares[y][x + 1].piece || board->squares[y][x + 1].piece != color)
			possibleMoves.push_back(board->squares[y][x + 1]);
		if (!board->squares[y][x - 1].piece || board->squares[y][x - 1].piece != color)
			possibleMoves.push_back(board->squares[y][x - 1]);
		if (!board->squares[y - 1][x + 1].piece || board->squares[y - 1][x + 1].piece != color)
			possibleMoves.push_back(board->squares[y - 1][x + 1]);
		if (!board->squares[y - 1][x].piece || board->squares[y - 1][x].piece != color)
			possibleMoves.push_back(board->squares[y - 1][x]);
		if (!board->squares[y - 1][x - 1].piece || board->squares[y - 1][x - 1].piece != color)
			possibleMoves.push_back(board->squares[y - 1][x - 1]);

		break;
	case 'Q':
		for (int i = x + 1; i < 8; i++)
			if (!board->squares[y][i].piece)
				possibleMoves.push_back(board->squares[y][i]);
			else if (board->squares[y][i].piece != color) {
				possibleMoves.push_back(board->squares[y][i]);
				break;
			}
			else break;

		for (int i = x - 1; i >= 0; i--)
			if (!board->squares[y][i].piece)
				possibleMoves.push_back(board->squares[y][i]);
			else if (board->squares[y][i].piece != color) {
				possibleMoves.push_back(board->squares[y][i]);
				break;
			}
			else break;

		for (int i = y + 1; i < 8; i++)
			if (!board->squares[i][x].piece)
				possibleMoves.push_back(board->squares[i][x]);
			else if (board->squares[i][x].piece != color) {
				possibleMoves.push_back(board->squares[i][x]);
				break;
			}
			else break;

		for (int i = y - 1; i >= 0; i--)
			if (!board->squares[i][x].piece)
				possibleMoves.push_back(board->squares[i][x]);
			else if (board->squares[i][x].piece != color) {
				possibleMoves.push_back(board->squares[i][x]);
				break;
			}
			else break;
			a = y + 1,
				b = x + 1;

			do { //1
				if (!board->squares[a][b].piece)
					possibleMoves.push_back(board->squares[a][b]);
				else if (board->squares[a][b].piece != color) {
					possibleMoves.push_back(board->squares[a][b]);
					break;
				}
				else break;
				++a;
				++b;
			} while (a < 8 || b < 8);

			a = y - 1;
			b = x + 1;
			do { //2
				if (!board->squares[a][b].piece)
					possibleMoves.push_back(board->squares[a][b]);
				else if (board->squares[a][b].piece != color) {
					possibleMoves.push_back(board->squares[a][b]);
					break;
				}
				else break;
				--a;
				++b;
			} while (a >= 0 || b < 8);

			a = y - 1;
			b = x - 1;
			do { //3
				if (!board->squares[a][b].piece)
					possibleMoves.push_back(board->squares[a][b]);
				else if (board->squares[a][b].piece != color) {
					possibleMoves.push_back(board->squares[a][b]);
					break;
				}
				else break;
				--a;
				--b;
			} while (a >= 0 || b >= 0);

			a = y + 1;
			b = x - 1;
			do { //4
				if (!board->squares[a][b].piece)
					possibleMoves.push_back(board->squares[a][b]);
				else if (board->squares[a][b].piece != color) {
					possibleMoves.push_back(board->squares[a][b]);
					break;
				}
				else break;
				++a;
				--b;
			} while (a < 8 || b >= 0);

		break;
	case 'R':
		for (int i = x + 1; i < 8; i++)
			if (!board->squares[y][i].piece)
				possibleMoves.push_back(board->squares[y][i]);
			else if (board->squares[y][i].piece != color) {
				possibleMoves.push_back(board->squares[y][i]);
				break;
			}
			else break;

		for (int i = x - 1; i >= 0; i--)
			if (!board->squares[y][i].piece)
				possibleMoves.push_back(board->squares[y][i]);
			else if (board->squares[y][i].piece != color) {
				possibleMoves.push_back(board->squares[y][i]);
				break;
			}
			else break;

		for (int i = y + 1; i < 8; i++)
			if (!board->squares[i][x].piece)
				possibleMoves.push_back(board->squares[i][x]);
			else if (board->squares[i][x].piece != color) {
				possibleMoves.push_back(board->squares[i][x]);
				break;
			}
			else break;

		for (int i = y - 1; i >= 0; i--)
			if (!board->squares[i][x].piece)
				possibleMoves.push_back(board->squares[i][x]);
			else if (board->squares[i][x].piece != color) {
				possibleMoves.push_back(board->squares[i][x]);
				break;
			}
			else break;

		break;
	case 'B':
		a = y + 1, 
		b = x + 1;

		do { //1
			if (!board->squares[a][b].piece)
				possibleMoves.push_back(board->squares[a][b]);
			else if (board->squares[a][b].piece != color) {
				possibleMoves.push_back(board->squares[a][b]);
				break;
			}
			else break;
			++a;
			++b;
		} while (a < 8 || b < 8);

		a = y - 1;
		b = x + 1;
		do { //2
			if (!board->squares[a][b].piece)
				possibleMoves.push_back(board->squares[a][b]);
			else if (board->squares[a][b].piece != color) {
				possibleMoves.push_back(board->squares[a][b]);
				break;
			}
			else break;
			--a;
			++b;
		} while (a >= 0 || b < 8);

		a = y - 1;
		b = x - 1;
		do { //3
			if (!board->squares[a][b].piece)
				possibleMoves.push_back(board->squares[a][b]);
			else if (board->squares[a][b].piece != color) {
				possibleMoves.push_back(board->squares[a][b]);
				break;
			}
			else break;
			--a;
			--b;
		} while (a >= 0 || b >= 0);

		a = y + 1;
		b = x - 1;
		do { //4
			if (!board->squares[a][b].piece)
				possibleMoves.push_back(board->squares[a][b]);
			else if (board->squares[a][b].piece != color) {
				possibleMoves.push_back(board->squares[a][b]);
				break;
			}
			else break;
			++a;
			--b;
		} while (a < 8 || b >= 0);
		break;
	case 'N':
		if (!board->squares[y + 1][x + 2].piece || board->squares[y + 1][x + 2].piece != color)
			possibleMoves.push_back(board->squares[y + 1][x + 2]);
		if (!board->squares[y + 2][x + 1].piece || board->squares[y + 2][x + 1].piece != color)
			possibleMoves.push_back(board->squares[y + 2][x + 1]);
		if (!board->squares[y + 2][x - 1].piece || board->squares[y + 2][x - 1].piece != color)
			possibleMoves.push_back(board->squares[y + 2][x - 1]);
		if (!board->squares[y + 1][x - 2].piece || board->squares[y + 1][x - 2].piece != color)
			possibleMoves.push_back(board->squares[y + 1][x - 2]);
		if (!board->squares[y - 1][x - 2].piece || board->squares[y - 1][x - 2].piece != color)
			possibleMoves.push_back(board->squares[y - 1][x - 2]);
		if (!board->squares[y - 2][x - 1].piece || board->squares[y - 2][x - 1].piece != color)
			possibleMoves.push_back(board->squares[y - 2][x - 1]);
		if (!board->squares[y - 2][x + 1].piece || board->squares[y - 1][x + 1].piece != color)
			possibleMoves.push_back(board->squares[y - 1][x + 1]);
		if (!board->squares[y - 1][x + 2].piece || board->squares[y - 1][x + 2].piece != color)
			possibleMoves.push_back(board->squares[y - 1][x + 2]);
		
		break;
	case 'p':
		if (color == 1) {
			if (x == 1) {
				if (!board->squares[y][x - 1].piece) {
					possibleMoves.push_back(board->squares[y][x - 1]);
					if (!board->squares[y][x - 2].piece)
						possibleMoves.push_back(board->squares[y][x - 2]);
				}
			}
			else {
				if (!board->squares[y][x - 1].piece)
					possibleMoves.push_back(board->squares[y][x - 1]);
				if (board->squares[y + 1][x - 1].piece != color)
					possibleMoves.push_back(board->squares[y + 1][x - 1]);
				if (board->squares[y - 1][x - 1].piece != color)
					possibleMoves.push_back(board->squares[y - 1][x - 1]);
			}
		}
		else {
			if (x == 1) {
				if (!board->squares[y][x + 1].piece) {
					possibleMoves.push_back(board->squares[y][x + 1]);
					if (!board->squares[y][x + 2].piece)
						possibleMoves.push_back(board->squares[y][x + 2]);
				}
			}
			else {
				if (!board->squares[y][x + 1].piece)
					possibleMoves.push_back(board->squares[y][x + 1]);
				if (board->squares[y + 1][x + 1].piece != color)
					possibleMoves.push_back(board->squares[y + 1][x + 1]);
				if (board->squares[y - 1][x + 1].piece != color)
					possibleMoves.push_back(board->squares[y - 1][x + 1]);
			}
		}
		break;
	}
}

Piece::Piece(Model *m, GLuint *tex, GLuint *specular, Square* square, char t) {
	model = m;
	type = t;
	matM = translate(mat4(1.0f), vec3(square->x, 0.0f, square->y));
	texture = tex;
	textureSpecular = specular;
	onBoard = true;
	moving = false;
	current = square;
	xCoord = current->x;
	yCoord = current->y;

	if (current->x == 1.75f){	//rz¹d 1
		if ((current->y == 1.25f) || (current->y == -1.25f) || (current->y == 0.25f)) {
			rot = 0;
			matM = rotate(matM, -90.0f, vec3(0.0f, 1.0f, 0.0f));
		}
		if ((current->y == 0.75f) || (current->y == -0.75f)) {
			rot = 1;
			matM = rotate(matM, -180.0f, vec3(0.0f, 1.0f, 0.0f));
		}

	}
	if (current->x == -1.75f){	//rz¹d 8
		if ((current->y == 1.25f) || (current->y == -1.25f) || (current->y == 0.25f)) {
			rot = 2;
			matM = rotate(matM, 90.0f, vec3(0.0f, 1.0f, 0.0f));
		}
	}
}