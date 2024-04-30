#pragma once

#include "../Util.cpp";
#include "../math/Vec3f.cpp"
#include <vector>
#include <math.h>
#include "../system/MouseMoveablePoints.cpp";

class BezierSurface {

public:

	std::vector<Vec3f*> points;

	BezierSurface(Vec3f center,float rad) {
		points = std::vector<Vec3f*>();
		for (int x = -2; x <= 2;x++) { //-2 -1 0 1 2 
			for (int z = -2; z <= 2; z++) {
				Vec3f* v = new Vec3f(x * rad, 0, z * rad);
				v->x += center.x;
				v->y += center.y;
				v->z += center.z;
				points.push_back(v);
			}
		}
	}

	void addAllPointsAsMoveable(MouseMoveablePoints& mpoints) {
		for (Vec3f* v : points) {
			mpoints.addPoint(v);
		}
	}

	Vec3f calcPoint(float u,float v) {
		Vec3f result = Vec3f();
		for (int n = 0; n < 5; n++) {
			float bci = binomialCoefficient(4, n);
			float ui = powf(u, n);
			float ui1 = powf(1 - u, 4 - n);
			float bni = bci * ui * ui1;
			for (int m = 0; m < 5; m++) {
				Vec3f* point = points[n * 5 + m];
				float bcm = binomialCoefficient(4, m);
				float vi = powf(v,m);
				float vi1 = powf(1 - v, 4 - m);
				float bmj = bcm * vi * vi1;

				float x = point->x * bni * bmj;
				float y = point->y * bni * bmj;
				float z = point->z * bni * bmj;

				result.x += x;
				result.y += y;
				result.z += z;
			}
		}
		return result;
	}

	float binomialCoefficient(int n, int i) {
		float nfact = (float) Util::factorial(n);
		float ifact = (float) Util::factorial(i);
		float infact = (float) Util::factorial(n - i);
		return nfact / (ifact * infact);
	}


};
