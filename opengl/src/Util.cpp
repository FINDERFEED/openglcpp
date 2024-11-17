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

private: 


	static double readValue(std::string line,int index) {

		int reading = 0;
		int id = 0;
		int begin = 0;
		int end = 0;

		while (!reading) {
			char c = line.at(begin++);
			if (c == ' ') {
				if (id != index) {
					id++;
				} else {
					reading = true;
					end = begin;
				}
			}
		}

		while (true) {

			if (end >= line.length()) {
				reading = false;
				break;
			}

			char ch = line.at(end);
			if (ch == ' ') {
				break;
			}
			end++;
		}

		std::string l = line.substr(begin, end - begin);

		double value = std::stod(l.c_str());

		return value;
	}

public:


	static std::vector<Vec3f> readPointsFromFile(std::string path,float modifier) {

		int flag = 0;
		Vec3f firstPoint;

		std::vector<Vec3f> points = std::vector<Vec3f>();

		std::ifstream stream(path);

		std::string line;
		while (std::getline(stream,line)) {

			float v1 = (float) readValue(line,0) * modifier;
			float v2 = (float) readValue(line,1) * modifier;

			Vec3f v = Vec3f(v1,v2,0);

			if (!flag) {
				

				firstPoint = v;

				v = Vec3f(0, 0, 0);

				flag = true;
			} else {
				v = v - firstPoint;
			}

			points.push_back(v);

		}

		return points;
	}

	//N(i,k)
	//i - index
	//knots are 0,1,2,3,4.....
	static float coxDeBoorFormula(float i, float t,int degree) {
		if (degree != 0) {

			float t1 = (t - i) / (float) degree;
			float t2 = (i + degree + 1 - t) / (float) degree ;

			float n1 = coxDeBoorFormula(i,t,degree - 1);
			float n2 = coxDeBoorFormula(i + 1,t,degree - 1);

			float res = t1 * n1 + t2 * n2;

			return res;

		} else {
			if (t >= i && t < i + 1) {
				return 1;
			} else {
				return 0;
			}
		}
	}

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

	static int factorial(int val) {
		float result = 1;
		while (val > 1) {
			result *= val;
			val--;
		}
		return result;
	}

};