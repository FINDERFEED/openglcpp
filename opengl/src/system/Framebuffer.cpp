#pragma once

#include "Texture.cpp"
#include <glew.h>

class Framebuffer {

	GLuint id = -1;
	GLuint rboId = -1;
	Texture* colorTexture = nullptr;
	Texture* depthTexture = nullptr;
	TextureParameters colorParams;
	TextureParameters depthParams;

public:

	Framebuffer(std::string name,TextureParameters colorTexParameters,TextureParameters depthParams, int width, int height) : 
		Framebuffer(name, colorTexParameters,depthParams, 1, width, height,1, width, height,0){

	}

	Framebuffer(std::string name, int useColor, int width, int height, int useDepth, int dWidth, int dHeight, int createRbo) : 
		Framebuffer(name,DEFAULT_PARAMETERS,SHADOW_DEPTH_PARAMETERS,useColor,width,height,useDepth,dWidth,dHeight,createRbo) {

	}

	Framebuffer(std::string name, TextureParameters colorTexParams,TextureParameters depthParams, int useColor, int width, int height, int useDepth, int dWidth, int dHeight,int createRbo) {
		colorParams = colorTexParams;
		this->depthParams = depthParams;
		if (useDepth && createRbo) {
			std::cout << "Cannot initialize depth and renderbuffer at the same time! Framebuffer: " << name;
			exit(1);
		}

		glGenFramebuffers(1, &id);
		glBindFramebuffer(GL_FRAMEBUFFER, id);

		if (useColor) {
			colorTexParams.setResolution(width, height);
			Texture* texture = new Texture("generated_color:" + name, colorTexParams, 1);
			this->colorTexture = texture;
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture->getTexId(), 0);
		}
		if (useDepth) {
			TextureParameters params = depthParams.setResolution(dWidth, dHeight);
			Texture* texture = new Texture("generated_depth:" + name, params, 0);
			this->depthTexture = texture;
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,texture->getTexId(), 0);
		} else if (createRbo) {
			glGenRenderbuffers(1, &rboId);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboId);
		}

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::cout << "Failed to create framebuffer " << name;
			exit(1);
		}

		glBindFramebuffer(GL_FRAMEBUFFER,0);
	}

	void bindWrite() {
		glBindFramebuffer(GL_FRAMEBUFFER, this->id);
	}

	void unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void resize(int width,int height,int dwidth,int dheight) {
		if (colorTexture != nullptr && (width - 1 || height != -1)) {
			TextureParameters params = colorParams.setResolution(width,height);
			colorTexture->resize(params);
		}
		if (depthTexture != nullptr && (dwidth -1 || dheight != -1)) {
			TextureParameters params = depthParams.setResolution(dwidth, dheight);
			depthTexture->resize(params);
		}
	}

	Texture* getColorTexture() {
		return colorTexture;
	}

	Texture* getDepthTexture() {
		return depthTexture;
	}

};