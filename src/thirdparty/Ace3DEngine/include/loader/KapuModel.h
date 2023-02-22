#ifndef __KAPU_MODEL_H__
#define __KAPU_MODEL_H__

#include "loader/kapu_struct.h"

struct KapuSkeleton {
    filament::math::float3 scale;
    filament::math::float3 position;
};

struct KapuModel {
    bool needUpdate = false;
    std::map<std::string, float> blend_shapes;
    std::map<std::string, KapuSkeleton> skeletons;
    std::map<std::string, filament::math::float3> hsvs;
};

KapuModel* parseParams(kapu_data* sourceAsset);

#endif
