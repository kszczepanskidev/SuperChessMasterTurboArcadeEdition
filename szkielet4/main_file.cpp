#include <stdio.h>
#include <vector>
#include <string>
#include <iterator>
#include <ctime>

#include "GL/glew.h"
#include "GL/freeglut.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "application.h"
#include "tga.h"
#include "shaderprogram.h"
#include "board.h"
#include "piece.h"
#include "model.h"
#include "pgn.h"


using namespace std;
using namespace glm;

int test=2;

//Macierze
mat4  matP;																										//rzutowania
mat4  matV;																										//widoku

//Ustawienia okna i rzutowania
int windowWidth=1280;
int windowHeight=720;
float cameraAngle=45.0f;

//Zmienne do animacji
float speedX = 0, speedY = 0;
float angleX = 0, angleY = 0;
int lastTime = 0;

//Uchwyty na shadery
ShaderProgram *shader;																					//WskaŸnik na obiekt reprezentuj¹cy program cieniuj¹cy.
Application *App;



//Procedura rysuj¹ca jakiœ obiekt. Ustawia odpowiednie parametry dla vertex shadera i rysuje.
void drawObjects() {
	//W³¹czenie programu cieniuj¹cego, który ma zostaæ u¿yty do rysowania
	//W tym programie wystarczy³oby wywo³aæ to raz, w setupShaders, ale chodzi o pokazanie, 
	//¿e mozna zmieniaæ program cieniuj¹cy podczas rysowania jednej sceny
	shader->use();

	//Przeka¿ do shadera macierze P i V.
	glUniformMatrix4fv(shader->getUniformLocation("P"), 1, false, value_ptr(matP));
	glUniformMatrix4fv(shader->getUniformLocation("V"), 1, false, value_ptr(matV));

	glUniform1i(shader->getUniformLocation("textureMap0"), 0);
	glUniform1i(shader->getUniformLocation("textureMap1"), 1);

	glUniform4f(shader->getUniformLocation("lightPosition1"), 4, 2, 4, 1);
	glUniform4f(shader->getUniformLocation("lightPosition2"), -4, 2, -4, 1);

	App->chessBoard->draw(shader);																				//drawing chessboard
	for (int i = 0; i < 32; i++)																				//drawing all pieces
		if (App->pieces[i]->getOnBoard())
			App->pieces[i]->draw(shader);
	
	
}

