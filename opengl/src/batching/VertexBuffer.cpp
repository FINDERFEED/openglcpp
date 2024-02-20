#pragma once
#include "../buffers/ByteBuffer.cpp"
#include "VertexFormats.cpp"
#include "glew.h";
#include "../math/Matrix4f.cpp"
#include "Elements.cpp";
#include <stdlib.h>

static int const triangleDrawOrder[] = {
		0,1,2,
		2,3,0
};

class VertexBuffer {

	

private:

	GLuint vao;
	GLuint vbo;
	GLuint ebo;

	ByteBuffer* vertexData;
	VertexFormat* format;
	GLuint drawMode;

	int vertexCount = 0;
	int currentElementId = 0;

public:


	VertexBuffer(VertexFormat* format,unsigned int defaultSize,GLuint drawMode) {
		this->vertexData = new ByteBuffer(defaultSize);
		this->format = format;
		this->drawMode = drawMode;
		glGenVertexArrays(1,&vao);
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);
	}

	VertexBuffer* position(Matrix4f& t,float x,float y,float z) {
		this->checkCurrentElement(Elements::POSITION);
		Vec4f v = Vec4f(x, y, z, 1.0);
		t.transform(v);
		vertexData->putFloat(v.x);
		vertexData->putFloat(v.y);
		vertexData->putFloat(v.z);
		currentElementId++;
		this->checkCurrentElementId();
		return this;
	}

	VertexBuffer* color(float r,float g,float b,float a) {
		this->checkCurrentElement(Elements::COLOR);
		vertexData->putFloat(r);
		vertexData->putFloat(g);
		vertexData->putFloat(b);
		vertexData->putFloat(a);
		currentElementId++;
		this->checkCurrentElementId();
		return this;
	}

	VertexBuffer* endVertex() {
		if (currentElementId != format->getSize()) {
			std::cout << "Vertex incomplete!" << std::endl;
			exit(1);
		}
		vertexCount++;
		currentElementId = 0;
		return this;
	}

	void checkCurrentElementId() {
		if (currentElementId > format->getSize()) {
			std::cout << "Too many elements!" << std::endl;
			exit(1);
		}
	}

	void checkCurrentElement(Element element) {
		Element& c = this->currentElement();
		if (c != element) {
			std::cout << "Incorrect element! Was " << c.getName() << " while " << element.getName() << " is required!" << std::endl;
			exit(1);
		}
	}

	Element& currentElement() {
		return format->getElement(currentElementId);
	}

	void draw() {
		switch (drawMode) {
		case GL_TRIANGLES: {
			this->drawTriangles();
			break;
		}
		}
	}

	void drawTriangles() {
		if (vertexCount % 4 != 0) {
			std::cout << "Incomplete buffer!";
			exit(1);
		}
		int vertexSize = format->getByteSize();
		glBindVertexArray(vao);
	
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		unsigned int datasize = vertexSize * vertexCount;
		glBufferData(GL_ARRAY_BUFFER, datasize,vertexData->getData(), GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
		int polygonCount = vertexCount / 4;
		unsigned int* indices = new unsigned int[polygonCount * 6];
		int k = 0;
		for (int p = 0; p < polygonCount; p++) {
			for (int i = 0; i < 6; i++) {
				int index = triangleDrawOrder[i] + p * 4;
				indices[k++] = index;
			}
		}
		int indicesSize = polygonCount * 6 * sizeof(unsigned int);
	
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,indicesSize,indices,GL_DYNAMIC_DRAW);
		int pointer = 0;
		for (int i = 0; i < format->getSize(); i++) {
			Element& element = format->getElement(i);
			glEnableVertexAttribArray(i);
			int elementCount = element.getElementCount();
			glVertexAttribPointer(i, elementCount, 
				element.getElementType(), GL_FALSE,
				vertexSize, (void*)pointer);
			pointer += element.getFullByteSize();
		}
		
		glDrawElements(GL_TRIANGLES, polygonCount * 6, GL_UNSIGNED_INT, nullptr);

		
		delete[] indices;
		this->cleanup();
	}

	void cleanup() {
		vertexData->resetPosition();
		currentElementId = 0;
		vertexCount = 0;
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	~VertexBuffer() {
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ebo);
		delete vertexData;
	}

};