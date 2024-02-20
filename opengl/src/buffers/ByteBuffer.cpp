#pragma once

#include <iostream>
#include <stdio.h>

typedef uint8_t byte;


class ByteBuffer {
	
private:

	unsigned int position;
	unsigned int capacity;
	byte* data;
	
public:

	~ByteBuffer() {
		free(data);
	}

	ByteBuffer(unsigned int defaultByteSize) {
		data = (byte*) malloc(defaultByteSize);
		this->capacity = defaultByteSize;
		this->position = 0;
	}


	void putFloat(float a) {
		unsigned int byteValue = sizeof(float);
		this->ensureCapacity(byteValue);
		float* p = (float*)(&(data[position]));
		*p = a;
		position += byteValue;
	}

	void putFloats(int size,float* a) {
		unsigned int byteValue = sizeof(float) * size;
		this->ensureCapacity(byteValue);
		for (int i = 0; i < size;i++) {
			float* p = (float*) ( & this->getData()[position + i * sizeof(float)]);
			*p = a[i];
		}
		position += sizeof(float) * size;
	}


	void ensureCapacity(unsigned int byteValue) {
		while (position + byteValue >= capacity) {
			
			byte* p = (byte*)realloc(data, capacity * 2);
			if (p == NULL) {
				throw "Out of memory";
				exit(1);
			}
			this->data = p;
			capacity *= 2;
		}
	}

	byte* getData() {
		return this->data;
	}

	void resetPosition() {
		this->position = 0;
	}

	unsigned int getPosition() {
		return position;
	}


};