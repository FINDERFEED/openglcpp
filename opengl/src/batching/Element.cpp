#pragma once

#include <string.h>
#include "glew.h"

class Element {

private:
	int byteSize;
	int elementCount;
	const char* name;
	GLuint elementType;
	friend class Elements;
	constexpr Element(int byteSize, int elementCount,GLuint elementType, const char* name) :
		byteSize(byteSize),elementCount(elementCount),name(name),elementType(elementType)
	{
		
	}

public:

	GLuint getElementType() {
		return this->elementType;
	}

	int getSingleElementByteSize() {
		return byteSize;
	}

	int getElementCount() {
		return elementCount;
	}

	const char* getName() {
		return name;
	}

	int getFullByteSize() {
		return elementCount * byteSize;
	}

	int operator!=(const Element& other) {
		return strcmp(this->name, other.name);
	}

};