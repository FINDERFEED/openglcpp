#pragma once

#include "Spline.h"

class BSpline : public Spline {

public:

	int degree;

	BSpline(int degree) : Spline() {
		this->degree = degree;
	}

	virtual void draw(Matrix4f transform, VertexBuffer* lineBuffer,float r,float g,float b,float a);
	

	Vec3f calculateDeBor(float p);
};