//Procedura rysuj¹ca
void displayFrame() {
	//Wyczyœæ bufor kolorów i bufor g³êbokoœci
	glClearColor(0.5,0.5,0.5,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Wylicz macierz rzutowania
	matP=glm::perspective(cameraAngle, (float)windowWidth/(float)windowHeight, 1.0f, 100.0f);
	
	//Wylicz macierz widoku
	matV = lookAt(vec3(0.0f, 5.0f, -5.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

	matV = rotate(matV, angleX, vec3(1.0f, 0.0f, 0.0f));
	matV = rotate(matV, angleY, vec3(0.0f, 1.0f, 0.0f));
	
	//Narysuj obiekty
	drawObjects();
	
	//Tylny bufor na przedni
	glutSwapBuffers();
}


//Procedura uruchamiana okresowo. Robi animacjê.
void nextFrame(void) {
	int actTime=glutGet(GLUT_ELAPSED_TIME);
	int interval=actTime-lastTime;
	lastTime=actTime;

	angleX += speedX*interval / 1000.0f;
	angleY += speedY*interval / 1000.0f;

	if (angleX>360) angleX -= 360;
	if (angleX<0) angleX += 360;
	if (angleY>360) angleY -= 360;
	if (angleY<0) angleY += 360;
	
	for (int i = 0; i < 32; ++i)
		if (App->pieces[i]->getMoving())
			App->pieces[i]->move(App->chessBoard);
			
	glutPostRedisplay();
}

//Procedura wywo³ywana przy zmianie rozmiaru okna
void changeSize(int w, int h) {
	//Ustawienie wymiarow przestrzeni okna
	glViewport(0,0,w,h);
	//Zapamiêtanie nowych wymiarów okna dla poprawnego wyliczania macierzy rzutowania
	windowWidth=w;
	windowHeight=h;
}

//Procedura inicjuj¹ca biblotekê glut
void initGLUT(int *argc, char** argv) {
	glutInit(argc,argv); //Zainicjuj bibliotekê GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);													//Alokuj bufory kolorów (podwójne buforowanie) i bufor kolorów
	
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - windowWidth) / 2, (glutGet(GLUT_SCREEN_HEIGHT) - windowHeight) / 2);	//Wska¿ pocz¹tkow¹ pozycjê okna
	glutInitWindowSize(windowWidth,windowHeight); //Wska¿ pocz¹tkowy rozmiar okna
	glutCreateWindow("SuperChessMasterTurboArcadeEdition X"); //Utwórz okno i nadaj mu tytu³
	
	glutReshapeFunc(changeSize);																				//Zarejestruj procedurê changeSize jako procedurê obs³uguj¹ca zmianê rozmiaru okna
	glutDisplayFunc(displayFrame);																				//Zarejestruj procedurê displayFrame jako procedurê obs³uguj¹ca odœwierzanie okna
	glutIdleFunc(nextFrame);																					//Zarejestruj procedurê nextFrame jako procedurê wywo³ywan¹ najczêœciêj jak siê da (animacja)
}


//Procedura inicjuj¹ca bibliotekê glew
void initGLEW() {
	GLenum err=glewInit();
	if (GLEW_OK!=err) {
		fprintf(stderr,"%s\n",glewGetErrorString(err));
		exit(1);
	}
	
}


//Wczytuje vertex shader i fragment shader i ³¹czy je w program cieniuj¹cy
void setupShaders() {
	shader = new ShaderProgram("vshader.txt", NULL, "fshader.txt");
}

//procedura inicjuj¹ca ró¿ne sprawy zwi¹zane z rysowaniem w OpenGL
void initOpenGL() {
	setupShaders();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE_ARB);
}

//Zwolnij pamiêæ karty graficznej z shaderów i programu cieniuj¹cego
void cleanShaders() {
	delete shader;
}


void keyDown(int c, int x, int y){
	int temp;
	switch (c) {
	case GLUT_KEY_UP:
		speedX = 100;
		break;
	case GLUT_KEY_DOWN:
		speedX = -100;
		break;
	case GLUT_KEY_LEFT:
		speedY = 100;
		break;
	case GLUT_KEY_RIGHT:
		speedY = -100;
		break;
	case GLUT_KEY_F1:
		App->pieces[2]->setTarget(&App->chessBoard->squares[2][test]);
		test++;
		break;
	case GLUT_KEY_F2:
		temp = rand() % App->pieces[2]->possibleMoves.size();// 2;//rand() % 32;
		App->pieces[2]->setTarget(&App->pieces[2]->possibleMoves[temp]);
		break;
	case GLUT_KEY_F3:
		App->pieces[2]->setTarget(&App->chessBoard->squares[2][1]);
		test = 2;
		break;
	case GLUT_KEY_F4:
		App->pieces[2]->setTarget(&App->chessBoard->squares[rand() % 8][(rand() % 4) + 2]);
		break;
	case GLUT_KEY_F5:
		int x, y;
		do {
			x = rand() % 8;
			y = rand() % 8;
		} while (App->chessBoard->squares[x][y].piece != 0);
		App->pieces[rand() % 32]->setTarget(&App->chessBoard->squares[x][y]);
		break;
	case GLUT_KEY_F6:
		system("cls");
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++)
				cout << App->chessBoard->squares[i][j].piece << "\t";
			cout << endl;
		}
		break;
	}

}


void keyUp(int c, int x, int y){
	if (c == GLUT_KEY_UP || c == GLUT_KEY_DOWN)
		speedX = 0;
	if (c == GLUT_KEY_RIGHT || c == GLUT_KEY_LEFT)
		speedY = 0;
}

int main(int argc, char** argv) {
	srand(time(NULL));
	initGLUT(&argc,argv);
	initGLEW();
	initOpenGL(); 
	
	glutSpecialFunc(keyDown);
	glutSpecialUpFunc(keyUp);

	App = new Application(shader);

	glutMainLoop();
	
	cleanShaders();
	return 0;
}
