#pragma once
#include <math.h>
#include "loader/UnityBool3.h"

namespace UM {
    struct int3 {
        int x = 0;
        int y = 0;
        int z = 0;

        //int3(int _x, int _y, int _z) : x(_x), y(_y), z(_z) { }
        explicit int3(bool b) : x(b ? 1 : 0), y(b ? 1 : 0), z(b ? 1 : 0) { }
        explicit int3(bool3 b) : x(b.x ? 1 : 0), y(b.y ? 1 : 0), z(b.z ? 1 : 0) { }
        //int3(const int3& rhs) { x = rhs.x; y = rhs.y; z = rhs.z; }
        int3& operator-() { x = -x; y = -y; z = -z; return *this; }
    };
}
