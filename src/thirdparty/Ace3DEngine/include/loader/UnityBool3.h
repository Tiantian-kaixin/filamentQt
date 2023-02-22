#pragma once

namespace UM {
    struct bool3 {
        bool x = false;
        bool y = false;
        bool z = false;

        bool3(bool _x, bool _y, bool _z): x(_x), y(_y), z(_z) { }
        //bool3(const bool3& xyz): x(xyz.x), y(xyz.y), z(xyz.z) { }
        //bool3(bool b): x(b), y(b), z(b) { }
    };
}
