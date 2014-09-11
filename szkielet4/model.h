#ifndef MODEL_H
#define MODEL_H

#include <vector>

#include "glm/glm.hpp"
#include "shaderprogram.h"

using namespace std;
using namespace glm;

class Model {
public:
	GLuint Vertices, Normals, TexCoords;
	GLuint Indices, VAO;

	int IndicesCount;

	Model(string, ShaderProgram*);	//loading Wavefront model and VBO, VAO setup
	Model(float*, float*, float*, int, ShaderProgram*);	//loading model from vertices and VBO, VAO setup
	~Model();						//VBO, VAO destructor
};


#endif