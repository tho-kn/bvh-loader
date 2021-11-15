#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <chrono>
#include "GlHelper/DrawHelper.h"
#include "BVHViewer.h"
#define PI 3.14159265

vector<BVHViewer*> bvhs;
int frame = 0;
int frame_size = 0;
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

void DrawStringOnScreen(float _x, float _y, const std::string& _s,bool _bigFont,const Eigen::Vector3d& color)
{
    glColor3f(color[0],color[1],color[2]);
	
    // draws text on the screen
    GLint oldMode;
    glGetIntegerv(GL_MATRIX_MODE, &oldMode);
    glMatrixMode(GL_PROJECTION);

    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, 1.0, 0.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glRasterPos2f(_x, _y);
    unsigned int length = _s.length();
    for (unsigned int c = 0; c < length; c++) {
        if (_bigFont)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, _s.at(c) );
        else
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, _s.at(c) );
    }  
    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(oldMode);
}

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

void DrawPlane() {
	glBegin(GL_QUADS);
	glColor4f(1.0, 1.0, 1.0, 0.5);
	glVertex3f(-1000, 0, -1000);
	glVertex3f(1000, 0, -1000);
	glVertex3f(1000, 0, 1000);
	glVertex3f(-1000, 0, 1000);
	glEnd();
}

void DrawGridPlane() {
	glBegin(GL_LINES);
	glColor3f(0.5, 0.5, 0.5);
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
	//DrawPlane();
	DrawGridPlane();
	for (auto bvh: bvhs)
		bvh->draw();
	
	glPopMatrix();
	DrawStringOnScreen(0.8, 0.9, std::to_string(frame) + "/" + std::to_string(frame_size), true, Eigen::Vector3d::Zero());

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
			if(frame < 0) frame = frame_size - 1;
			for (auto bvh: bvhs)
				bvh->loadFrame(frame);
		}
		break;
	case 'e':
		if(!play){
			frame += 1;
			if(frame >= frame_size) frame = 0;
			for (auto bvh: bvhs)
				bvh->loadFrame(frame);
		}
		break;
	case 27:
		for (auto bvh: bvhs)
			free(bvh);
		exit(0);
		break;
	default:
		break;
	}
}

void Timer(int unused)
{
	
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	if (play){
		frame += 1;
		if(frame >= frame_size) frame = 0;
		for (auto bvh: bvhs)
			bvh->loadFrame(frame);
	}
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	double elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1000.;

	glutTimerFunc(timeStep - elapsed, Timer, 0);
	
	/* call the display callback and forces the current window to be displayed */
	glutPostRedisplay();
}

int main(int argc, char** argv) {
	if (argc < 2){
		printf("Usage: %s Filename", argv[0]);
		return 0;
	}

	for (int i = 1; i < argc; i++){
		BVHReader reader = BVHReader(argv[i]);
		if (!reader.loadFile()){
			std::cout << "Failed to load .bvh file" << std::endl;
			return 1;
		}

		timeStep = reader.getFrameTime() * 1000.0;
		std::cout << "Loaded " << argv[i] << " successfully" << std::endl;
		auto bvh = new BVHViewer(move(reader.getRoots()), reader.getMotion(), reader.getChannels());
		bvhs.push_back(bvh);

		if (bvh->frameSize() > frame_size) frame_size = bvh->frameSize();
	}

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(50, 0);
	glutCreateWindow("BVH Viewer");

  	glClearColor(135.0/255.0, 206.0/255.0, 235.0/255.0, 1.0f);

	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutTimerFunc(timeStep, Timer, 0);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(glutMouse);
	glutMotionFunc(glutMotion);

	glutMainLoop();

	return 0;
}