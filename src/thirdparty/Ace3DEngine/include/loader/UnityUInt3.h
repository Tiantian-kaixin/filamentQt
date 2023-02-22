#pragma once
#include <math.h>

namespace UM {
    struct uint3 {
        unsigned int x = 0;
        unsigned int y = 0;
        unsigned int z = 0;

        //uint3() {}
        uint3(unsigned int _x, unsigned int _y, unsigned int _z): x(_x), y(_y), z(_z) {}
        //uint3(const uint3& rhs): x(rhs.x), y(rhs.y), z(rhs.z) {}

        friend uint3 operator*(const uint3& lhs, unsigned int rhs) { return uint3(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs); }
        friend uint3 operator*(const uint3& lhs, const uint3& rhs) { return uint3(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z); }
        friend uint3 operator*(unsigned int lhs, const uint3& rhs) { return uint3(rhs.x * lhs, rhs.y * lhs, rhs.z * lhs); }

        friend uint3 operator^(const uint3& lhs, unsigned int rhs) { return uint3(lhs.x ^ rhs, lhs.y ^ rhs, lhs.z ^ rhs); }
        friend uint3 operator^(const uint3& lhs, const uint3& rhs) { return uint3(lhs.x ^ rhs.x, lhs.y ^ rhs.y, lhs.z ^ rhs.z); }
        friend uint3 operator^(unsigned int lhs, const uint3& rhs) { return uint3(lhs ^ rhs.x, lhs ^ rhs.y, lhs ^ rhs.z); }
    };
}
