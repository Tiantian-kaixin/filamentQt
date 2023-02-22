/*
 * Copyright (C) 2019 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef GLTFIO_ANIMATOR_H
#define GLTFIO_ANIMATOR_H

#include <gltfio/FilamentAsset.h>
#include <gltfio/FilamentInstance.h>
#include <map>
#if (__has_include(<cgltf/cgltf.h>))
#include <cgltf/cgltf.h>
#else
#include "cgltf.h"
#endif

namespace gltfio {

struct FFilamentAsset;
struct FFilamentInstance;
struct AnimatorImpl;

//Ace3DEngine add begin
struct Transform {
    filament::math::float3 translation{};
    filament::math::quatf rotation;
    filament::math::float3 scale{};
};
using TransformMap = std::map<utils::Entity, Transform>;
using MorphWeightsMap = std::map<utils::Entity, filament::math::float4>;

struct AnimationMap {
    TransformMap  transformMap;
    MorphWeightsMap weightsMap;
};

class AnimationConfig {
public:
    AnimationConfig() = default;
    AnimationConfig(int animatorIndex, float time, float blendDuration = 2.f, float weight = 0.f) : animatorIndex(animatorIndex),
                                                                                        time(time), weight(weight),
                                                                                        blendDuration(blendDuration){}
    int animatorIndex{};
    float startTime = 0;
    float time{};
    float endTime = 99999;
    float weight{};
    float blendDuration{};
    float animDuration = -1;
    void copy(std::shared_ptr<AnimationConfig> config) {
        this->animatorIndex = config->animatorIndex;
        this->blendDuration = config->blendDuration;
        this->animDuration = config->animDuration;
        this->time = config->time;
    }
};
//Ace3DEngine add end

/**
 * \class Animator Animator.h gltfio/Animator.h
 * \brief Updates matrices according to glTF \c animation and \c skin definitions.
 *
 * Animator can be used for two things:
 * - Updating matrices in filament::TransformManager components according to glTF \c animation definitions.
 * - Updating bone matrices in filament::RenderableManager components according to glTF \c skin definitions.
 *
 * For a usage example, see the documentation for AssetLoader.
 */
class Animator {
 public:
    /**
     * Applies rotation, translation, and scale to entities that have been targeted by the given
     * animation definition. Uses filament::TransformManager.
     *
     * @param animationIndex Zero-based index for the \c animation of interest.
     * @param time Elapsed time of interest in seconds.
     */
    void applyAnimation(size_t animationIndex, float time);

    /**
     * Computes root-to-node transforms for all bone nodes, then passes
     * the results into filament::RenderableManager::setBones.
     * Uses filament::TransformManager and filament::RenderableManager.
     *
     * NOTE: this operation is independent of \c animation.
     */
    void updateBoneMatrices(int type=0);

    /** Returns the number of \c animation definitions in the glTF asset. */
    size_t getAnimationCount() const;

    /** Returns the duration of the specified glTF \c animation in seconds. */
    float getAnimationDuration(size_t animationIndex) const;

    /**
     * Returns a weak reference to the string name of the specified \c animation, or an
     * empty string if none was specified.
     */
    const char* getAnimationName(size_t animationIndex) const;

    // Ace3DEngine add
    void blendAnimation(std::vector<AnimationConfig> animationWeights);
    void overlayAnimation(std::vector<std::tuple<int, float, float>> animationWeights);
    AnimationMap applyTransform(size_t animationIndex, float time) const;
    /** Mark whether to play morph animation */
    void isApplyMorphAnimationWeights(bool flag);
    /** get morph weights */
    filament::math::float4 getMorphAnimationWeights();

    bool isApplyMorphWeights = true;
    filament::math::float4 morphWeights =  filament::math::float4(0.0,0.0,0.0,0.0);
    std::vector<float> animationWeights;
    //Ace3DEngine add end

private:

    /*! \cond PRIVATE */
    friend struct FFilamentAsset;
    friend struct FFilamentInstance;
    /*! \endcond */

    Animator(FFilamentAsset* asset, FFilamentInstance* instance);
    Animator(FilamentAsset* publicAsset, cgltf_animation *animation);
    ~Animator();
    AnimatorImpl* mImpl;
};

}  // namespace gltfio

#endif  // GLTFIO_ANIMATOR_H
