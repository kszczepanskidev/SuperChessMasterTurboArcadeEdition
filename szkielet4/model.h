#ifndef MODEL_H
#define MODEL_H

#include <vector>

#include "glm/glm.hpp"
#include "shaderprogram.h"

using namespace std;
using namespace glm;

class Model {
public:
	GLuint Vertices;
	GLuint Normals;
	GLuint TexCoords;
	GLuint Indices;
	GLuint VAO;

	int IndicesCount;

	Model(string, ShaderProgram*);	//loading Wavefront model and VBO, VAO setup
	~Model();						//VBO, VAO destructor
};


#endif