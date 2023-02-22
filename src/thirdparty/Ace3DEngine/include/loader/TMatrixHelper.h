#pragma once
#include "loader/TMathHelper.h"

namespace T3D {
    float matrixDeterminant(const float* matrix);
    void matrixCompose(float* matrix, const float* position, const float* quaternion, const float* scale);
    void matrixDecompose(const float* matrix, float* position, float* quaternion, float* scale);
    void matrixMultiplyMatrices(float* outMatrix, const float* inMatrix1, const float* inMatrix2);
    void matrixInverse(float* outMatrix, const float* inMatrix);
    void multiplyMatrixArm(float* dst, const float* m1, const float* m2);
    void matrixMultiplyFloat4(float* out, const float* inMatrix, const float* in);
    void matrixMultiplyFloat4Bone(float* out, const float* inMatrix, const float* in);

    //float3 matrixMultiplyFloat3(const float* inMatrix, const float3& in);

    //void matrix3x3SetFromToRotation(const float3& inV3From, const float3& inV3To, float* outMatrix3x3);
    //float matrix3x3Get(const float* inMatrix3x3, int row, int col);
    //float3 invMul(const float* m, const float3& v);
}
