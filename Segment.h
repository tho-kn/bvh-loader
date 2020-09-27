#pragma once
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Sparse>
#include <eigen3/Eigen/Geometry>
#include <iostream>
#include <vector>
#include <memory>

class Segment {

public:
	enum Channel{
		Xposition,
		Yposition,
		Zposition,
		Xrotation,
		Yrotation,
		Zrotation,
	};

	Segment(const std::string name);

	std::string getName();

	void setColor(const Eigen::Vector3d& color);
	void setOffset(const Eigen::Vector3d& offset);
	void rename(const std::string name);

	void translate(const Eigen::Vector3d& trans);
	void rotate(const Eigen::Vector3d& rot);

	void addChannel(const Channel ch);
	void applyChannel(const double val, const int num);
	int numChannels();

	void addSub(const std::unique_ptr<Segment> seg);
	int numSub();
	Segment *getSeg(int num);
	void resetSub();

	void draw();
	void print();

private:
	std::string name;

	Eigen::Vector3d offset;
	Eigen::Vector3d trans;
	Eigen::Vector3d rot;
	Eigen::Vector3d color;

	void translateX(const double val);
	void translateY(const double val);
	void translateZ(const double val);

	void rotateX(const double val);
	void rotateY(const double val);
	void rotateZ(const double val);

	std::vector<Channel> channels;

	std::vector<std::unique_ptr<Segment>> subSeg;
};