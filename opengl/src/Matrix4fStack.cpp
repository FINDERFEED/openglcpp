#pragma once

#include <vector>
#include "math/Matrix4f.cpp"

class Matrix4fStack {

private:

	std::vector<Matrix4f*>* matrices;


public:

	Matrix4fStack() {
		this->matrices = new std::vector<Matrix4f*>();
	}

	~Matrix4fStack() {
		delete matrices;
	}

	void push() {
		Matrix4f* mat = new Matrix4f();
		if (matrices->size() != 0) {
			Matrix4f* m1 = matrices->at(matrices->size() - 1);
			mat->copy(*m1);
		}
		matrices->push_back(mat);
	}

	void pop() {
		this->checkEmpty();
		matrices->pop_back();
	}

	Matrix4f* getLast() {
		this->checkEmpty();
		return matrices->at(matrices->size() - 1);
	}

	void translate(float x, float y, float z) {
		this->checkEmpty();
		this->getLast()->translate(x, y, z);
	}

	void rotate(float x, float y, float z, float degrees) {
		this->checkEmpty();
		Quaternion q = Quaternion(x, y, z, degrees);
		this->getLast()->rotate(q);
	}


	void checkEmpty() {
		if (matrices->size() == 0) {
			std::cout << "Matrix stack empty!" << std::endl;
			exit(1);
		}
	}



};