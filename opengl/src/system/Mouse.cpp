#pragma once

#include "glew.h";
#include "GLFW/glfw3.h";

class Mouse {

	double mx = 0;
	double my = 0;
	double mxo = 0;
	double myo = 0;
	double dx = 0;
	double dy = 0;
	int clickedButtons[10];

public:

	Mouse() {
		for (int i = 0; i < 10; i++) {
			clickedButtons[i] = 0;
		}
	}


	void setPos(double x, double y) {
		this->mx = x;
		this->my = y;
	}

	void tick() {
		dx = mx - mxo;
		dy = my - myo;
		mxo = mx;
		myo = my;
	}

	double getMouseX() {
		return mx;
	}
	
	double getMouseY() {
		return my;
	}
	double getMouseXO() {
		return mxo;
	}
	
	double getMouseYO() {
		return myo;
	}
	double getMouseDX() {
		return dx;
	}
	
	double getMouseDY() {
		return dy;
	}

	void clickButton(int button) {
		if (button < 10) {
			clickedButtons[button] = 1;
		}
	}

	void releaseButton(int button) {
		if (button < 10) {
			clickedButtons[button] = 0;
		}
	}

	int isLeftButtonPressed() {
		return clickedButtons[0];
	}
	int isRightButtonPressed() {
		return clickedButtons[1];
	}
};