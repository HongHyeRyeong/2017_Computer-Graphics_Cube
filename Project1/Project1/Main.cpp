#include <GL/glut.h>
#include <iostream>
#include "windows.h"
#include "Background.h"
#include "Stage.h"



GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void TimerFunction(int value);
void Keyboard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);

float traX, traY, traZ;
float rotX{ 30 }, rotY, rotZ;

bool left_button{ false };
int mouseX, mouseY;

Background* bg;
Stage* stage;

DWORD g_startTime = NULL;
DWORD g_bulletTime = NULL;

int selectNum;
int saveNum1=  0;
int saveNum2 = 1;
int saveNum3 = 2;


void main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(500, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("CubeSpace");
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutReshapeFunc(Reshape);
	glutTimerFunc(100, TimerFunction, 1);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND); // 블렌딩 투명
	
	// 조명
	
	GLfloat DiffuseLight[] = { 1.0f, 1.0f, 1.0f, 0.0f };
	GLfloat lightPos1[] = { -300, 300 , 300 ,0 }; //뒤
	GLfloat lightPos2[] = { -300, 300 , -300 ,0 }; //앞


	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	//glLightfv(GL_LIGHT0, GL_DIFFUSE, DiffuseLight);
	//glLightfv(GL_LIGHT0, GL_POSITION, lightPos1);

	glLightfv(GL_LIGHT1, GL_DIFFUSE, DiffuseLight);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos2);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glMateriali(GL_FRONT, GL_SHININESS, 128);
	


	bg = new Background();
	stage = new Stage(1);
	g_startTime = timeGetTime();

	glutMainLoop();

	delete bg;
}

GLvoid drawScene(GLvoid)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(traX, traY, traZ);
	glRotatef(rotX, 1.0f, 0.0f, 0.0f);
	glRotatef(rotY, 0.0f, 1.0f, 0.0f);
	glRotatef(rotZ, 0.0f, 0.0f, 1.0f);

	DWORD currTime = timeGetTime();
	DWORD elapsedTime = currTime - g_startTime;
	g_startTime = currTime;

	bg->updateBackground((float)elapsedTime);
	stage->updateStage((float)elapsedTime);

	bg->drawBackground();
	stage->drawStage();

	glutSwapBuffers();
}

void TimerFunction(int value)
{
	glutPostRedisplay();
	glutTimerFunc(100, TimerFunction, 1);
}

void Keyboard(unsigned char key, int x, int y)
{
	if (key == 'q')
		exit(0);
	else if (key == 'w') {
		if (selectNum > 0) {
			selectNum--;
			stage->setSelectCube(selectNum);
			PlaySound(TEXT(SOUND_FILE_NAME_CUBE), NULL, SND_ASYNC);

		}
	}
	else if (key == 'e') {
		if (selectNum < stage->getNumCube() - 1) {
			selectNum++;
			stage->setSelectCube(selectNum);
			PlaySound(TEXT(SOUND_FILE_NAME_CUBE), NULL,SND_ASYNC);

		}
	}
	else if (key == 'r')
	{
		stage->Keyboard(key);

		// 

		PlaySound(TEXT(SOUND_FILE_NAME_CUBE), NULL, SND_ASYNC);

		/*
		if (saveNum1 == selectNum)
		{
			stage->savecube1(1);
			stage->setSaveCube1(saveNum1);
		}
		if (saveNum2 == selectNum)
		{
			stage->savecube2(1);
			stage->setSaveCube2(saveNum2);
		}
	
		if (saveNum3 == selectNum)
		{
			stage->savecube3(1);
			stage->setSaveCube3(saveNum3);
		}*/
		
	}
	else if (key == 't')
	{
		stage->Keyboard(key);
		//stage->setCubeType(0);
		PlaySound(TEXT(SOUND_FILE_NAME_CUBE), NULL, SND_ASYNC);
	}
	else if (key == 'a' || key == 'z' || key == 's' || key == 'x' || key == 'd' || key == 'c' ||
		key == 'f' || key == 'g' || key == 'h')
	{
		stage->Keyboard(key);
		PlaySound(TEXT(SOUND_FILE_NAME_CUBE), NULL, SND_ASYNC);

	}
	else if (key == 'u') {
		bg->setGrid(true);
		PlaySound(TEXT(SOUND_FILE_NAME_CUBE), NULL, SND_ASYNC);
	}
	else if (key == 'U') {
		bg->setGrid(false);
		PlaySound(TEXT(SOUND_FILE_NAME_CUBE), NULL, SND_ASYNC);

	}
	else if (key == 'i') {
		rotX = 30;
		rotY = 0;
	}
}

void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		left_button = true;
		mouseX = x;
		mouseY = y;
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		left_button = false;
}

void Motion(int x, int y)
{
	if (left_button == true) {

		if (mouseX > x)
			rotY--;
		else if (mouseX < x)
			rotY++;

		if (mouseY > y)
			rotX--;
		else if (mouseY < y)
			rotX++;

		mouseX = x;
		mouseY = y;
	}
}

GLvoid Reshape(int w, int h)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(60.0f, w / h, 1.0, 4000.0);
	glTranslatef(0.0, -20.0, -230.0);

	glMatrixMode(GL_MODELVIEW);
	gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0);
}