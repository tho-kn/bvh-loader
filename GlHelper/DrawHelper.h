#pragma once
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Sparse>
#include <eigen3/Eigen/Geometry>

void DrawBox(double x1, double y1, double z1, double x2, double y2, double z2);
void DrawBox(double x1, double y1, double z1, double x2, double y2, double z2, float R, float G, float B);
void DrawThickLine(double x1, double y1, double z1, double x2, double y2, double z2, double thickness);

void glEigenRotate4f(Eigen::Vector4f);
void glEigenRotate3f(Eigen::Vector3f);
void glEigenTranslatef(Eigen::Vector3f);
void glEigenVertex3f(Eigen::Vector3f);

void glEigenRotate4d(Eigen::Vector4d);
void glEigenRotate3d(Eigen::Vector3d);
void glEigenTranslated(Eigen::Vector3d);
void glEigenVertex3d(Eigen::Vector3d);

void glEigenColor3d(Eigen::Vector3d);