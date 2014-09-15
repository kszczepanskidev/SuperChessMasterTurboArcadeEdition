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

void Piece::move() {
	if (fabs(target.x - xCoord) < 0.00001 && fabs(target.y - yCoord) < 0.00001)
	{
		moving = false;
		current = target;
		//cout << "x:" << xCoord << " y:" << yCoord << endl;
	}
	else {
		matM = translate(matM, vec3(float(xDistance / 25.0f), 0.0f, float(yDistance / 25.0f)));
		xCoord += xDistance / 25.0f;
		yCoord += yDistance / 25.0f;
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

void Piece::setTarget(Square t) {
	if (!moving) {
		target = t;
		xDistance = target.x - current.x;
		yDistance = target.y - current.y;
		moving = true;
	}
}

void Piece::setOnBoard(bool ingame){
	onBoard = ingame;
}

bool Piece::getOnBoard() {
	return onBoard;
}

void Piece::setPossibleMoves() {
	switch (type) {
	case 'K':
		break;
	case 'Q':
		break;
	case 'R':
		break;
	case 'B':
		break;
	case 'N':
		break;
	case 'p':
		break;
	}
}

Piece::Piece(Model *m, GLuint *tex, GLuint *specular, Square square, char type) {
	model = m;
	matM = translate(mat4(1.0f), vec3(square.x, 0.0f, square.y));
	texture = tex;
	textureSpecular = specular;
	onBoard = true;
	moving = false;
	current = square;
	xCoord = current.x;
	yCoord = current.y;

	if (current.x == 1.75f){	//rz¹d 1
		if ((current.y == 1.25f) || (current.y == -1.25f) || (current.y == 0.25f))
			matM = rotate(matM, -90.0f, vec3(0.0f, 1.0f, 0.0f));
		if ((current.y == 0.75f) || (current.y == -0.75f))
			matM = rotate(matM, -180.0f, vec3(0.0f, 1.0f, 0.0f));

	}
	if (current.x == -1.75f){	//rz¹d 8
		if ((current.y == 1.25f) || (current.y == -1.25f) || (current.y == 0.25f))
			matM = rotate(matM, 90.0f, vec3(0.0f, 1.0f, 0.0f));

	}
}