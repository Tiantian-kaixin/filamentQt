//
// Created by ericwywang on 2020/1/19.
//

#ifndef TNT_ASSETHELPER_H
#define TNT_ASSETHELPER_H

#include <gltfio/FurComponent.h>
#include <gltfio/BloomComponent.h>
#include <gltfio/TextureManager.h>
#include <gltfio/FaceMeshComponent.h>
#include <gltfio/ReflectionComponent.h>
#include <gltfio/DynamicGlbComponent.h>
#include <gltfio/CgltfDataManager.h>
#include <gltfio/GeometryComponent.h>
#include <gltfio/DualFaceMeshComponent.h>
#include <gltfio/NormalCubemapComponent.h>
#include <gltfio/ResourceHelper.h>
#include <gltfio/HeadFeatherComponent.h>
#include <gltfio/material_util.h>
#include "TransparentComponent.h"
#include <gltfio/CustomShaderComponent.h>

#include <vector>
LIGHT3D_NAMESPACE_BEGIN

class AssetHelper {
private:
    filament::Engine *engine;
    filament::Scene *scene;
    utils::NameComponentManager* manager = nullptr;

    SMART_PTR<gltfio::MaterialProvider> materialProvider; // This is responsible to fetch ubershader materials
    SMART_PTR<MaterialManager> materialManager; // This is responsible to fetch assets materials and local materials
    SMART_PTR<gltfio::AssetLoader> assetLoader;
    SMART_PTR<ResourceHelper> resourceHelper;
    SMART_PTR<TextureManager> textureManager;

    SMART_PTR<TTFilamentSetupParam> param_ = nullptr;

    SMART_PTR<BloomComponent> bloomComponent;
    SMART_PTR<TransparentComponent> transparentComponent;
    SMART_PTR<FurComponent> furComponent;
    SMART_PTR<FaceMeshComponent> faceMeshComponent;
    SMART_PTR<GeometryComponent> geometryComponent;
    SMART_PTR<DualFaceMeshComponent> dualFaceMeshComponent;
    SMART_PTR<ReflectionComponent> reflectionComponent;
    SMART_PTR<DynamicGlbComponent> dynamicGlbComponent;
    SMART_PTR<HeadFeatherComponent> headFeatherComponent;
    SMART_PTR<NormalCubemapComponent> normalCubemapComponent;
    SMART_PTR<CustomShaderComponent> customShaderComponent;

    bool loadResourceAsync;
    void setupMesh(TTThreeDMaterialModel *materialModel);
    void setupCastShadow(TTThreeDMaterialModel *materialModel, gltfio::FilamentAsset* asset);
    void setupReceiveShadow(TTThreeDMaterialModel *materialModel, gltfio::FilamentAsset* asset);
    void setupBlendOrder(TTThreeDMaterialModel* materialModel, gltfio::FilamentAsset* asset);
    gltfio::FilamentAsset* CreateAsset(TTThreeDMaterialModel* model);

public:
    AssetHelper(filament::Engine *engine, filament::Scene *scene,
                SMART_PTR<TextureManager> textureManager,
                SMART_PTR<MaterialManager> materialManager, utils::Path resourcePath,
                bool loadResourceAsync);
    ~AssetHelper();

    std::vector<std::vector<gltfio::FilamentAsset *>> assets_for_each_person;

    void setTransform(SMART_PTR<TransformHelper> transformHelper, int userIndex, TTThreeDMaterialModel *materialModel,
            int materialIndex, filament::math::float3 headDiffScale, float posScale);

    void setTransform(filament::math::mat4f transform, int userIndex, int materialIndex);

    void setupMesh(std::vector<TTThreeDMaterialModel*>& materialModels, SMART_PTR<DynamicBoneHelper> dynamicBoneHelper, SMART_PTR<LIGHT3D::AnimatorHelper>);

    void loadFinish();

    void setTransparentComponent(SMART_PTR<TransparentComponent> component);

    void addDynamicBone(SMART_PTR<DynamicBoneHelper> dynamicBoneHelper, TTDynamicBoneParam* boneParam, int materialIndex);

    void setAnimator(SMART_PTR<LIGHT3D::AnimatorHelper> animatorHelper, int materialIndex);

    void setMorphWeight(int userIndex, int materialIndex, const std::string &name, const std::vector<gltfio::MorphWeight> &weights);

    // 更新眼珠旋转
    void updateEyeNodeEuler(uint32_t userIndex, uint32_t materialIndex, const std::string& nodeName, const filament::math::float3 euler);

    void updateNodeTransform(uint32_t userIndex, uint32_t materialIndex, const std::string &nodeName,
                             filament::math::float3 *scale, filament::math::float3 *translate,
                             filament::math::float3 *rotate);

    void setMaterialImage(int userIndex, int materialIndex, const std::string& name, const std::string& parameterName, filament::Texture* texture, bool hideEntity);

    void setMaterialImage(int userIndex, int materialIndex, const std::string& name, const std::string& parameterName, filament::Texture* texture);

    void setCurrentHeadCount(int n);

    void clearMesh();

    void updateCustomMaterial(std::vector<TTThreeDMaterialModel*>& materialModels);

    void setNormalComponent(SMART_PTR<NormalCubemapComponent> component);

    void initConfig(SMART_PTR<TTFilamentSetupParam> param, SMART_PTR<BloomComponent> bloom = nullptr, SMART_PTR<FurComponent> fur = nullptr,
            SMART_PTR<FaceMeshComponent> faceMesh = nullptr, SMART_PTR<DualFaceMeshComponent> dualFace = nullptr, SMART_PTR<GeometryComponent> geometry = nullptr,
            SMART_PTR<DynamicGlbComponent> dynamicGlb = nullptr, SMART_PTR<ReflectionComponent> reflection = nullptr, SMART_PTR<HeadFeatherComponent> headFeather = nullptr);

    void processAfterLoad();

    void updateReflectionParams(float zNormalScale, float blendFactor, filament::math::float2 reflectOffset, float modulateAlpha);
    void updateReflectionParams(std::vector<std::vector<TTReflectConfig*>>& reflect);

    void changeAssetDynamic(int* charmRange, int rangeCount, int* genders, int genderCount);
    void showOrHideAsset(int userIndex, int materialIndex, bool show);

    void showOrHideAssetEntity(int userIndex, int materialIndex, const std::string &entityName, bool show);

    bool addEntityWhenReady(SMART_PTR<TTFilamentSetupParam> param);
    
    void updateFaceBind(SMART_PTR<FaceBindHelper> helper, int userIndex, int materialIndex, TTThreeDMaterialModel *materialModel);
};

LIGHT3D_NAMESPACE_END

#endif //TNT_ASSETHELPER_H
