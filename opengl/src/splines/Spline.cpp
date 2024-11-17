#include "Spline.h"


Spline::Spline() {
	this->controlPoints = new std::vector<Vec3f*>();
}

Spline::~Spline() {
	delete this->controlPoints;
}


void Spline::addPoint(Vec3f* v) {
	this->controlPoints->push_back(v);
}

void Spline::draw(Matrix4f transform,VertexBuffer* lineBuffer,float r,float g,float b,float a) {

}

