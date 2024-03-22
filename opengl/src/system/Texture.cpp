#pragma once

#include <stb_image.h>
#include <iostream>
#include "glew.h"

class TextureParameters {

public:

	int width = 0;
	int height = 0;
	int minFilter = GL_NEAREST;
	int magFilter = GL_NEAREST;
	int wrapT = GL_REPEAT;
	int wrapS = GL_REPEAT;
	int internalFormat = GL_RGBA8;
	int pixelFormat = GL_RGBA;

	TextureParameters() {}

	TextureParameters(int width,int height) {
		this->width = width;
		this->height = height;
	}

	TextureParameters& setFilter(int minFilter, int magFilter) {
		this->minFilter = minFilter;
		this->magFilter = magFilter;
		return *this;
	}

	TextureParameters& setWrap(int wrapS, int wrapT) {
		this->wrapS = wrapS;
		this->wrapT = wrapT;
		return *this;
	}

	TextureParameters& setLoadParameters(int internalFormat,int pixelFormat) {
		this->internalFormat = internalFormat;
		this->pixelFormat = pixelFormat;
		return *this;
	}

	TextureParameters& setResolution(int width, int height) {
		this->width = width;
		this->height = height;
		return *this;
	}



};

static TextureParameters DEFAULT_PARAMETERS = TextureParameters();

class Texture {

private:

	int width;
	int height;
	int channels;
	GLuint texId;

public:
	


	Texture(std::string name,TextureParameters& parameters,int load) {
		stbi_set_flip_vertically_on_load(1);
		
		unsigned char* bytes = nullptr;
		this->width = parameters.width;
		this->height = parameters.height;

		if (load) {
			std::string f = "resources/textures/" + name + ".png";
			bytes = stbi_load(f.c_str(), &(this->width), &(this->height), &channels, 4);
		}

		glGenTextures(1,&texId);

		glBindTexture(GL_TEXTURE_2D, texId);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, parameters.wrapT); //repeat or clamp
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, parameters.wrapS);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, parameters.minFilter);//nearest linear etc
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, parameters.magFilter);

		glTexImage2D(GL_TEXTURE_2D, 0,parameters.internalFormat,this->width,this->height,0,parameters.pixelFormat, GL_UNSIGNED_BYTE, bytes);
	
		if (load) {
			stbi_image_free(bytes);
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	Texture(std::string name) : Texture(name,DEFAULT_PARAMETERS,1) {}

	GLuint getTexId() {
		return this->texId;
	}

	int getWidth() {
		return width;
	}

	int getHeight() {
		return height;
	}

	void resize(TextureParameters& params) {
		glBindTexture(GL_TEXTURE_2D, texId);
		glTexImage2D(GL_TEXTURE_2D, 0, params.internalFormat, params.width, params.height, 0, params.pixelFormat, GL_UNSIGNED_BYTE, 0);
	}
};