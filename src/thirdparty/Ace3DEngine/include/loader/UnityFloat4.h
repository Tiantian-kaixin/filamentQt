#pragma once
#include <math.h>

namespace UM {
    struct float3;
    struct matrix3;

    struct float4 {
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
        float w = 0.0f;

        float4();
        explicit float4(float a);
        float4(const float4& f);
        explicit float4(float* f);
        float4(float _x, float _y, float _z, float _w);
        explicit float4(bool b);
        float4(const float3& f, float a);

        float4& operator-();
        bool equal(const float4& rhs);

        float& operator[] (size_t index);
        const float& operator[] (size_t index) const;

        float length();
        float4 normalize();
        float4 conjugate();

        // returns the inverse of q.
        // * if q is normalized. The inverse quaternions is also normalized
        // * it is legal to call quatConj with non-normalized quaternion, the output will also be non-normalized
        float4 inverse();

        float3 multiplyFloat3(const float3& u);
        float3 inverseMultiplyFloat3(const float3& v);

        void setFromRotationMatrix3(const matrix3& m);

        float4 multiplyQuaternion(const float4& quaternion);
        matrix3 toMatrix3();

        friend float4 operator*(const float4& lhs, float rhs);
        friend float4 operator*(const float4& lhs, const float4& rhs);
        friend float4 operator*(float lhs, const float4& rhs);

        friend float4 operator-(const float4& lhs, const float4& rhs);
        friend float4 operator-(const float4& lhs, float rhs);
        friend float4 operator-(float lhs, const float4& rhs);

        friend float4 operator+(const float4& lhs, const float4& rhs);
        friend float4 operator+(const float4& lhs, float rhs);
        friend float4 operator+(float lhs, const float4& rhs);

        //friend float4 operator/(const float4& lhs, const float4& rhs);
        //friend float4 operator/(const float4& lhs, float rhs);
        //friend float4 operator/(float lhs, const float4& rhs);

        float4 multiplyQuat(const float4& quaternion);
    };
}
