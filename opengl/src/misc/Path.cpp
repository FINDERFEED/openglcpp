#pragma once

#include "../Util.cpp";
#include "../math/Vec3f.cpp"
#include <vector>
#include <math.h>

class Path {

public:

	std::vector<Vec3f> points;

	Path() {

	}

	Path* addPoint(Vec3f point) {
		this->points.push_back(point);
		return this;
	}

	Vec3f getPoint(float percent,Vec3f* direction) {
		
		float globalPercent = percent * points.size();
		int gl = (int)globalPercent;
		int index = gl % points.size();
		int indexNext = (gl + 1) % points.size();
		float localPercent = globalPercent - gl;


		Vec3f point1 = points.at(index);
		Vec3f point2 = points.at(indexNext);
		Vec3f between = point2 - point1;

		Vec3f b1 = between * localPercent;
		Vec3f fpoint = point1 + b1;
		b1.normalize();
		direction->x = b1.x;
		direction->y = b1.y;
		direction->z = b1.z;

		return fpoint;
	}

};