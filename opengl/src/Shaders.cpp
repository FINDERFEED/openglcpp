#pragma once
#include "Shader.cpp"
#include "batching/VertexFormats.cpp"


class Shaders {

public:


	Shaders() {

	}


	Shader* POSITION_COLOR = nullptr;
	Shader* POSITION_COLOR_TEX = nullptr;


	void initialize() {
		POSITION_COLOR = new Shader("position_color", *VERTEX_FORMATS.POSITION_COLOR);
		POSITION_COLOR_TEX = new Shader("position_color_tex", *VERTEX_FORMATS.POSITION_COLOR_TEX);
	}

};

static Shaders SHADERS = Shaders();