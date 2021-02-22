#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <algorithm>
#include "GlHelper/DrawHelper.h"
#include "BVHViewer.h"
#define PI 3.14159265

BVHViewer *bvh;
int frame = 0;
double accel = 10.0;

unsigned timeStep = 30;
bool play = false;

GLdouble rotMatrix[16] =
{
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
};

/* set global variables */
int width, height;

bool leftButton = false;
GLfloat mousePosX, mousePosY;

/* vectors that makes the rotation and translation of the cube */
Eigen::Vector3f eye(-300.0f, 200.0f, -300.0f);
Eigen::Vector3f ori(1.0f, 0.0f, 1.0f);
Eigen::Vector2f oriAngle(3.0 * PI / 4.0, PI / 2.0);
Eigen::Vector3f rightVec(0.0f, 0.0f, 1.0f);
Eigen::Vector3f up(0.0f, 1.0f, 0.0f);

void loadGlobalCoord()
{
	glLoadIdentity();
	gluLookAt(eye[0], eye[1], eye[2], eye[0]+ori[0], eye[1]+ori[1], eye[2]+ori[2], up[0], up[1], up[2]);
	glMultMatrixd(rotMatrix);
}

//------------------------------------------------------------------------
// Moves the screen based on mouse pressed button
//------------------------------------------------------------------------

void glutMotion(int x, int y)
{
	if (leftButton) {
		float dx = x - mousePosX;
		float dy = y - mousePosY;

		mousePosX = x;
		mousePosY = y;

		oriAngle[0] += dx / width;
		oriAngle[1] -= dy / width;

		ori[0] = sin(oriAngle[0])*sin(oriAngle[1]);
		ori[1] = -cos(oriAngle[1]);
		ori[2] = -cos(oriAngle[0])*sin(oriAngle[1]);

		ori.normalize();
		rightVec = ori.cross(up).normalized();
		
		loadGlobalCoord();
	}
	return;
}

//------------------------------------------------------------------------
// Function that handles mouse input
//------------------------------------------------------------------------
void glutMouse(int button, int state, int x, int y)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
		{
			mousePosX = x;
			mousePosY = y;
			leftButton = true;
		}
		else if (state == GLUT_UP)
		{
			leftButton = false;
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN){
			play = !play;
		}
		break;
	case 3:break;
	default:break;
	}
	return;
}

void DrawGridPlane() {
	glBegin(GL_LINES);
	glColor3f(0, 1, 0);
	for (int i = -1000; i < 1001; i+=100){
		glVertex3f(i, 0, -1000);
		glVertex3f(i, 0, 1000);
		glVertex3f(1000, 0, i);
		glVertex3f(-1000, 0, i);
	}
	glEnd();
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	loadGlobalCoord();

	//glRotatef(45, -1, 0, 0);
	glPushMatrix();
	DrawGridPlane();
	bvh->draw();
	
	glPopMatrix();

	glutSwapBuffers();
}

void resize(int w, int h) {
	width = w;
	height = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)w / (GLfloat)h, .1f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'w':
		eye += ori * accel;
		break;
	case 's':
		eye -= ori * accel;
		break;
	case 'a':
		eye -= rightVec * accel;
		break;
	case 'd':
		eye += rightVec * accel;
		break;
	case 'q':
		if(!play){
			frame -= 1;
			if(frame < 0) frame = bvh->frameSize() - 1;
			bvh->loadFrame(frame);
		}
		break;
	case 'e':
		if(!play){
			frame += 1;
			if(frame >= bvh->frameSize()) frame = 0;
			bvh->loadFrame(frame);
		}
		break;
	case 27:
		free(bvh);
		exit(0);
		break;
	default:
		break;
	}
}

void Timer(int unused)
{
	if (play){
		frame += 1;
		if(frame >= bvh->frameSize()) frame = 0;
		bvh->loadFrame(frame);
	}

	/* call the display callback and forces the current window to be displayed */
	glutPostRedisplay();
	glutTimerFunc(timeStep, Timer, 0);
}

int main(int argc, char** argv) {
	if (argc < 2){
		printf("Usage: %s Filename", argv[0]);
		return 0;
	}

	{
		BVHReader reader = BVHReader(argv[1]);
		if (!reader.loadFile()){
			std::cout << "Failed to load .bvh file" << std::endl;
			return 1;
		}

		timeStep = reader.getFrameTime() * 1000;
		std::cout << "Loaded " << argv[1] << " successfully" << std::endl;
		bvh = new BVHViewer(reader.getRoots(), reader.getMotion(), reader.getChannels());
	}

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(50, 0);
	glutCreateWindow("BVH Viewer");

	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutTimerFunc(timeStep, Timer, 0);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(glutMouse);
	glutMotionFunc(glutMotion);

	glutMainLoop();

	return 0;
}