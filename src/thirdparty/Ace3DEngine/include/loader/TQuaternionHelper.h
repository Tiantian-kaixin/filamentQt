#pragma once
#include "loader/TMathHelper.h"
#include "loader/UnityFloat3.h"
#include "loader/UnityFloat4.h"

namespace T3D {
    void QuaternionSetFromRotationMatrix3(const float* matrix3, float* outQuaternion);

    /*
    UM::float4 QuaternionFromToRotation(const UM::float3& inV3From, const UM::float3& inV3To);
    UM::float4 QuaternionFromMatrix3x3(const float* inMatrix3x3);
    UM::float4 QuaternionMultiplyQuaternion(const UM::float4& inQ1, const UM::float4& inQ2);
    void QuaternionMultiplyVector3(const float* inQuaternion, const float* inV, float* outV);

    UM::float4 quatConj(const UM::float4& q);
    UM::float3 quatMulVec(const UM::float4& q, const UM::float3& u);
    UM::float3 quatInvMulVec(const UM::float4 &q, const UM::float3 &v);
    // */
}
