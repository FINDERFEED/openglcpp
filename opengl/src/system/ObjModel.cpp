#pragma once
#include <vector>
#include <iostream>
#include "../math/Vec2f.cpp"
#include "../math/Vec3f.cpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include "../Util.cpp"

class OBJFace {

public:

	std::vector<Vec3f> vertices;
	std::vector<Vec3f> normals;
	std::vector<Vec2f> uvs;

	OBJFace(){
	
	}

	OBJFace(std::vector<Vec3f> vertices, std::vector<Vec2f> uvs, std::vector<Vec3f> normals) {
		this->vertices = vertices;
		this->uvs = uvs;
		this->normals = normals;
		if (vertices.size() != uvs.size() || vertices.size() != normals.size()) {
			std::cout << "Count of normals and uvs should be equal to vertices amount.";
			exit(1);
		}
	}

	void addVertex(Vec3f vertex, Vec2f uv, Vec3f normal) {
		this->vertices.push_back(vertex);
		this->uvs.push_back(uv);
		this->normals.push_back(normal);
	}

};

class ObjModel {

	std::vector<OBJFace> faces;



	static ObjModel* loadModel(std::string& path) {
		std::ifstream stream(path);
		std::string line;
		std::vector<Vec3f> vertices;
		std::vector<Vec3f> normals;
		std::vector<Vec2f> uvs;
		while (std::getline(stream, line)) {



		}
		ObjModel* model = new ObjModel();
		return model;
	}

public:

	static OBJFace parseFace(std::string line,std::vector<Vec3f>& vertices, std::vector<Vec2f>& uvs, std::vector<Vec3f>& normals) {
		auto f = Util::splitStringByRegex(line, "\\s+");
		OBJFace face = OBJFace();
		for (int i = 1; i < f->size(); i++) {
			std::string faceData = (*f)[i];
			auto vuvn = Util::splitStringByRegex(faceData, "\\");
			int v = std::stoi((*vuvn)[0]);
			int uv = std::stoi((*vuvn)[1]);
			int n = std::stoi((*vuvn)[2]);
			delete vuvn;
			face.addVertex(vertices[v - 1],uvs[uv - 1],normals[n - 1]);
		}
		delete f;
		return face;
	}

	static Vec3f parseVec3(std::string line) {
		auto v123 = Util::splitStringByRegex(line, "\\s+");
		float x = std::stof((*v123)[1]);
		float y = std::stof((*v123)[2]);
		float z = std::stof((*v123)[3]);
		Vec3f v = Vec3f(x, y, z);
		delete v123;
		return v;
	}

	static Vec2f parseVec2(std::string line) {
		auto v123 = Util::splitStringByRegex(line, "\\s+");
		float x = std::stof((*v123)[1]);
		float y = std::stof((*v123)[2]);
		Vec2f v = Vec2f(x, y);
		delete v123;
		return v;
	}

	

};

