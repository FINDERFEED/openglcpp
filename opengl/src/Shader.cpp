#pragma once
#include "Util.cpp"
#include <glew.h>
#include "math/Matrix4f.cpp"
#include "batching/VertexFormat.cpp";
#include "system/Texture.cpp";

class Shader {

public:

	GLuint shaderProgram;
	GLuint vertexShader;
	GLuint fragmentShader;
	std::string name;

	Shader(std::string name,VertexFormat& format) {
		this->name = name;
		std::string vname = "resources/shaders/" + name + ".v";
		std::string fname = "resources/shaders/" + name + ".f";

		std::string vertex = Util::readFile(vname);
		std::string fragment = Util::readFile(fname);

		shaderProgram = glCreateProgram();
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		const char* vertexC = vertex.c_str();
		const char* fragmentC = fragment.c_str();
		compileShaders(name, vertexC, fragmentC);


		glAttachShader(shaderProgram, fragmentShader);
		glAttachShader(shaderProgram, vertexShader);
		for (int i = 0; i < format.getSize(); i++) {
			Element element = format.getElement(i);
			glBindAttribLocation(shaderProgram, i, element.getName());
		}
		link(name);
		validate(name);

	}

	GLuint getVertexShader() {
		return vertexShader;
	}
	GLuint getFragmentShader() {
		return fragmentShader;
	}
	GLuint getShaderProgram() {
		return shaderProgram;
	}

	void mat4uniform(const char* name,Matrix4f& mat) {
		FloatBuffer* buffer = new FloatBuffer(16);
		mat.loadToBuffer(*buffer);
		int pos = glGetUniformLocation(shaderProgram, name);
		if (pos == -1) {
			std::cout << "Uniform " << name << " doesn't exist in " << this->name << std::endl;
			exit(1);
		}
		glUniformMatrix4fv(pos,1,GL_FALSE,buffer->data);
		delete buffer;
	}

	void textureUniform(const char* name,GLuint texId) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texId);

		int pos = glGetUniformLocation(shaderProgram, name);
		if (pos == -1) {
			std::cout << "Uniform " << name << " doesn't exist in " << this->name << std::endl;
			exit(1);
		}
		glUniform1i(pos, 0);
	}

	void textureUniform(const char* name, Texture& tex) {
		this->textureUniform(name, tex.getTexId());
	}

	void process() {
		glUseProgram(shaderProgram);
	}

	void stop() {
		glUseProgram(0);
	}

private:

	void link(std::string name) {
		glLinkProgram(shaderProgram);
		int status; glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
		if (status == GL_FALSE) {
			int len; glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &len);
			char* c = new char[len];
			glGetProgramInfoLog(shaderProgram, len, &len, c);
			std::cout << "Error linking shader: " << name << " " << c << std::endl;
			throw "Error linking shader";
		}
	}

	void validate(std::string name) {
		glValidateProgram(shaderProgram);
		int status; glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &status);
		if (status == GL_FALSE) {
			int len; glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &len);
			char* c = new char[len];
			glGetProgramInfoLog(shaderProgram, len, &len, c);
			std::cout << "Error validating shader: " << name << " " << c << std::endl;
			throw "Error validating shader";
		}
	}

	void compileShaders(std::string& name,const char* vertexC,const char* fragmentC) {
		glShaderSource(vertexShader, 1, &vertexC, nullptr);
		glCompileShader(vertexShader);
		int result;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE) {
			int len;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &len);
			char* mes = new char[len];
			glGetShaderInfoLog(vertexShader, len, &len, mes);
			std::cout << mes << std::endl;
			std::cout << "Error compiling vertex shader" << name << ": " << mes << std::endl;
			delete[] mes;
			throw "Error compiling vertex shader";
		}

		glShaderSource(fragmentShader, 1, &fragmentC, nullptr);
		glCompileShader(fragmentShader);
		
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE) {
			int len;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &len);
			char* mes = new char[len];
			glGetShaderInfoLog(fragmentShader, len, &len, mes);
			std::cout << "Error compiling fragment shader" << name << ": " << mes << std::endl;
			delete[] mes;
			throw "Error compiling fragment shader";
		}
	}

};