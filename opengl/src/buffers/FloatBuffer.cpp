#pragma once

#include <iostream>
#include <stdio.h>

class FloatBuffer {


public:
	int position;
	int size;
	float* data;

	FloatBuffer(unsigned int size) {
		this->data = (float*)malloc(size * sizeof(float));
		this->position = 0;
		this->size = size;
	}

	void putFloat(float data) {
		this->ensureCapacity(1);
		this->data[position] = data;
		position++;
	}

	void putFloats(int size,float nums[]) {
		this->ensureCapacity(size);
		for (int i = 0; i < size; i++) {
			this->data[position + i] = nums[i];
		}
		position += size;
	}


	void ensureCapacity(int dataSize) {
		while (this->position + dataSize >= size) {
			float* p = (float*) realloc(data,size * 2 * sizeof(float));
			if (p == NULL) {
				throw "Out of memory";
			}
			data = p;
			size *= 2;
		}
	}

	void resetPosition() {
		this->position = 0;
	}

	void release() {
		free(this->data);
	}

	~FloatBuffer() {
		delete data;
	}

};