//
// Created by ericwywang on 2020/1/21.
//

#ifndef TNT_ANIMATORMANAGER_H
#define TNT_ANIMATORMANAGER_H

#include <gltfio/Animator.h>
#include <gltfio/common_util.h>
#include <gltfio/AnimationComponent.h>
LIGHT3D_NAMESPACE_BEGIN
class AnimatorManager {
private:
    std::vector<gltfio::Animator*> animatorList; // weak reference. Don't release
    SMART_PTR<gltfio::AnimationConfig> lastConfig;
    SMART_PTR<gltfio::AnimationConfig> nextAnimConfig;
    void updateCurrentConfigState(SMART_PTR<gltfio::AnimationConfig> config, float time);

public:
    void addAnimator(gltfio::FilamentAsset* asset);

    void addAnimator(gltfio::FilamentAsset *asset, cgltf_animation* animation);

    void removeAnimator(gltfio::FilamentAsset *asset);

    void updateTime(LIGHT3D::AnimationComponent * animationComponent, float time, float fixedTime);

    void updateBoneMatrices(int type = 0); // type:0 普通3d素材 1:kapu boneMatrix 需要乘以 enitity 的 localTransform

    void clear();

    void forcePlay(float time, float fixedTime);

    void changeToAnimation(SMART_PTR<gltfio::AnimationConfig> config);

    void blendAnimation(const std::vector<gltfio::AnimationConfig>& animationWeights);

    void blendAnimationTest(gltfio::Animator* animator, float time, bool blend);

    float getAnimationBlendDuration(int animIndex, const std::vector<TTThreeDMaterialModel *>& models);
};

LIGHT3D_NAMESPACE_END
#endif //TNT_ANIMATORMANAGER_H
