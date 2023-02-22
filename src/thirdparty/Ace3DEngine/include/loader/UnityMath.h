#pragma once

namespace UM {
    enum RotationOrder {
        XYZ,
        XZY,
        YXZ,
        YZX,
        ZXY,
        ZYX,
        Default = ZXY
    };

    struct float3;
    struct float4;
    struct uint3;
    struct uint4;
    struct int3;
    struct int4;

    float csum(float3 x);

    float asfloat(const int& x);
    int asint(const float& x);

    uint3 asuint(float3 x);
    uint4 asuint(float4 x);
    float3 asfloat(uint3 x);
    float4 asfloat(uint4 x);

    //void sincos(float x, float& s, float& c);
    //void sincos(float3 x, float3& s, float3& c);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    float bitselect(float a, float b, int c);
    float select(float a, float b, int c);
    float3 select(const float3& a, const float3& b, const int3& c);
    float4 select(const float4& a, const float4& b, const int4& c);
    float3 cond(bool c, const float3& a, const float3& b);
    float4 cond(bool c, const float4& a, const float4& b);
    float sign(float x);
    float convert_float(int v);
    float3 sign(const float3& x);
    float4 sign(const float4& x);

    // chgsign: change sign
    // return value: msb(y) ? -x : x
    float chgsign(float x, float y);
    float3 chgsign(const float3& x, const float3& y);
    float4 chgsign(const float4& x, const float4& y);

    inline float epsilon_scale() { return 1e-9f; };
    float3 inverseScale(float3 const &s, float epsilon = epsilon_scale());

    //
    //  rcp: reciprocal
    //  return value: 1/x
    //  note: the return value for NaNs are left to the implementation
    //
    inline float rcp(float x) { return 1.0f / x; }
    float4 rcp(const float4 &x);
    float3 rcp(const float3 &x);
}
