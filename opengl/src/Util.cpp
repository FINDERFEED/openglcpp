#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

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

};