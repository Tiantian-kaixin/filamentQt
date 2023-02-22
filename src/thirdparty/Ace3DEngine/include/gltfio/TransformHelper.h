//
// Created by ericwywang on 2020/1/20.
//

#ifndef TNT_TRANSFORMHELPER_H
#define TNT_TRANSFORMHELPER_H

#include <gltfio/common_util.h>
#include <gltfio/ModelDef.h>
LIGHT3D_NAMESPACE_BEGIN
class TransformHelper {
 private:
    std::vector<filament::math::mat4f> transformList;
    std::vector<filament::math::mat4f> oldTransformList;
    filament::math::float3 sScale =
        filament::math::float3(1.0f, -1.0f, -1.0f);  // 用于转换优图的坐标系，他们给的是反的
    filament::math::float3 headScale = filament::math::float3(1.0f, 1.0f, 1.0f);
    float alpha = 0.0;
    int currentFaceCount;

 public:
    void setHeadScale(filament::math::float3 scale);

    void setAdjustAlpha(float adjustAlpha);

    void setFaceCount(int faceCount);

    void setTransform(const filament::math::mat4f &transform, int userIndex);

    // headDiffScale: 基准头和当前头的scale比例
    filament::math::mat4f updateTransform(filament::Engine *engine, int userIndex,
                                          TTThreeDMaterialModel *materialModel,
                                          gltfio::FilamentAsset *asset,
                                          filament::math::float3 headDiffScale,
                                          float posScale);

    void updateTransformForHead(filament::Engine *engine, int userIndex, utils::Entity head);

    void updateTransformForSmallHead(filament::Engine *engine, int userIndex, utils::Entity head,
                                     filament::math::float3 scale);

    filament::math::mat4f getHeadTransform(int userIndex);
};
LIGHT3D_NAMESPACE_END

#endif //TNT_TRANSFORMHELPER_H
