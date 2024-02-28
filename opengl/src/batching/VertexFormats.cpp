#pragma once
#include "VertexFormat.cpp"
#include "Elements.cpp";
class VertexFormats {


public:
	VertexFormat* POSITION_COLOR = nullptr;
	VertexFormat* POSITION_COLOR_TEX = nullptr;

	void initialize() {
		POSITION_COLOR = (new VertexFormat(Elements::POSITION))->add(Elements::COLOR);
		POSITION_COLOR_TEX = (new VertexFormat(Elements::POSITION))->add(Elements::COLOR)->add(Elements::UV);

	}

};

static VertexFormats VERTEX_FORMATS = VertexFormats();