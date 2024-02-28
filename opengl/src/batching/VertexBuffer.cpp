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
	int wasRenderedOnce = 0;

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

	VertexBuffer* uv(float u,float v) {
		this->checkCurrentElement(Elements::UV);
		vertexData->putFloat(u);
		vertexData->putFloat(v);
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

	void draw(int keepData) {
		if (vertexCount == 0) {
			return;
		}
		switch (drawMode) {
		case GL_QUADS: {
			this->drawQuads(keepData);
			break;
		}
		case GL_LINES: {
			this->drawLines(keepData);
			break;
		}
		case GL_TRIANGLES: {
			this->drawTriangles(keepData);
			break;
		}
		}
	}

	void drawTriangles(int keepData) {
		if (vertexCount % 3 != 0) {
			std::cout << "Incomplete buffer!" << std::endl;
			exit(1);
		}

		int vertexSize = format->getByteSize();
		glBindVertexArray(vao);

		loadToVBO(keepData);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		if (!keepData || keepData && !wasRenderedOnce) {

			unsigned int* indices = new unsigned int[vertexCount];
			for (int i = 0; i < vertexCount; i++) {
				indices[i] = i;
			}

			glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexCount * sizeof(unsigned int), indices,keepData ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
			delete[] indices;
		}
		initiateVAO();

		glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, nullptr);


		this->cleanup(keepData);
	}

	void drawLines(int keepData) {
		if (vertexCount % 2 != 0) {
			std::cout << "Incomplete buffer!" << std::endl;
			exit(1);
		}
		int vertexSize = format->getByteSize();
		glBindVertexArray(vao);

		loadToVBO(keepData);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		if (!keepData || keepData && !wasRenderedOnce) {

			unsigned int* indices = new unsigned int[vertexCount];
			for (int i = 0; i < vertexCount; i++) {
				indices[i] = i;
			}

			glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexCount * sizeof(unsigned int), indices, keepData ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
			delete[] indices;
		}
		initiateVAO();

		glDrawElements(GL_LINES, vertexCount, GL_UNSIGNED_INT, nullptr);


		this->cleanup(keepData);
	}




	void drawQuads(int keepData) {
		if (vertexCount % 4 != 0) {
			std::cout << "Incomplete buffer!";
			exit(1);
		}
		int vertexSize = format->getByteSize();
		glBindVertexArray(vao);
	


		loadToVBO(keepData);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
		int polygonCount = vertexCount / 4;
		if (!keepData || keepData && !wasRenderedOnce) {
			
			unsigned int* indices = new unsigned int[polygonCount * 6];
			int k = 0;
			for (int p = 0; p < polygonCount; p++) {
				for (int i = 0; i < 6; i++) {
					int index = triangleDrawOrder[i] + p * 4;
					indices[k++] = index;
				}
			}
			int indicesSize = polygonCount * 6 * sizeof(unsigned int);

			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, keepData ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
			delete[] indices;
		}
		initiateVAO();
		
		glDrawElements(GL_TRIANGLES, polygonCount * 6, GL_UNSIGNED_INT, nullptr);

		this->cleanup(keepData);
	}

	void loadToVBO(int keepData) {
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		if (!keepData || keepData && !wasRenderedOnce) {
			unsigned int datasize = format->getByteSize() * vertexCount;
			glBufferData(GL_ARRAY_BUFFER, datasize, vertexData->getData(),keepData ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
		}
	}

	void initiateVAO() {
		int pointer = 0;
		for (int i = 0; i < format->getSize(); i++) {
			Element& element = format->getElement(i);
			glEnableVertexAttribArray(i);
			int elementCount = element.getElementCount();
			glVertexAttribPointer(i, elementCount,
				element.getElementType(), GL_FALSE,
				format->getByteSize(), (void*)pointer);
			pointer += element.getFullByteSize();
		}
	}


	void cleanup(int keepData) {
		if (!keepData || keepData && !wasRenderedOnce) {
			vertexData->resetPosition();
			currentElementId = 0;
			if (!keepData){
				vertexCount = 0;
			}
			wasRenderedOnce = keepData;
		}
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