// Copyright (C) 2019 The Android Open Source Project
// Created by kevinxing on 2020-06-08.
//

#ifndef LIBS_GLTFIO_INCLUDE_GLTFIO_ANIMOJIMANAGER_H_
#define LIBS_GLTFIO_INCLUDE_GLTFIO_ANIMOJIMANAGER_H_


#include <gltfio/ModelDef.h>
#include <gltfio/FilamentAsset.h>
#include <gltfio/AssetHelper.h>
#include <gltfio/AssetHelperKapu.h>
#include <gltfio/common_util.h>

LIGHT3D_NAMESPACE_BEGIN

    class AnimojiManager {
    public:
        AnimojiManager(SMART_PTR<AssetHelper> assetList, SMART_PTR<TTFilamentSetupParam> param);

        AnimojiManager(SMART_PTR<AssetHelperKapu> assetHelper, SMART_PTR<TTFilamentSetupParam> param);

        void updateMorphWeights(int userIndex, float expressionWeights[], filament::math::float3 faceAngles);

        void updateMorphWeightsKapu(float expressionWeights[], int userIndex);

        void updateEyeEulerAngles(int userIndex, filament::math::float3 eulerAngles);

    private:
        SMART_PTR<AssetHelper> asset_helper_;
        SMART_PTR<AssetHelperKapu> asset_helper_kapu_;
        SMART_PTR<TTFilamentSetupParam> param_;
        std::map<std::string, float> jaw_open_threshold_map_;
        std::map<std::string, int> expName2Index;
        std::map<std::string, TTShapeRange> defaultExpressionList;
        std::vector<std::string> kapuOrderList;

        void initJawOpenThreshold();

        void adjustExpressionWeights(std::vector<SMART_PTR<TTExpressionConfig>> expressionConfigList,
                                     float *expressionWeights,
                                     float &jawOpenThreshold, filament::math::float3 faceAngles);

        void setMorphWeights(int userIndex, int materialIndex, TTNodeModel *node, float *expressionWeights);

        void updateEyeNodeEuler(int userIndex, int materialIndex, std::string nodeName,
                                filament::math::float3 eyeEulerAngles);

        void adjustValue(float *expressionWeights, int &index, float min, float max);

        float adjustValue(float in, float min, float max, float targetMin, float targetMax);

        void initExpressionIndexMap();

        void initDefaultExpressionValueMap();
    };

LIGHT3D_NAMESPACE_END

#endif  // LIBS_GLTFIO_INCLUDE_GLTFIO_ANIMOJIMANAGER_H_
