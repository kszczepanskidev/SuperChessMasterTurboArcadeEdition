#include <iostream>

#include "model.h"
#include "piece.h"

#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace glm;

Piece::Piece(Model *m, GLuint *tex, GLuint *specular, Square square) {
	model = m;
	matM = translate(mat4(1.0f), vec3(square.x, 0.0f, square.z));
	texture = tex;
	textureSpecular = specular;
	onBoard = true;
	current = square;
}

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

void Piece::setTarget(Square t) {
	target = t;
}