#pragma once

#include "Texture.cpp"
#include <glew.h>

class Framebuffer {

	GLuint id = -1;
	GLuint rboId = -1;
	Texture* colorTexture = nullptr;
	Texture* depthTexture = nullptr;

public:

	Framebuffer(std::string name, int useColor, int width, int height, int useDepth, int dWidth, int dHeight,int createRbo) {

		if (useDepth && createRbo) {
			std::cout << "Cannot initialize depth and renderbuffer at the same time! Framebuffer: " << name;
			exit(1);
		}

		glGenFramebuffers(1, &id);
		glBindFramebuffer(GL_FRAMEBUFFER, id);

		if (useColor) {
			Texture* texture = new Texture("generated_color:" + name, DEFAULT_PARAMETERS, 1);
			this->colorTexture = texture;
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture->getTexId(), 0);
		}
		if (useDepth) {
			TextureParameters params = TextureParameters().setLoadParameters(GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT)
				.setResolution(dWidth, dHeight);
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
			TextureParameters params = TextureParameters().setResolution(width,height).setLoadParameters(GL_RGBA8,GL_RGBA);
			colorTexture->resize(params);
		}
		if (depthTexture != nullptr && (dwidth -1 || dheight != -1)) {
			TextureParameters params = TextureParameters().setResolution(width, height)
				.setLoadParameters(GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT);
			colorTexture->resize(params);
		}
	}

	Texture* getColorTexture() {
		return colorTexture;
	}

	Texture* getDepthTexture() {
		return depthTexture;
	}

};