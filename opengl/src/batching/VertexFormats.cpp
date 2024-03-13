#pragma once
#include "VertexFormat.cpp"
#include "Elements.cpp";
class VertexFormats {


public:
	VertexFormat* POSITION_COLOR = nullptr;
	VertexFormat* POSITION_COLOR_TEX = nullptr;
	VertexFormat* POSITION_COLOR_TEX_NORMAL = nullptr;

	void initialize() {
		POSITION_COLOR = (new VertexFormat(Elements::POSITION))->add(Elements::COLOR);
		POSITION_COLOR_TEX = (new VertexFormat(Elements::POSITION))->add(Elements::COLOR)->add(Elements::UV);
		POSITION_COLOR_TEX_NORMAL = (new VertexFormat(Elements::POSITION))->add(Elements::COLOR)->add(Elements::UV)->add(Elements::NORMAL);

	}

};

static VertexFormats VERTEX_FORMATS = VertexFormats();