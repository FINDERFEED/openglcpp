#pragma once

#include "Spline.h"

class BezierSpline : public Spline {

public: 

	virtual void draw(Matrix4f transform, VertexBuffer* lineBuffer, float r, float g, float b, float a);


};