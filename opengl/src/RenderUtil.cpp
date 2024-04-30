#pragma once
#include "batching/VertexBuffer.cpp"
#include "math/Matrix4f.cpp"
#include "math/Vec3f.cpp"
#include <math.h>
#include "Util.cpp"

class RenderUtil {

public:

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