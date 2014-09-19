#include "model.h"
#include "board.h"

#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"


void Chessboard::draw(ShaderProgram* shader) {
	glBindVertexArray(model->VAO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->Indices);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, *texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, *textureSpecular);

	matM = translate(rotate(scale(mat4(1.0f), vec3(1.0f, 0.25f, 1.0f)), -90.0f, vec3(0, 1, 0)), vec3(0.0f, -1.2f, 0.0f));
	glUniformMatrix4fv(shader->getUniformLocation("M"), 1, false, value_ptr(matM));
	glUniform1f(shader->getUniformLocation("shininess"), 30.0f);

	glDrawArrays(GL_TRIANGLES, 0, model->IndicesCount); // Starting from vertex 0; 438 vertices total -> 146 triangles
	glBindVertexArray(0);
}


Chessboard::Chessboard(Model* m, GLuint* tex, GLuint* specular) {
	model = m;
	matM = mat4(1.0f);
	texture = tex;
	textureSpecular = specular;

	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++) {
			squares[i][j].piece = 0;
			squares[i][j].col = i;	//A..H
			squares[i][j].row = j;	//1..8
		}
}