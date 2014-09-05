#include "model.h"
#include "board.h"

#include "glm/gtc/type_ptr.hpp"

void Chessboard::draw(ShaderProgram* shader) {
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


Chessboard::Chessboard(Model* m, GLuint* tex, GLuint* specular) {
	model = m;
	matM = mat4(1.0f);
	texture = tex;
	textureSpecular = specular;
}