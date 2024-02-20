#pragma once

#include "Vec3f.cpp"

class Vec4f {

public:
	float x, y, z, w;

	Vec4f(const Vec4f& v) {
		this->x = v.x;
		this->y = v.y;
		this->z = v.z;
		this->w = v.w;
	}

	Vec4f() {
		x = 0;
		y = 0;
		z = 0;
		w = 0;
	}

	Vec4f(float x, float y, float z, float w) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	Vec3f toVec3f() {
		return Vec3f(x, y, z);
	}

};