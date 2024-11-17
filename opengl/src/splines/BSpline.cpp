#include "BSpline.h"
#include "../RenderUtil.cpp"


Vec3f BSpline::calculateDeBor(float p) {

	int pointCount = this->controlPoints->size();

	Vec3f sum = Vec3f(0,0,0);

	for (int index = 0; index < pointCount; index++) {

		Vec3f* point = this->controlPoints->at(index);
		float n = Util::coxDeBoorFormula(index , p, this->degree);
		Vec3f res = (*point) * n;
		sum += res;

	}

	return sum;
}

void BSpline::draw(Matrix4f transform,VertexBuffer* lineBuffer, float r, float g, float b, float a) {

	auto points = this->controlPoints;

	int pointCount = points->size();

	if (pointCount < 3) return;


	Vec3f previousPoint = calculateDeBor(0);

	float step = 0.1;

	for (float i = 1 + step; i <= pointCount; i += step) {

		Vec3f point = calculateDeBor(i);

		RenderUtil::lineBetween(transform, lineBuffer, previousPoint, point, r, g, b, a);

		previousPoint = point;
	}

}

