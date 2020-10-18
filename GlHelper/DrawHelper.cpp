#include "DrawHelper.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

void DrawBox(double x1, double y1, double z1, double x2, double y2, double z2) {
	DrawBox(x1, y1, z1, x2, y2, z2, 1.0, 1.0, 1.0);
}

void DrawBox(double x1, double y1, double z1, double x2, double y2, double z2, float R, float G, float B) {
	glPushMatrix();

	glBegin(GL_POLYGON);
	glColor3f(R, G, B);
	glVertex3d(x1, y1, z1);
	glVertex3d(x1, y2, z1);
	glVertex3d(x1, y2, z2);
	glVertex3d(x1, y1, z2);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(R, G, B);
	glVertex3d(x2, y1, z1);
	glVertex3d(x2, y2, z1);
	glVertex3d(x2, y2, z2);
	glVertex3d(x2, y1, z2);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(R, G, B);
	glVertex3d(x1, y1, z1);
	glVertex3d(x2, y1, z1);
	glVertex3d(x2, y1, z2);
	glVertex3d(x1, y1, z2);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(R, G, B);
	glVertex3d(x1, y2, z1);
	glVertex3d(x2, y2, z1);
	glVertex3d(x2, y2, z2);
	glVertex3d(x1, y2, z2);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(R, G, B);
	glVertex3d(x1, y1, z1);
	glVertex3d(x2, y1, z1);
	glVertex3d(x2, y2, z1);
	glVertex3d(x1, y2, z1);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(R, G, B);
	glVertex3d(x1, y1, z2);
	glVertex3d(x2, y1, z2);
	glVertex3d(x2, y2, z2);
	glVertex3d(x1, y2, z2);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0, 0, 0);
	glVertex3d(x1, y1, z1);
	glVertex3d(x1, y2, z1);
	glVertex3d(x1, y2, z2);
	glVertex3d(x1, y1, z2);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0, 0, 0);
	glVertex3d(x2, y1, z1);
	glVertex3d(x2, y2, z1);
	glVertex3d(x2, y2, z2);
	glVertex3d(x2, y1, z2);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glColor3f(0, 0, 0);
	glVertex3d(x1, y1, z1);
	glVertex3d(x2, y1, z1);
	glVertex3d(x2, y1, z2);
	glVertex3d(x1, y1, z2);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glColor3f(0, 0, 0);
	glVertex3d(x1, y2, z1);
	glVertex3d(x2, y2, z1);
	glVertex3d(x2, y2, z2);
	glVertex3d(x1, y2, z2);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glColor3f(0, 0, 0);
	glVertex3d(x1, y1, z1);
	glVertex3d(x2, y1, z1);
	glVertex3d(x2, y2, z1);
	glVertex3d(x1, y2, z1);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glColor3f(0, 0, 0);
	glVertex3d(x1, y1, z2);
	glVertex3d(x2, y1, z2);
	glVertex3d(x2, y2, z2);
	glVertex3d(x1, y2, z2);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glColor3f(0, 0, 0);
	glVertex3d(x1, y1, z1);
	glVertex3d(x1, y1, z2);
	glVertex3d(x1, y2, z2);
	glVertex3d(x1, y2, z1);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glColor3f(0, 0, 0);
	glVertex3d(x2, y1, z1);
	glVertex3d(x2, y1, z2);
	glVertex3d(x2, y2, z2);
	glVertex3d(x2, y2, z1);
	glEnd();

	glPopMatrix();
}

/*
void DrawThickLine(double x1, double y1, double z1, double x2, double y2, double z2, double thickness){
	glPushMatrix();
	{
		glTranslated(x1, y1, z1);
		glRotated(-atan2(y2 - y1, z2 - z1), 1.0, 0.0, 0.0);
		glRotated(atan2(x2 - x1, sqrt(pow(z2 - z1, 2.0) + pow(y2 - y1, 2.0))), 0.0, 1.0, 0.0);
		DrawBox(-1.0, -1.0, 0.0, 1.0, 1.0, sqrt(pow(z2 - z1, 2.0) + pow(y2 - y1, 2.0) + pow(x2 - x1, 2.0)));
	}
	glPopMatrix();
}
*/

void glEigenRotate4f(Eigen::Vector4f rot) {
	glRotatef(rot[0], rot[1], rot[2], rot[3]);
}

void glEigenRotate3f(Eigen::Vector3f rot) {
	glRotatef(rot[0], 1.0, 0.0, 0.0);
	glRotatef(rot[1], 0.0, 1.0, 0.0);
	glRotatef(rot[2], 0.0, 0.0, 1.0);
}

void glEigenTranslatef(Eigen::Vector3f vec) {
	glTranslatef(vec[0], vec[1], vec[2]);
}

void glEigenVertex3f(Eigen::Vector3f vec) {
	glVertex3f(vec[0], vec[1], vec[2]);
}

void glEigenRotate4d(Eigen::Vector4d rot) {
	glRotated(rot[0], rot[1], rot[2], rot[3]);
}

void glEigenRotate3d(Eigen::Vector3d rot) {
	glRotated(rot[2], 0.0, 0.0, 1.0);
	glRotated(rot[0], 1.0, 0.0, 0.0);
	glRotated(rot[1], 0.0, 1.0, 0.0);
}

void glEigenTranslated(Eigen::Vector3d vec) {
	glTranslated(vec[0], vec[1], vec[2]);
}

void glEigenVertex3d(Eigen::Vector3d vec) {
	glVertex3d(vec[0], vec[1], vec[2]);
}

void glEigenColor3d(Eigen::Vector3d vec) {
	glColor3d(vec[0], vec[1], vec[2]);
}