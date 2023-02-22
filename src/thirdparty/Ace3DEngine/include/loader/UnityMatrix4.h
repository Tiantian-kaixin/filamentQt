#pragma once
#include "UnityFloat4.h"
#include "UnityMath.h"

namespace UM {
    struct float3;
    struct float4;
    struct matrix3;
    struct RigidTransform;

    struct matrix4 {
        float4 c0;
        float4 c1;
        float4 c2;
        float4 c3;

        static matrix4 identity;

        matrix4(float4 _c0, float4 _c1, float4 _c2, float4 _c3);
        matrix4(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33);
        explicit matrix4(float v);
        explicit matrix4(const float* v);
        explicit matrix4(bool v);
        matrix4(matrix3 rotation, float3 translation);
        matrix4(float4 rotation, float3 translation);
        //matrix4(RigidTransform transform);
        explicit matrix4(const matrix4& rhs);

        float& operator[](size_t index);
        const float& operator[](size_t index) const;

        //static matrix4 axisAngle(float3 axis, float angle);

        //static matrix4 eulerXYZ(float3 xyz);
        //static matrix4 eulerXZY(float3 xyz);
        //static matrix4 eulerYXZ(float3 xyz);
        //static matrix4 eulerYZX(float3 xyz);
        //static matrix4 eulerZXY(float3 xyz);
        //static matrix4 eulerZYX(float3 xyz);

        //static matrix4 eulerXYZ(float x, float y, float z);
        //static matrix4 eulerXZY(float x, float y, float z);
        //static matrix4 eulerYXZ(float x, float y, float z);
        //static matrix4 eulerYZX(float x, float y, float z);
        //static matrix4 eulerZXY(float x, float y, float z);
        //static matrix4 eulerZYX(float x, float y, float z);

        //static matrix4 euler(float3 xyz, RotationOrder order = RotationOrder::Default);
        //static matrix4 euler(float x, float y, float z, RotationOrder order = RotationOrder::Default);

        float determinant();
        void decompose(float3& position, float4& quaternion, float3& scale);
        float3 inverseMultiply(const float3& v);
        float3 multiply(const float3& v);
        float3 multiplyPoint3x4(const float3& point);

        matrix4& inverse();
    };
}
