//
// Created by ericwywang on 2020/1/17.
//

#ifndef TNT_FURCOMPONENT_H
#define TNT_FURCOMPONENT_H

#include <gltfio/Component.h>
#include <gltfio/TextureManager.h>
#include <gltfio/ModelDef.h>
#include <gltfio/MaterialManager.h>
#include <gltfio/material_util.h>
#include <gltfio/FilamentInstance.h>
#include <gltfio/FaceBindHelper.h>

LIGHT3D_NAMESPACE_BEGIN

using InstanceHandle = gltfio::FilamentInstance*;

class FurComponent : Component {
private:
    filament::Engine *engine_ = nullptr;
    filament::Scene *scene_ = nullptr;
    filament::Material *furMaterial_= nullptr;
    SMART_PTR<TTFilamentSetupParam> param_;
    InstanceHandle* instances_;

public:
    FurComponent(filament::Engine *engine, filament::Scene *scene);
    ~FurComponent();

    void initConfig(SMART_PTR<MaterialManager> materialManager,
                    SMART_PTR<TTFilamentSetupParam> param, int maxFaceCount);

    void setup(TTThreeDMaterialModel *materialModel, std::vector<gltfio::FilamentAsset *> *batchList, int faceIndex,
               SMART_PTR<gltfio::AssetLoader> assetLoader, SMART_PTR<ResourceHelper> resourceHelper, SMART_PTR<TextureManager> textureLoader) override;

    void setMorphWeight(int userIndex, int materialIndex, const std::string &name,
                        const std::vector<gltfio::MorphWeight> &weights) override;

    void setMaterialImage(int userIndex, int materialIndex, const std::string& name,
                          const std::string& parameterName, filament::Texture *texture,
                          bool hideEntity) override;
    void setMaterialImage(int userIndex, int materialIndex, const std::string& name,
                          const std::string& parameterName, filament::Texture *texture);

    void updateParams(float now, TTThreeDMaterialModel *materialModel,
                      int materialIndex, bool onlyTime = true);

    void setTransform(filament::math::mat4f& transform, int userIndex, int materialIndex) override;

    void addDynamicBone(SMART_PTR<DynamicBoneHelper> dynamicBoneHelper, int userIndex,
                        TTDynamicBoneParam* boneParam, int materialIndex) override;

    void setAnimator(SMART_PTR<LIGHT3D::AnimatorHelper> animatorHelper, int userIndex, int materialIndex) override ;

    void setCurrentHeadCount(int n) override ;

    void clearMesh(SMART_PTR<gltfio::AssetLoader> assetLoader);

    std::vector<TTNodeModel *> furMap;
    std::vector<std::vector<std::vector<gltfio::FilamentInstance*>>> furList;
    std::vector<std::vector<gltfio::FilamentAsset*>> furAssetList;
    std::vector<filament::MaterialInstance*> matList;

    void showOrHideAsset(int userIndex, int materialIndex, bool show);

    void showOrHideAssetEntity(int userIndex, int materialIndex,
                               const std::string &entityName, bool show);

    void updateMatParams(filament::MaterialInstance *mat, const TTNodeModel *node, float offset) const;

    void updateFaceBind(SMART_PTR<FaceBindHelper> helper, int userIndex, int materialIndex, TTThreeDMaterialModel *materialModel);
};

LIGHT3D_NAMESPACE_END

#endif //TNT_FURCOMPONENT_H
