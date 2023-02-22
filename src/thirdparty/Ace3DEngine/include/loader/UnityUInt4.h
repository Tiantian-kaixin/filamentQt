#pragma once
#include <math.h>

namespace UM {
    struct uint4 {
        unsigned int x = 0;
        unsigned int y = 0;
        unsigned int z = 0;
        unsigned int w = 0;

        //uint4() {}
        uint4(unsigned int _x, unsigned int _y, unsigned int _z, unsigned int _w): x(_x), y(_y), z(_z), w(_w) {}
        //uint4(const uint4& rhs): x(rhs.x), y(rhs.y), z(rhs.z), w(rhs.w) {}

        friend uint4 operator*(const uint4& lhs, unsigned int rhs) { return uint4(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs); }
        friend uint4 operator*(const uint4& lhs, const uint4& rhs) { return uint4(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w); }
        friend uint4 operator*(unsigned int lhs, const uint4& rhs) { return uint4(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z, lhs * rhs.w); }

        friend uint4 operator^(const uint4& lhs, unsigned int rhs) { return uint4(lhs.x ^ rhs, lhs.y ^ rhs, lhs.z ^ rhs, lhs.w ^ rhs); }
        friend uint4 operator^(const uint4& lhs, const uint4& rhs) { return uint4(lhs.x ^ rhs.x, lhs.y ^ rhs.y, lhs.z ^ rhs.z, lhs.w ^ rhs.w); }
        friend uint4 operator^(unsigned int lhs, const uint4& rhs) { return uint4(lhs ^ rhs.x, lhs ^ rhs.y, lhs ^ rhs.z, lhs ^ rhs.w); }

        friend uint4 operator&(const uint4& lhs, unsigned int rhs) { return uint4(lhs.x & rhs, lhs.y & rhs, lhs.z & rhs, lhs.w & rhs); }
        friend uint4 operator&(const uint4& lhs, const uint4& rhs) { return uint4(lhs.x & rhs.x, lhs.y & rhs.y, lhs.z & rhs.z, lhs.w & rhs.w); }
        friend uint4 operator&(unsigned int lhs, const uint4& rhs) { return uint4(lhs & rhs.x, lhs & rhs.y, lhs & rhs.z, lhs & rhs.w); }
    };
}
