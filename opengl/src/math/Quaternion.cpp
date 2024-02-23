#pragma once
#include "../Util.cpp"
#include <math.h>
#include "Vec3f.cpp"

class Quaternion {


public:
	float x;
	float y;
	float z;
	float w;


	Quaternion() {
		w = 1.0;
		x = 0;
		y = 0;
		z = 0;
	}

	Quaternion(float x, float y, float z, float angle) {
		
		angle = Util::toRadians(angle);
		float si = sin(angle / 2);
		float co = cos(angle / 2);
		this->x = x * si;
		this->y = y * si;
		this->z = z * si;
		this->w = co;
	}

	Quaternion(Vec3f& axis, float angle) : Quaternion(axis.x,axis.y,axis.z,angle) {}

	void conjugate() {
		x = -x;
		y = -y;
		z = -z;
	}

	float len() {
		return sqrt(this->lenSqr());
	}
	float lenSqr() {
		return w * w + x * x + y * y + z * z;
	}

	void multiply(Quaternion& other) {
		float rw = w * other.w - x * other.x - y * other.y - z * other.z;
		float rx = w * other.x + x * other.w + y * other.z - z * other.y;
		float ry = w * other.y - x * other.z + y * other.w + z * other.x;
		float rz = w * other.z + x * other.y - y * other.x + z * other.w;
		this->x = rx;
		this->y = ry;
		this->z = rz;
		this->w = rw;
	}

};