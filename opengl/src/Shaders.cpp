#pragma once
#include "Shader.cpp"
#include "batching/VertexFormats.cpp"


class Shaders {

public:


	Shaders() {

	}


	Shader* POSITION_COLOR = nullptr;
	Shader* POSITION_COLOR_TEX = nullptr;
	Shader* POSITION_COLOR_TEX_CIRCLE = nullptr;
	Shader* POSITION_COLOR_TEX_NORMAL = nullptr;
	Shader* POSITION_COLOR_TEX_NORMAL_SHADOW = nullptr;
	Shader* SHADOW = nullptr;


	void initialize() {
		POSITION_COLOR = new Shader("position_color", *VERTEX_FORMATS.POSITION_COLOR);
		POSITION_COLOR_TEX = new Shader("position_color_tex", *VERTEX_FORMATS.POSITION_COLOR_TEX);
		POSITION_COLOR_TEX_CIRCLE = new Shader("position_color_tex_circle", *VERTEX_FORMATS.POSITION_COLOR_TEX);
		POSITION_COLOR_TEX_NORMAL = new Shader("position_color_tex_normal", *VERTEX_FORMATS.POSITION_COLOR_TEX_NORMAL);
		POSITION_COLOR_TEX_NORMAL_SHADOW = new Shader("position_color_tex_normal_shadow", *VERTEX_FORMATS.POSITION_COLOR_TEX_NORMAL);
		SHADOW = new Shader("shadow", *VERTEX_FORMATS.POSITION);
	}

};

static Shaders SHADERS = Shaders();