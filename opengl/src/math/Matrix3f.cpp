#pragma once
#include <math.h>
#include "Vec3f.cpp"

class Matrix3f {

public:

	float m11, m12, m13;
	float m21, m22, m23;
	float m31, m32, m33;

	Matrix3f() {
		this->identity();
	}


	void multiply(Matrix3f& other) {

		float nm11 = m11 * other.m11 + m12 * other.m21 + m13 * other.m31;
		float nm12 = m11 * other.m12 + m12 * other.m22 + m13 * other.m32;
		float nm13 = m11 * other.m13 + m12 * other.m23 + m13 * other.m33;

		float nm21 = m21 * other.m11 + m22 * other.m21 + m23 * other.m31;
		float nm22 = m21 * other.m12 + m22 * other.m22 + m23 * other.m32;
		float nm23 = m21 * other.m13 + m22 * other.m23 + m23 * other.m33;

		float nm31 = m31 * other.m11 + m32 * other.m21 + m33 * other.m31;
		float nm32 = m31 * other.m12 + m32 * other.m22 + m33 * other.m32;
		float nm33 = m31 * other.m13 + m32 * other.m23 + m33 * other.m33;

		m11 = nm11;
		m12 = nm12;
		m13 = nm13;
		m21 = nm21;
		m22 = nm22;
		m23 = nm23;
		m31 = nm31;
		m32 = nm32;
		m33 = nm33;
	}

	void rotateYDegrees(float angle) {
		Matrix3f mat = Matrix3f();
		angle = (angle / 360) * 3.1415 * 2;
		float co = cos(angle);
		float si = sin(angle);
		mat.m11 = co;
		mat.m13 = si;
		mat.m31 = -si;
		mat.m33 = co;
		this->multiply(mat);
	}

	void rotateXDegrees(float angle) {
		Matrix3f mat = Matrix3f();
		angle = (angle / 360) * 3.1415 * 2;
		float co = cos(angle);
		float si = sin(angle);
		mat.m22 = co;
		mat.m23 = -si;
		mat.m32 = si;
		mat.m33 = co;
		this->multiply(mat);
	}

	void rotateZDegrees(float angle) {
		Matrix3f mat = Matrix3f();
		angle = (angle / 360) * 3.1415 * 2;
		float co = cos(angle);
		float si = sin(angle);
		mat.m11 = co;
		mat.m12 = -si;
		mat.m21 = si;
		mat.m22 = co;
		this->multiply(mat);
	}

	void scale(float x, float y, float z) {
		Matrix3f m = Matrix3f();
		m.m11 = x;
		m.m22 = y;
		m.m33 = z;
		this->multiply(m);
	}

	void identity() {
		m11 = 1;
		m22 = 1;
		m33 = 1;
		m12 = 0;
		m13 = 0;
		m21 = 0;
		m23 = 0;
		m31 = 0;
		m32 = 0;
	}

	void transform(Vec3f& v) {
		float x = m11 * v.x + m12 * v.y + m13 * v.z;
		float y = m21 * v.x + m22 * v.y + m23 * v.z;
		float z = m31 * v.x + m32 * v.y + m33 * v.z;
		v.x = x;
		v.y = y;
		v.z = z;
	}

};