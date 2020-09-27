#include "Segment.h"
#include "GlHelper/DrawHelper.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iterator>
#include <cmath>

Segment::Segment(const std::string name = ""):
	offset(Eigen::Vector3d(0, 0, 0)),trans(Eigen::Vector3d(0, 0, 0)), rot(Eigen::Vector3d(0, 0, 0)), color(Eigen::Vector3d(1, 0, 0)), name(name) {}

void Segment::setOffset(const Eigen::Vector3d& offset){
	this->offset = offset;
}

void Segment::translate(const Eigen::Vector3d& trans){
	this->trans = trans;
}

void Segment::translateX(const double val) { this->trans[0] = val; }
void Segment::translateY(const double val) { this->trans[1] = val; }
void Segment::translateZ(const double val) { this->trans[2] = val; }

void Segment::rotate(const Eigen::Vector3d& rot){
	this->rot = rot;
}

void Segment::rotateX(const double val) { this->rot[0] = val; }
void Segment::rotateY(const double val) { this->rot[1] = val; }
void Segment::rotateZ(const double val) { this->rot[2] = val; }

void Segment::setColor(const Eigen::Vector3d& color){
	this->color = color;
}

std::string Segment::getName(){
	return this->name;
}

void Segment::rename(const std::string name){
	this->name = name;
}

void Segment::addSub(std::unique_ptr<Segment> seg){
	this->subSeg.push_back(std::move(seg));
}

int Segment::numSub(){
	return this->subSeg.size();
}

Segment *Segment::getSeg(int num){
	if(num >= this->numSub()){
		return nullptr;
	}
	return this->subSeg[num].get();
}

void Segment::resetSub(){
	this->subSeg = std::vector<std::unique_ptr<Segment>>();
}

void Segment::draw() {
	glPushMatrix();
	{
		//DrawThickLine(0.0, 0.0, 0.0, this->offset[0], this->offset[1], this->offset[2], 1);
		glPushMatrix();
		{
			for(int i = 0; i < 4; i++){
				glColor3d(1.0, 1.0, 1.0);
				glEigenTranslated(this->offset / 5.0);
				glutSolidSphere(1, 4, 4);
			}
		}
		glPopMatrix();

		glBegin(GL_LINE);
		glEigenColor3d(this->color);
		glVertex3f(0, 0, 0);
		glEigenVertex3d(this->offset);
		glEnd();
	
		glEigenTranslated(this->offset);
		if(trans.norm() > 0.01) glEigenTranslated(this->trans);
		glutSolidSphere(2, 4, 4);
		
		glEigenRotate3d(this->rot);

		auto iter = this->subSeg.begin();

		for(; iter != this->subSeg.end(); ++iter){
			(*iter)->draw();
		}
	}
	glPopMatrix();
}

void Segment::addChannel(const Channel ch){
	this->channels.push_back(ch);
}

void Segment::applyChannel(const double val, const int num){
	Channel ch = this->channels[num];
	switch (ch){
		case Xposition:
			this->translateX(val);
			break;
		case Yposition:
			this->translateY(val);
			break;
		case Zposition:
			this->translateZ(val);
			break;
		case Xrotation:
			this->rotateX(val);
			break;
		case Yrotation:
			this->rotateY(val);
			break;
		case Zrotation:
			this->rotateZ(val);
			break;
	}
}

int Segment::numChannels(){
	return this->channels.size();
}

void Segment::print(){
	using namespace std;
	
	cout << "name: " << this->name << endl;
	cout << "offset: " << this->offset << endl;
	cout << "translation: " << this->trans << endl;
	cout << "rotation: " << this->rot << endl;
	cout << "sub: ";
	for (auto i = this->subSeg.begin(); i < this->subSeg.end(); i++){
		cout << (*i)->name << " ";
	}
}