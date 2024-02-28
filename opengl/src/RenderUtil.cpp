#pragma once
#include "batching/VertexBuffer.cpp"
#include "math/Matrix4f.cpp"
#include "math/Vec3f.cpp"
#include <math.h>
#include "Util.cpp"

class RenderUtil {

public:

     static void applyMovementMatrixRotations(Matrix4f& matrix, Vec3f& vec) {
        double angleY = Util::toDegrees(atan2(vec.x, vec.z));
        double angleX = Util::toDegrees(atan2(sqrt(vec.x * vec.x + vec.z * vec.z), vec.y));
        matrix.rotateZDegrees(angleY);
        matrix.rotateXDegrees(angleX);
        //matrices.mulPose(rotationDegrees(YP(), (float)angleY));
        //matrices.mulPose(rotationDegrees(XP(), (float)angleX));
    }
};