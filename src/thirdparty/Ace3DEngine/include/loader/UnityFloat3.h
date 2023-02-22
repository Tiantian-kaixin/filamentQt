#pragma once
#include <math.h>

namespace UM {
    struct float4;
    struct bool3;
    struct matrix4;

    struct float3 {
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;

        float3();
        explicit float3(float a);
        explicit float3(const float4& f);
        explicit float3(float* f);
        float3(const float3& rhs);
        float3(float _x, float _y, float _z);
        explicit float3(bool b);
        explicit float3(bool3 v);

        float3& operator-();
        float3& operator-=(const float3& rhs);
        float3& operator+=(const float3& rhs);
        bool equals(const float3& rhs);

        float& operator[] (size_t index);
        const float& operator[] (size_t index) const;

        float length();
        float sqrLength();

        float3 normalize();

        float dot(const float3& v) const;

        float3 cross(const float3& v) const;

        float4 multiplyQuaternion(const float4& quaternion);

        friend float3 operator*(const float3& lhs, float rhs);
        friend float3 operator*(const float3& lhs, const float3& rhs);
        friend float3 operator*(float lhs, const float3& rhs);

        friend float3 operator-(const float3& lhs, const float3& rhs);
        friend float3 operator-(const float3& lhs, float rhs);
        friend float3 operator-(float lhs, const float3& rhs);

        friend float3 operator+(const float3& lhs, const float3& rhs);
        friend float3 operator+(const float3& lhs, float rhs);
        friend float3 operator+(float lhs, const float3& rhs);

        //friend float3 operator/(const float3& lhs, const float3& rhs);
        //friend float3 operator/(const float3& lhs, float rhs);
        //friend float3 operator/(float lhs, const float3& rhs);

        friend bool3 operator<(const float3& lhs, const float3& rhs);
        friend bool3 operator<(const float3& lhs, float rhs);
        friend bool3 operator<(float lhs, const float3& rhs);

        //friend bool3 operator<=(const float3& lhs, const float3& rhs);
        //friend bool3 operator<=(const float3& lhs, float rhs);
        //friend bool3 operator<=(float lhs, const float3& rhs);

        //friend bool3 operator>(const float3& lhs, const float3& rhs);
        //friend bool3 operator>(const float3& lhs, float rhs);
        //friend bool3 operator>(float lhs, const float3& rhs);

        //friend bool3 operator>=(const float3& lhs, const float3& rhs);
        //friend bool3 operator>=(const float3& lhs, float rhs);
        //friend bool3 operator>=(float lhs, const float3& rhs);

        friend float3 abs(const float3& rhs);
        friend float3 mad(const float3& a, const float3& b, const float3& c);

        float3& applyMatrix4(const matrix4& m);
    };
}
