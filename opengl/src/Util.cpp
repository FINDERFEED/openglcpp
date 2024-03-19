#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "math/Vec3f.cpp"
#include <regex>
#define PI 3.1415926

class Util {

public:

	static std::vector<std::string>* splitStringByRegex(std::string s,std::string reg) {
		std::regex rgx(reg);
		std::sregex_token_iterator iter(s.begin(),s.end(),rgx,-1);
		std::sregex_token_iterator end;
		std::vector<std::string>* strs = new std::vector<std::string>();
		while (iter != end) {
			if (iter->length()) {
				strs->push_back(*iter);
			}
			iter++;
		}
		return strs;
	}

	static std::string readFile(std::string& path) {
		std::ifstream stream(path);

		std::string file;

		std::string line;
		while (std::getline(stream,line)) {
			file.append(line);
			file.append("\n");
		}


		stream.close();
		return file;
	}

	static float clamp(float val, float min, float max) {
		if (val < min) {
			return min;
		}
		else if (val > max) {
			return max;
		}
		return val;
	}

	static float toRadians(float angle) {
		return angle / 360 * PI * 2;
	}

	static float toDegrees(float angle) {
		return angle / PI / 2 * 360;
	}

	static float toEuler(float degrees) {
		degrees = fmod(degrees, 360);
		if (degrees < -180) {
			degrees = 360 + degrees;
		} else if (degrees > 180){
			degrees = degrees - 360;
		}
		return degrees;
	}


	static Vec3f computeNormal(Vec3f p1, Vec3f p2, Vec3f p3) {
		Vec3f b1 = p2 - p1;
		Vec3f b2 = p3 - p1;
		Vec3f n = b1.cross(b2);
		n.normalize();
		return n;
	}

	static Vec3f computeNormal(float x1,float y1,float z1,float x2,float y2,float z2,float x3,float y3,float z3) {
		Vec3f p1 = Vec3f(x1, y1, z1);
		Vec3f p2 = Vec3f(x2, y2, z2);
		Vec3f p3 = Vec3f(x3, y3, z3);
		return computeNormal(p1, p2, p3);
	}

};