#pragma once

#include <stb_image.h>
#include <iostream>
#include "glew.h"

class Texture {

private:

	int width;
	int height;
	int channels;
	GLuint texId;

public:
	

	Texture(std::string name,GLuint wrapt,GLuint wraps,GLuint minfilter,GLuint magfilter) {
		std::string f = "resources/textures/" + name + ".png";
		stbi_set_flip_vertically_on_load(1);
		
		unsigned char* bytes = stbi_load(f.c_str(),&width,&height,&channels,4);


		glGenTextures(1,&texId);

		glBindTexture(GL_TEXTURE_2D, texId);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapt); //repeat or clamp
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wraps);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minfilter);//nearest linear etc
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magfilter);

		glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA8,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,bytes);
	
		stbi_image_free(bytes);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	Texture(std::string name) : Texture(name,GL_REPEAT,GL_REPEAT,GL_NEAREST,GL_NEAREST) {}

	GLuint getTexId() {
		return this->texId;
	}

	int getWidth() {
		return width;
	}

	int getHeight() {
		return height;
	}

};