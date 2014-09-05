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
	glUniformMatrix4fv(shader->getUniformLocation("M"), 1, false, value_ptr(matM));		//glUniform1f(shader->getUniformLocation("shininess"), 25);
	
	glBindVertexArray(model->VAO);	

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->Indices);

	glActiveTexture(GL_TEXTURE0);										
	glBindTexture(GL_TEXTURE_2D, *texture);
	glActiveTexture(GL_TEXTURE1);										
	glBindTexture(GL_TEXTURE_2D, *textureSpecular);


	glDrawElements(GL_TRIANGLES, model->IndicesCount, GL_UNSIGNED_SHORT, NULL);
	//glBindVertexArray(0);
}

void Piece::move() {
	//matM = translate(matM, vec3(float((target.x - current.x) / 25.0f), 0.0f, float((target.y - current.y) / 25.0f)));
	matM = translate(matM, vec3(float(xDistance / 25.0f), 0.0f, float(yDistance / 25.0f)));

	if ((target.x == current.x) && (target.y == current.y))
		moving = false;
}

bool Piece::getOnBoard() {
	return moving;
}

void Piece::promotion(Model* m) {
	model = m;
}

void Piece::setTarget(Square t) {
	target = t;
	xDistance = target.x - current.x;
	yDistance = target.y - current.y;
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
}