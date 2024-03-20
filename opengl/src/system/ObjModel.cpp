#pragma once
#include <vector>
#include <iostream>
#include "../math/Vec2f.cpp"
#include "../math/Vec3f.cpp"
#include "../math/Matrix4f.cpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include "../Util.cpp"
#include "../batching/VertexBuffer.cpp"

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

public:
	std::vector<OBJFace> faces;
	std::string name;

	ObjModel(std::vector<OBJFace> faces,std::string name) {
		this->faces = faces;
		this->name = name;
	}


	static ObjModel* loadModel(std::string modelName) {
		std::string path = "resources/models/" + modelName + ".obj";
		std::ifstream stream(path);
		std::cout << "Reading model: " + path << std::endl;
		if (!stream.is_open()) {
			std::cout << "File " << path << " not found";
			exit(1);
		}
		std::string line;
		std::vector<Vec3f> vertices;
		std::vector<Vec3f> normals;
		std::vector<Vec2f> uvs;
		std::vector<OBJFace> faces;
		while (std::getline(stream, line)) {
			if (line[0] == 'v' && line[1] == ' ') {
				Vec3f v = parseVec3(line);
				vertices.push_back(v);
			} else if (line[0] == 'v' && line[1] == 't') {
				Vec2f vt = parseVec2(line);
				uvs.push_back(vt);
			} else if (line[0] == 'v' && line[1] == 'n') {
				Vec3f vn = parseVec3(line);
				normals.push_back(vn);
			} else if (line[0] == 'f') {
				OBJFace face = parseFace(line,vertices,uvs,normals);
				faces.push_back(face);
			}
		}
		stream.close();
		ObjModel* model = new ObjModel(faces,path);
		return model;
	}

	//position color uv normals
	void render(Matrix4f mat,VertexBuffer* quadBuffer,VertexBuffer* triangleBuffer,float r,float g,float b,float a,int immediate) {
		for (OBJFace face : faces) {
			int size = face.vertices.size();
			if (size == 3) {
				loadFace(mat, face, 3, triangleBuffer, r, g, b, a);
			} else if (size == 4) {
				loadFace(mat, face, 4, quadBuffer, r, g, b, a);
			} else {
				std::cout << "Model " << name << " has a face with vertex count not equal to 3 or 4";
				exit(1);
			}
		}
		if (immediate) {
			quadBuffer->draw(0);
			triangleBuffer->draw(0);
		}
	}

private:

	void loadFace(Matrix4f mat,OBJFace& face,int size,VertexBuffer* buffer,float r,float g,float b,float a) {
		for (int i = 0; i < size; i++) {
			Vec3f v = face.vertices[i];
			Vec2f uv = face.uvs[i];
			Vec3f vn = face.normals[i];
			buffer->position(mat, v.x, v.y, v.z)->color(r, g, b, a)->uv(uv.x, uv.y)->normal(mat, vn)->endVertex();
		}
	}

	static OBJFace parseFace(std::string line,std::vector<Vec3f>& vertices, std::vector<Vec2f>& uvs, std::vector<Vec3f>& normals) {
		auto f = Util::splitStringByRegex(line, "\\s+");
		OBJFace face = OBJFace();
		for (int i = 1; i < f->size(); i++) {
			std::string faceData = (*f)[i];
			auto vuvn = Util::splitStringByRegex(faceData, "/");
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

