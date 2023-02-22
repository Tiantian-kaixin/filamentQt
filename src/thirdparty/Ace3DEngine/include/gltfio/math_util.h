//
//  TTMatixUtil.h
//  GXSceneTest
//
//  Created by gennyxu on 2019/7/12.
//  Copyright © 2019 ddd. All rights reserved.
//
#ifndef TT_MATRIXUTIL_H
#define TT_MATRIXUTIL_H
#include <math/vec3.h>
#include <math/vec4.h>
#include <math/mat4.h>
#include <math/TQuatHelpers.h>
#include "gltfio/common_util.h"

LIGHT3D_NAMESPACE_BEGIN
namespace math {

long CurrentTimestampMillis();
filament::math::float3 multiplyPoint3x4(filament::math::float3 vec, filament::math::mat4f matrix);
filament::math::float3 transformDirection(filament::math::float3 vec, filament::math::mat4f matrix);
filament::math::float3 cross2(const filament::math::float3 u, const filament::math::float3& v);
float calVecAngle(filament::math::float3 from, filament::math::float3 to);
filament::math::float4 quatInverse(filament::math::float4 quat);
filament::math::float4 fromToRotation(filament::math::float3 from, filament::math::float3 to);
filament::math::float3 transformQuat(filament::math::float4 rotation, filament::math::float3 vec);
filament::math::float4 quatLerp(filament::math::float4 quaternion1, filament::math::float4 quaternion2, float amount);
filament::math::float4 mulRotation(filament::math::float4 rot1, filament::math::float4 rot2);
filament::math::float4 quatMultiply(filament::math::float4 lhs, filament::math::float4 rhs);
bool isRaysIntersectRectPlane(filament::math::float3 rayDirectionVec,
    filament::math::float3 rayStartPoint,
    filament::math::float3 lt,
    filament::math::float3 rt,
    filament::math::float3 rb,
    filament::math::float3 lb);
bool isRayIntersectWithAABB(filament::math::float3 rayDirectionVec,
    filament::math::float3 rayStartPoint,
    filament::math::float3 aabbMin,
    filament::math::float3 aabbMax);
filament::math::quatf eulerZXYToQuaternion(double z, double x, double y);
// 本文件的欧拉角XYZ，X指Pitch单位是弧度，Y指Yaw单位是弧度，Z指Roll单位是弧度

// 将四元数转成欧拉角ZXY的顺序，返回值里x是x轴的旋转弧度，y是y轴的旋转弧度，z是z轴的旋转弧度
filament::math::float3 convertSimdQuaternionToEulerZXY(filament::math::quatf quaternion);

// 将欧拉角ZXY顺序的角度转成四元数
filament::math::quatf eulerZXYToQuaternion(double z, double x, double y);

// 将欧拉角YXZ顺序的角度转成四元数
filament::math::quatf eulerYXZToQuaternion(double y, double x, double z);


void decomposeMatrix(const filament::math::mat4f& mat, filament::math::float3* translation,
                     filament::math::quatf* rotation, filament::math::float3* scale);

filament::math::mat4f composeMatrix(const filament::math::float3& translation,
                                    const filament::math::quatf& rotation, const filament::math::float3& scale);

}
LIGHT3D_NAMESPACE_END

#endif