#pragma once
#include <vector>
#include "Element.cpp";

class VertexFormat {

private:
	friend class VertexFormats;

	std::vector<Element> elements;

	VertexFormat(Element element) {
		elements = std::vector<Element>();
		elements.push_back(element);
	}

	VertexFormat* add(Element element) {
		elements.push_back(element);
		return this;
	}

public:

	int getByteSize() {
		int size = 0;
		for (Element e : elements) {
			size += e.getFullByteSize();
		}
		return size;
	}

	int getSize() {
		return elements.size();
	}

	Element& getElement(int idx) {
		return elements.at(idx);
	}


};