#pragma once
#include <math.h>
#include "UnityFloat3.h"
#include "UnityMath.h"

namespace UM {
    struct float4;

    struct matrix3 {
        float3 c0;
        float3 c1;
        float3 c2;

        static matrix3 identity;

        matrix3();
        matrix3(float3 _c0, float3 _c1, float3 _c2);
        matrix3(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22);
        explicit matrix3(float v);
        explicit matrix3(const float* v);
        explicit matrix3(const float4& quaternion);

        float& operator[](size_t index);
        const float& operator[](size_t index) const;

        bool equals(const matrix3& rhs);

        matrix3 inverse(matrix3 m);
        float determinant(matrix3 m);

        matrix3& setFromToRotation(const float3& from, const float3& to);
        float4 toQuaternion();

        float getElement(size_t col, size_t index);
        matrix3 multiplyScale(const float3& s);
        float3 multiplyFloat3(const float3& v);

        friend matrix3 operator*(const matrix3& lhs, const matrix3& rhs);
        friend matrix3 operator*(const matrix3& lhs, float rhs);
        friend matrix3 operator*(float lhs, const matrix3& rhs);

        friend matrix3 operator+(const matrix3& lhs, const matrix3& rhs);
        friend matrix3 operator+(const matrix3& lhs, float rhs);
        friend matrix3 operator+(float lhs, const matrix3& rhs);

        friend matrix3 operator-(const matrix3& lhs, const matrix3& rhs);
        friend matrix3 operator-(const matrix3& lhs, float rhs);
        friend matrix3 operator-(float lhs, const matrix3& rhs);

        //static matrix3 axisAngle(float3 axis, float angle);

        //static matrix3 eulerXYZ(float3 xyz);
        //static matrix3 eulerXZY(float3 xyz);
        //static matrix3 eulerYXZ(float3 xyz);
        //static matrix3 eulerYZX(float3 xyz);
        //static matrix3 eulerZXY(float3 xyz);
        //static matrix3 eulerZYX(float3 xyz);

        //static matrix3 eulerXYZ(float x, float y, float z);
        //static matrix3 eulerXZY(float x, float y, float z);
        //static matrix3 eulerYXZ(float x, float y, float z);
        //static matrix3 eulerYZX(float x, float y, float z);
        //static matrix3 eulerZXY(float x, float y, float z);
        //static matrix3 eulerZYX(float x, float y, float z);

        //static matrix3 euler(float3 xyz, RotationOrder order = RotationOrder::Default);
        //static matrix3 euler(float x, float y, float z, RotationOrder order = RotationOrder::Default);

    };
}
