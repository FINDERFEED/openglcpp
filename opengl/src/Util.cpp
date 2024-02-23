#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#define PI 3.1415926

class Util {

public:


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

	static float toEuler(float degrees) {
		degrees = fmod(degrees, 360);
		if (degrees < -180) {
			degrees = 360 + degrees;
		} else if (degrees > 180){
			degrees = degrees - 360;
		}
		return degrees;
	}
};