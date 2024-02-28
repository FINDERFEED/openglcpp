#pragma once
#include "Element.cpp"


class Elements {
public:
	constexpr static Element POSITION = Element(sizeof(float), 3,GL_FLOAT, "pos");
	constexpr static Element COLOR = Element(sizeof(float), 4,GL_FLOAT, "color");
	constexpr static Element UV = Element(sizeof(float), 2,GL_FLOAT, "uv");

};