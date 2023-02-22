#pragma once
#include "loader/TMathHelper.h"

namespace T3D {
    void Vector3Normalize(const float* in, float* out);
    void Vector3Cross(const float* inV1, const float* inV2, float* out);
    float Vector3Length(const float* v);
    float Vector3Length(float x, float y, float z);
}
