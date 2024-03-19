#pragma once

#include <math.h>

class Vec2f {

public:

	float x, y;

	Vec2f() {
		x = 0;
		y = 0;
	}

	Vec2f(float x, float y) {
		this->x = x;
		this->y = y;
	}

	float lengthSqr() {
		return x * x + y * y;
	}

	float length() {
		return sqrt(this->lengthSqr());
	}

	void normalize() {
		float mod = 1 / this->length();
		x *= mod;
		y *= mod;
	}


	Vec2f operator-(Vec2f& other) {
		Vec2f nv = Vec2f(
			x - other.x,
			y - other.y
		);
		return nv;
	}

	Vec2f operator+(Vec2f& other) {
		Vec2f nv = Vec2f(
			x + other.x,
			y + other.y
		);
		return nv;
	}

	Vec2f operator*(float mul) {
		Vec2f nv = Vec2f(
			x * mul,
			y * mul
		);
		return nv;
	}

	Vec2f operator/(float mul) {
		Vec2f nv = Vec2f(
			x / mul,
			y / mul
		);
		return nv;
	}

	Vec2f operator*(Vec2f& other) {
		Vec2f nv = Vec2f(
			x * other.x,
			y * other.y
		);
		return nv;
	}

	void reverse() {
		x = -x;
		y = -y;
	}

};