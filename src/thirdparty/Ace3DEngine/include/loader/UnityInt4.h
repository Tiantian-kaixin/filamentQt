#pragma once
#include <math.h>

namespace UM {
    struct int4 {
        int x = 0;
        int y = 0;
        int z = 0;
        int w = 0;

        //int4(int _x, int _y, int _z, int _w) : x(_x), y(_y), z(_z), w(_w) { }
        explicit int4(bool b) : x(b ? 1 : 0), y(b ? 1 : 0), z(b ? 1 : 0), w(b ? 1 : 0) { }
        //int4(const int4& rhs) { x = rhs.x; y = rhs.y; z = rhs.z; w = rhs.w; }
        int4& operator-() { x = -x; y = -y; z = -z; w = -w; return *this; }
    };
}
