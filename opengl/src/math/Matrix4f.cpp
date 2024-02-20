#pragma once
#include "Vec4f.cpp"
#include <math.h>
#include "../buffers/FloatBuffer.cpp"

class Matrix4f {

public:

	float m11, m12, m13, m14;
	float m21, m22, m23, m24;
	float m31, m32, m33, m34;
	float m41, m42, m43, m44;

	Matrix4f() {
		this->identity();
	}

	void identity() {
		m11 = 1; m22 = 1; m33 = 1; m44 = 1;
		m12 = 0; m13 = 0; m14 = 0; m21 = 0;
		m23 = 0; m24 = 0; m31 = 0; m32 = 0;
		m34 = 0; m41 = 0; m42 = 0; m43 = 0;
	}

	void multiply(Matrix4f& other) {
		float cm11 = m11 * other.m11 + m12 * other.m21 + m13 * other.m31 + m14 * other.m41;
		float cm21 = m21 * other.m12 + m22 * other.m21 + m23 * other.m31 + m24 * other.m41;
		float cm31 = m31 * other.m12 + m32 * other.m21 + m33 * other.m31 + m34 * other.m41;
		float cm41 = m41 * other.m12 + m42 * other.m21 + m43 * other.m31 + m44 * other.m41;

		float cm12 = m11 * other.m12 + m12 * other.m22 + m13 * other.m32 + m14 * other.m42;
		float cm22 = m21 * other.m12 + m22 * other.m22 + m23 * other.m32 + m24 * other.m42;
		float cm32 = m31 * other.m12 + m32 * other.m22 + m33 * other.m32 + m34 * other.m42;
		float cm42 = m41 * other.m12 + m42 * other.m22 + m43 * other.m32 + m44 * other.m42;

		float cm13 = m11 * other.m13 + m12 * other.m23 + m13 * other.m33 + m14 * other.m43;
		float cm23 = m21 * other.m13 + m22 * other.m23 + m23 * other.m33 + m24 * other.m43;
		float cm33 = m31 * other.m13 + m32 * other.m23 + m33 * other.m33 + m34 * other.m43;
		float cm43 = m41 * other.m13 + m42 * other.m23 + m43 * other.m33 + m44 * other.m43;

		float cm14 = m11 * other.m14 + m12 * other.m24 + m13 * other.m34 + m14 * other.m44;
		float cm24 = m21 * other.m14 + m22 * other.m24 + m23 * other.m34 + m24 * other.m44;
		float cm34 = m31 * other.m14 + m32 * other.m24 + m33 * other.m34 + m34 * other.m44;
		float cm44 = m41 * other.m14 + m42 * other.m24 + m43 * other.m34 + m44 * other.m44;
		m11 = cm11;m21 = cm21;m31 = cm31;m41 = cm41;
		m12 = cm12;m22 = cm22;m32 = cm32;m42 = cm42;
		m13 = cm13;m23 = cm23;m33 = cm33;m43 = cm43;
		m14 = cm14;m24 = cm24;m34 = cm34;m44 = cm44;
	}

	void translate(float x, float y, float z) {
		Matrix4f mat = Matrix4f();
		mat.m14 = x;
		mat.m24 = y;
		mat.m34 = z;
		this->multiply(mat);
	}

	void rotateYDegrees(float angle) {
		Matrix4f mat = Matrix4f();
		angle = (angle / 360) * 3.1415 * 2;
		float co = cos(angle);
		float si = sin(angle);
		mat.m11 = co;
		mat.m13 = -si;
		mat.m31 = co;
		mat.m33 = si;
		this->multiply(mat);
	}

	void rotateXDegrees(float angle) {
		Matrix4f mat = Matrix4f();
		angle = (angle / 360) * 3.1415 * 2;
		float co = cos(angle);
		float si = sin(angle);
		mat.m22 = co;
		mat.m32 = -si;
		mat.m23 = si;
		mat.m33 = co;
		this->multiply(mat);
	}

	void rotateZDegrees(float angle) {
		Matrix4f mat = Matrix4f();
		angle = (angle / 360) * 3.1415 * 2;
		float co = cos(angle);
		float si = sin(angle);
		mat.m11 = co;
		mat.m12 = -si;
		mat.m21 = si;
		mat.m22 = co;
		this->multiply(mat);
	}

	void transform(Vec4f& vec) {
		float x = m11 * vec.x + m12 * vec.y + m13 * vec.z + m14 * vec.w;
		float y = m21 * vec.x + m22 * vec.y + m23 * vec.z + m24 * vec.w;
		float z = m31 * vec.x + m32 * vec.y + m33 * vec.z + m34 * vec.w;
		float w = m41 * vec.x + m42 * vec.y + m43 * vec.z + m44 * vec.w;
		vec.x = x;
		vec.y = y;
		vec.z = z;
		vec.w = w;
	}

	void loadToBuffer(FloatBuffer& buffer) {
		buffer.putFloat(m11);
		buffer.putFloat(m21);
		buffer.putFloat(m31);
		buffer.putFloat(m41);
		buffer.putFloat(m12);
		buffer.putFloat(m22);
		buffer.putFloat(m32);
		buffer.putFloat(m42);
		buffer.putFloat(m13);
		buffer.putFloat(m23);
		buffer.putFloat(m33);
		buffer.putFloat(m43);
		buffer.putFloat(m14);
		buffer.putFloat(m24);
		buffer.putFloat(m34);
		buffer.putFloat(m44);
	}

	static Matrix4f perspective(float fov,float aspectRatio,float zNear,float zFar) {
		Matrix4f mat = Matrix4f();
		fov = (fov / 180) * 3.141592;
		float t = tan(fov / 2);

		mat.m11 = 1 / (aspectRatio * t);
		mat.m22 = 1 / t;
		mat.m33 = (zFar + zNear) / (zNear - zFar);
		mat.m34 = (2 * zFar * zNear) / (zNear - zFar);
		mat.m43 = -1;
		mat.m44 = 0;
		return mat;
	}

	static Matrix4f ortho(float left, float right, float bottom, float top, float zNear, float zFar) {
		Matrix4f mat = Matrix4f();
		mat.m11 = 2 / (right - left);
		mat.m22 = 2 / (top - bottom);
		mat.m33 = -2 / (zFar - zNear);
		mat.m14 = -(right + left) / (right - left);
		mat.m24 = -(top + bottom) / (top - bottom);
		mat.m34 = -(zFar + zNear) / (zFar - zNear);
		mat.m44 = 1;
		return mat;
	}

	static Matrix4f modelview(Vec3f& eye, Vec3f& lookAt,Vec3f& up) {
		Vec3f between = eye - lookAt;
		between.normalize();
		Vec3f left = up.cross(between);
		left.normalize();
		Matrix4f mat = Matrix4f();

		mat.m11 = left.x;
		mat.m12 = left.y;
		mat.m13 = left.z;
		mat.m21 = up.x;
		mat.m22 = up.y;
		mat.m23 = up.z;
		mat.m31 = between.x;
		mat.m32 = between.y;
		mat.m33 = between.z;
		mat.m14 = -left.x * eye.x - left.y * eye.y - left.z * eye.z;
		mat.m24 = -up.x * eye.x - up.y * eye.y - up.z * eye.z;
		mat.m34 = -between.x * eye.x - between.y * eye.y - between.z * eye.z;
		mat.m44 = 1;
		mat.m42 = 0;
		mat.m41 = 0;
		return mat;
	}




};