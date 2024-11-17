#pragma once
#include "batching/VertexBuffer.cpp"
#include "math/Matrix4f.cpp"
#include "math/Vec3f.cpp"
#include <math.h>
#include "Util.cpp"

class RenderUtil {

public:



    static void lineBetween(Matrix4f& mat, VertexBuffer* line, Vec3f p1, Vec3f p2, float r, float g, float b, float a) {
        line->position(mat, p1.x, p1.y, p1.z)->color(r, g, b, a)->endVertex();
        line->position(mat, p2.x, p2.y, p2.z)->color(r, g, b, a)->endVertex();
    }


     static void applyMovementMatrixRotations(Matrix4f& matrix, Vec3f& vec) {
        float angleY = Util::toDegrees(atan2(vec.x, vec.z));
        float angleX = Util::toDegrees(atan2(sqrt(vec.x * vec.x + vec.z * vec.z), vec.y));
        matrix.rotateYDegrees(angleY);
        matrix.rotateXDegrees(angleX);
    }

     static float lerp(float v1,float v2,float p) {
         return v1 + (v2 - v1) * p;
     }
};