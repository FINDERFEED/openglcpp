#pragma once
#include "Shader.cpp"
#include "batching/VertexFormats.cpp"


class Shaders {

public:


	Shaders() {

	}


	Shader* POSITION_COLOR = nullptr;


	void initialize() {
		POSITION_COLOR = new Shader("position_color", *VERTEX_FORMATS.POSITION_COLOR);
	}

};

static Shaders SHADERS = Shaders();