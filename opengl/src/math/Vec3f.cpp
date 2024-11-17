#pragma once

#include <math.h>

class Vec3f {

public:

	float x, y, z;

	Vec3f() {
		x = 0;
		y = 0;
		z = 0;
	}

	Vec3f(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	float lengthSqr() {
		return x * x + y * y + z * z;
	}

	float length() {
		return sqrt(this->lengthSqr());
	}

	void normalize() {
		float mod = 1 / this->length();
		x *= mod;
		y *= mod;
		z *= mod;
	}

	Vec3f cross(Vec3f other) {
		Vec3f newVec = Vec3f();
		newVec.x = fma(y, other.z, -z * other.y);
		newVec.y = fma(z, other.x, -x * other.z);
		newVec.z = fma(x, other.y, -y * other.x);
		return newVec;
	}

	Vec3f operator-(Vec3f& other) {
		Vec3f nv = Vec3f(
			x - other.x,
			y - other.y,
			z - other.z
		);
		return nv;
	}

	Vec3f operator+(Vec3f& other) {
		Vec3f nv = Vec3f(
			x + other.x,
			y + other.y,
			z + other.z
		);
		return nv;
	}

	Vec3f operator*(float mul) {
		Vec3f nv = Vec3f(
			x * mul,
			y * mul,
			z * mul
		);
		return nv;
	}

	Vec3f operator/(float mul) {
		Vec3f nv = Vec3f(
			x / mul,
			y / mul,
			z / mul
		);
		return nv;
	}

	Vec3f operator*(Vec3f& other) {
		Vec3f nv = Vec3f(
			x * other.x,
			y * other.y,
			z * other.z
		);
		return nv;
	}

	void operator +=(Vec3f& other) {

		this->x += other.x;
		this->y += other.y;
		this->z += other.z;

	}

	void reverse() {
		x = -x;
		y = -y;
		z = -z;
	}

};