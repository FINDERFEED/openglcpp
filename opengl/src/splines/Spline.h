#pragma once


#include <vector>
#include "../math/Vec3f.cpp"
#include "../math/Matrix4f.cpp"
#include "../batching/VertexBuffer.cpp"

class Spline {

public:

	std::vector<Vec3f*>* controlPoints;

	Spline();

	~Spline();

	void addPoint(Vec3f* v);


	virtual void draw(Matrix4f transformation,VertexBuffer* lineBuffer,float r,float g,float b,float a);

};