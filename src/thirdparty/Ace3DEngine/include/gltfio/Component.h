//
// Created by ericwywang on 2020/1/21.
//

#ifndef TNT_COMPONENT_H
#define TNT_COMPONENT_H

#include "TransformHelper.h"
#include "DynamicBoneHelper.h"
#include "AnimatorHelper.h"
#include <gltfio/ModelDef.h>
#include <gltfio/TextureManager.h>
#include <gltfio/ResourceHelper.h>

LIGHT3D_NAMESPACE_BEGIN
class Component {
public:
    virtual void setup(TTThreeDMaterialModel* materialModel, std::vector<gltfio::FilamentAsset *> *batchList, int faceIndex,
                       SMART_PTR<gltfio::AssetLoader> assetLoader, SMART_PTR<ResourceHelper> resourceHelper, SMART_PTR<TextureManager> textureLoader) = 0;

    virtual void setMorphWeight(int userIndex, int materialIndex, const std::string& name,
                                const std::vector<gltfio::MorphWeight>& weights) = 0;

    virtual void setMaterialImage(int userIndex, int materialIndex, const std::string& name,
                                  const std::string& parameterName, filament::Texture* texture,
                                  bool hideEntity) = 0;

    virtual void setTransform(filament::math::mat4f& transform, int userIndex,
                              int materialIndex) = 0;

    virtual void addDynamicBone(SMART_PTR<DynamicBoneHelper> dynamicBoneHelper, int userIndex,
                                TTDynamicBoneParam* boneParam, int materialIndex) = 0;

    virtual void setAnimator(SMART_PTR<LIGHT3D::AnimatorHelper> animatorHelper, int userIndex, int materialIndex) = 0;

    virtual void setCurrentHeadCount(int n) = 0;
};
LIGHT3D_NAMESPACE_END
#endif //TNT_COMPONENT_H
