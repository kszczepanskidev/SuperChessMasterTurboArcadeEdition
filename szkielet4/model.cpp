#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "model.h"
#include "vboindex.h"
#include "glm/glm.hpp"

using namespace std;
using namespace glm;

void assignVBOtoAttribute(ShaderProgram *shader, char* attributeName, GLuint bufVBO, int variableSize) {
	GLuint location = shader->getAttribLocation(attributeName); //Pobierz numery slotów dla atrybutu
	glBindBuffer(GL_ARRAY_BUFFER, bufVBO);  //Uaktywnij uchwyt VBO 
	glEnableVertexAttribArray(location); //W³¹cz u¿ywanie atrybutu o numerze slotu zapisanym w zmiennej location
	glVertexAttribPointer(location, variableSize, GL_FLOAT, GL_FALSE, 0, NULL); //Dane do slotu location maj¹ byæ brane z aktywnego VBO
}

GLuint makeBuffer(vector <vec3> data, int vertexSize) {
	GLuint handle;

	glGenBuffers(1, &handle);//Wygeneruj uchwyt na Vertex Buffer Object (VBO), który bêdzie zawiera³ tablicê danych
	glBindBuffer(GL_ARRAY_BUFFER, handle);  //Uaktywnij wygenerowany uchwyt VBO 
	glBufferData(GL_ARRAY_BUFFER, data.size()*vertexSize, &data[0], GL_STATIC_DRAW);//Wgraj tablicê do VBO

	return handle;
}

GLuint makeBuffer(vector <vec2> data, int vertexSize) {
	GLuint handle;

	glGenBuffers(1, &handle);//Wygeneruj uchwyt na Vertex Buffer Object (VBO), który bêdzie zawiera³ tablicê danych
	glBindBuffer(GL_ARRAY_BUFFER, handle);  //Uaktywnij wygenerowany uchwyt VBO 
	glBufferData(GL_ARRAY_BUFFER, data.size()*vertexSize, &data[0], GL_STATIC_DRAW);//Wgraj tablicê do VBO

	return handle;
}

GLuint makeBuffer(vector <unsigned short> data, int vertexSize) {
	GLuint handle;

	glGenBuffers(1, &handle);//Wygeneruj uchwyt na Vertex Buffer Object (VBO), który bêdzie zawiera³ tablicê danych
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);  //Uaktywnij wygenerowany uchwyt VBO 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size()*vertexSize, &data[0], GL_STATIC_DRAW);//Wgraj tablicê do VBO

	return handle;
}

GLuint makeBuffer(void *data, int vertexCount, int vertexSize) {
	GLuint handle;

	glGenBuffers(1, &handle);//Wygeneruj uchwyt na Vertex Buffer Object (VBO), który bêdzie zawiera³ tablicê danych
	glBindBuffer(GL_ARRAY_BUFFER, handle);  //Uaktywnij wygenerowany uchwyt VBO 
	glBufferData(GL_ARRAY_BUFFER, vertexCount*vertexSize, data, GL_STATIC_DRAW);//Wgraj tablicê do VBO

	return handle;
}

void setupVBO(Model &model,	vector <unsigned short> indices, vector <vec3> vertices, vector <vec2> texCoords, vector <vec3> normals) {
	model.Vertices = makeBuffer(vertices, sizeof(vec3));
	model.Normals = makeBuffer(normals, sizeof(vec3));
	model.TexCoords = makeBuffer(texCoords, sizeof(vec2));
	model.Indices = makeBuffer(indices, sizeof(unsigned short));
}

void setupVBO(Model &model, float* vert, float* norms, float* texc, int vcount) {
	model.Vertices = makeBuffer(vert, vcount, sizeof(float)* 3); //Wspó³rzêdne wierzcho³ków
	model.Normals = makeBuffer(norms, vcount, sizeof(float)* 3);//Wektory normalne wierzcho³ków
	model.TexCoords = makeBuffer(texc, vcount, sizeof(float)* 2);//Wspó³rzêdne teksturowania
}


