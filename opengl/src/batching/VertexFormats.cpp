#pragma once
#include "VertexFormat.cpp"
#include "Elements.cpp";
class VertexFormats {


public:
	VertexFormat* POSITION_COLOR = nullptr;

	void initialize() {
		POSITION_COLOR = (new VertexFormat(Elements::POSITION))->add(Elements::COLOR);

	}

};

static VertexFormats VERTEX_FORMATS = VertexFormats();