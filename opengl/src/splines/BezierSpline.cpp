#include "BezierSpline.h"
#include "../RenderUtil.cpp"

//float bezier(float p, float p1, float p2, float p3, float p4) {
//    return (1 - p) * (1 - p) * (1 - p) * p1 +
//        3 * p * (1 - p) * (1 - p) * p2 +
//        3 * p * p * (1 - p) * p3 +
//        p * p * p * p4;
//}

float bezier(float p, float p1, float p2, float p3, float p4) {

    float t = p;
    float t3 = p * p * p;
    float t2 = p * p;

    return

        p1 * (-t3 + 3 * t2 - 3 * t + 1) +
        p2 * (3 * t3 - 6 * t2 + 3 * t) +
        p3 * (-3 * t3 + 3 * t2) +
        p4 * t3;

}

Vec3f bezierSpline(float p, Vec3f p1, Vec3f p2, Vec3f p3, Vec3f p4) {
    Vec3f v = Vec3f(
        bezier(p, p1.x, p2.x, p3.x, p4.x),
        bezier(p, p1.y, p2.y, p3.y, p4.y),
        bezier(p, p1.z, p2.z, p3.z, p4.z)
    );
    return v;
}

void BezierSpline::draw(Matrix4f transform, VertexBuffer* lineBuffer, float r, float g, float b, float a) {

    auto points = this->controlPoints;

    for (int i = 1; i < points->size() - 2;i++) {


        Vec3f pprev = *(points->at(i - 1));
        Vec3f p1 = *(points->at(i));
        Vec3f p4 = *(points->at(i + 1));
        Vec3f pnext = *(points->at(i + 2));

        float l = (p1 - p4).length();

        Vec3f p1add = p4 - pprev;
        p1add = p1add * (1 / p1add.length());
        p1add = p1add * (l / 4);
        
        Vec3f p2add = p1 - pnext;
        p2add = p2add * (1 / p2add.length());
        p2add = p2add * (l / 4);


        Vec3f p2 = p1 + p1add;

        Vec3f p3 = p4 + p2add;




        for (float p = 0; p < 1; p += 0.1) {


            Vec3f point = bezierSpline(p, p1, p2, p3, p4);
            Vec3f point2 = bezierSpline(p + 0.1, p1, p2, p3, p4);

            RenderUtil::lineBetween(transform, lineBuffer, point, point2, r, g, b, a);

        }

    }


}