Model::Model(string filename, ShaderProgram* shader){
	string line, type, temp;

	vector<vec3> Verts;
	vector<vec2> Texs;
	vector<vec3> Norms;

	vector<vec3> tempVertices;
	vector<vec2> tempTexCoords;
	vector<vec3> tempNormals;

	vector<unsigned int> vertexIndices, texCoordsIndices, normalIndices;

	cout << "Loading .OBJ file | ";
	ifstream objFile(filename.c_str());
	if (objFile.fail())	{
		cout << "Error: could not open file" << endl;
		exit(1);
	}

	while (getline(objFile, line)) {
		stringstream ss(line);
		ss >> type;

		if (type == "#")
			continue;
		else if (type == "v") {
			vec3 vertex;
			for (int i = 0; i < 3; i++) {
				ss >> temp;
				vertex[i] = (float)atof(temp.c_str());
			}
			tempVertices.push_back(vertex);
		}
		else if (type == "vn") {
			vec3 normal;
			for (int i = 0; i < 3; i++) {
				ss >> temp;
				normal[i] = (float)atof(temp.c_str());
			}
			tempNormals.push_back(normal);
		}
		else if (type == "vt") {
			vec2 tex;
			for (int i = 0; i < 2; i++) {
				ss >> temp;
				tex[i] = (float)atof(temp.c_str());
			}
			tex.y = -tex.y;
			tempTexCoords.push_back(tex);
		}
		else if (type == "f") {
			int index[3];
			for (int i = 0; i < 3; i++) {
				ss >> temp;
				sscanf_s(temp.c_str(), "%d/%d/%d", &index[0], &index[1], &index[2]);
				vertexIndices.push_back(index[0]);
				texCoordsIndices.push_back(index[1]);
				normalIndices.push_back(index[2]);
			}
		}
	}	//EOF
	objFile.close();

	for (unsigned int i = 0; i < vertexIndices.size(); ++i) {
		Verts.push_back(tempVertices[vertexIndices[i] - 1]);
		Texs.push_back(tempTexCoords[texCoordsIndices[i] - 1]);
		Norms.push_back(tempNormals[normalIndices[i] - 1]);
	}

	cout << "Model loading complete | ";

	vector<unsigned short> indices;
	vector<vec3> indexedVertices;
	vector<vec2> indexedTexCoords;
	vector<vec3> indexedNormals;

	indexVBO(Verts, Texs, Norms, indices, indexedVertices, indexedTexCoords, indexedNormals);
	IndicesCount = indices.size();

	cout << "Indexing complete | ";

	setupVBO(*this, indices, indexedVertices, indexedTexCoords, indexedNormals);
	cout << "VBO buffers created | ";

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	assignVBOtoAttribute(shader, "vertex", Vertices, 3);	//"vertex" odnosi siê do deklaracji "in vec3 vertex;" w vertex shaderze
	assignVBOtoAttribute(shader, "normal", Normals, 3);		//"normal" odnosi siê do deklaracji "in vec3 normal;" w vertex shaderze
	assignVBOtoAttribute(shader, "texCoord", TexCoords, 2);	//"texCoord" odnosi siê do deklaracji "in vec2 texCoord;" w vertex shaderze
	
	cout << "VAO created | ";
	cout << "Model initialized" << endl << endl;
}

Model::Model(float* vert, float* norms, float* texc, int vcount, ShaderProgram* shader){

	setupVBO(*this, vert, norms, texc, vcount);
	cout << "VBO buffers created | ";

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	assignVBOtoAttribute(shader, "vertex", Vertices, 3);	//"vertex" odnosi siê do deklaracji "in vec3 vertex;" w vertex shaderze
	assignVBOtoAttribute(shader, "normal", Normals, 3);		//"normal" odnosi siê do deklaracji "in vec3 normal;" w vertex shaderze
	assignVBOtoAttribute(shader, "texCoord", TexCoords, 2);	//"texCoord" odnosi siê do deklaracji "in vec2 texCoord;" w vertex shaderze
	IndicesCount = vcount;
	
	cout << "VAO created | ";
	cout << "Model initialized" << endl << endl;
}

Model::~Model() {
	//FreeVBO
	glDeleteBuffers(1, &Vertices);
	glDeleteBuffers(1, &Normals);
	glDeleteBuffers(1, &TexCoords);
	glDeleteBuffers(1, &Indices);

	//FreeVAO
	glDeleteVertexArrays(1, &VAO);
}