//
// Created by ericwywang on 2020/1/20.
//

#ifndef TNT_ANIMATORHELPER_H
#define TNT_ANIMATORHELPER_H

#include <set>
#include <gltfio/common_util.h>
#include <gltfio/AnimatorManager.h>
#include <gltfio/AnimationComponent.h>

LIGHT3D_NAMESPACE_BEGIN

struct AnimWeightConfig {
    int animationIndex;
    float weight;
    AnimWeightConfig(int animationIndex, float weight) {
        this->animationIndex = animationIndex;
        this->weight = weight;
    }
};

class AnimatorHelper {
 private:
    filament::Engine *mEngine;
    std::vector<std::vector<LIGHT3D::AnimatorManager>> manager_for_each_person;
    int currentFaceCount;
    SMART_PTR<LIGHT3D::AnimationComponent> animationComponent = MAKE_SMART_PTR<AnimationComponent>();
    std::vector<TriggerableAnimationItem> animationItemList;

 public:
    AnimatorHelper(filament::Engine *engine);

    void setupManager(size_t maxFaceCount, size_t materialSize);

    void addAnimator(int userIndex, int materialIndex, gltfio::FilamentAsset *asset);

    void addAnimator(int userIndex, int materialIndex, gltfio::FilamentAsset* asset, cgltf_animation *animation);

    void clearManager(int userIndex, int materialIndex);

    void setCurrentFaceCount(int value) {
        currentFaceCount = value;
    }

    void updateTime(float time, SMART_PTR<TTFilamentSetupParam> param);

    void changToAnimation(int animIndex, float time, SMART_PTR<TTFilamentSetupParam> param);

    std::vector<AnimWeightConfig> getAnimationWeights(float degreeXY, float degreeZ, const std::vector<TriggerableAnimationItem>& animationList);

    void changeAnimationToDegree(int materialIndex, float degreeXY, float degreeZ, float time, const std::vector<TriggerableAnimationItem>& animationList);

    void updateBoneMatrices(int type = 0);

    void reset(SMART_PTR<TTFilamentSetupParam> param);

    void registerAnimation(std::vector<std::string> &nameList);

    void updateAnimationTrigger(double renderTime, std::set<std::string> &nameList);

    void playAnimation(double renderTime, const std::string& name, int count);

    bool isAnimationRunning(std::string &name);

    void stopAnimation(const std::string& name);

    filament::math::float2 getAnimationTime(const std::string& name);
};
LIGHT3D_NAMESPACE_END

#endif  // TNT_ANIMATORHELPER_H
