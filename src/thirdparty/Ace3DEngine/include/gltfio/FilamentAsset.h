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

#ifndef GLTFIO_FILAMENTASSET_H
#define GLTFIO_FILAMENTASSET_H

#include <filament/Box.h>
#include <filament/TextureSampler.h>
#include <gltfio/FilamentInstance.h>

#include <utils/Entity.h>
#include <vector>
#include <map>
#include <tsl/robin_map.h>
#include <filament/VertexBuffer.h>
#include <filament/IndexBuffer.h>

#if (__has_include(<cgltf/cgltf.h>))
#include <cgltf/cgltf.h>
#else
#include "cgltf.h"
#endif


namespace filament {
    class Camera;
    class Engine;
    class MaterialInstance;
}

namespace gltfio {

struct MorphBinding;
struct MorphWeight;
class Animator;
class FilamentInstance;

// Encapsulates a connection between Texture and MaterialInstance.
struct TextureSlot {
  const cgltf_texture* texture;
  filament::MaterialInstance* materialInstance;
  const char* materialParameter;
  filament::TextureSampler sampler;
  bool srgb;
};

//Ace3DEngine add begin
//记录 Entity 对应的 glb node 信息。
struct SourceNodeInformation {
  // 如果entity对应的node是mesh节点，那么该值为对应mesh数组中的索引。
  int meshIndex = -1;
};
//Ace3DEngine add end

// Encapsulates VertexBuffer::setBufferAt() or IndexBuffer::setBuffer().
struct BufferSlot {
  const cgltf_accessor* accessor;
  cgltf_attribute_type attribute;
  int bufferIndex; // for vertex buffers only
  int morphTarget; // 0 if no morphing, otherwise 1-based index
  filament::VertexBuffer* vertexBuffer;
  filament::IndexBuffer* indexBuffer;
  const char* name;
};

/**
 * \class FilamentAsset FilamentAsset.h gltfio/FilamentAsset.h
 * \brief Owns a bundle of Filament objects that have been created by AssetLoader.
 *
 * For usage instructions, see the documentation for AssetLoader.
 *
 * This class owns a hierarchy of entities that have been loaded from a glTF asset. Every entity has
 * a filament::TransformManager component, and some entities also have \c Name, \c Renderable,
 * \c Light, or \c Camera components.
 *
 * In addition to the aforementioned entities, an asset has strong ownership over a list of
 * filament::VertexBuffer, filament::IndexBuffer, filament::MaterialInstance, filament::Texture,
 * and, optionally, a simple animation engine (gltfio::Animator).
 *
 * Clients must use ResourceLoader to create filament::Texture objects, compute tangent quaternions,
 * and upload data into vertex buffers and index buffers.
 *
 * \todo Only the default glTF scene is loaded, other glTF scenes are ignored.
 */
extern  void releaseCGLTFData(void* cgltfData);
class FilamentAsset {
public:

    /**
     * Gets the list of entities, one for each glTF node. All of these have a Transform component.
     * Some of the returned entities may also have a Renderable component and/or a Light component.
     */
    const utils::Entity* getEntities() const noexcept;
    const utils::Entity* getEntity(const char* name) const noexcept;
    const char* getNodeByEntity(const utils::Entity entity) const noexcept;

    // AceEngine added begin
    std::vector<filament::VertexBuffer*> getVertexVector() const noexcept;
    std::vector<BufferSlot> getBufferSlot() const noexcept;

    std::vector<TextureSlot> getTextureSlot() const noexcept;

    tsl::robin_map<utils::Entity, std::vector<int>> getBufferSlotEntityMap() const noexcept;

    tsl::robin_map<utils::Entity, tsl::robin_map<int, std::vector<int>>> getBufferSlotEntityPrimitiveMap() const noexcept;

    void releaseSourceAsset();
    void setMorphWeights(const std::string& nodeName, const std::vector<MorphWeight>& weights) noexcept;
    void updateMorphWeights(const std::string& nodeName, const std::vector<MorphWeight>& weights) noexcept;
    void setMorphWeights(const std::string& nodeName) noexcept;
    // AceEngine added end

    /**
     * Gets the number of entities returned by getEntities().
     */
    size_t getEntityCount() const noexcept;

    /**
     * Gets the list of entities in the scene representing lights. All of these have a Light component.
     */
    const utils::Entity* getLightEntities() const noexcept;

    /**
     * Gets the number of entities returned by getLightEntities().
     */
    size_t getLightEntityCount() const noexcept;

    /**
     * Gets the list of entities in the scene representing cameras. All of these have a \c Camera
     * component.
     *
     * Note about aspect ratios:
     * gltfio always uses an aspect ratio of 1.0 when setting the projection matrix for perspective
     * cameras. gltfio then sets the camera's scaling matrix with the aspect ratio specified in the
     * glTF file (if present).
     *
     * The camera's scaling matrix allows clients to adjust the aspect ratio independently from the
     * camera's projection.
     *
     * @see filament::Camera::setScaling
     */
    const utils::Entity* getCameraEntities() const noexcept;

    /**
     * Gets the number of entities returned by getCameraEntities().
     */
    size_t getCameraEntityCount() const noexcept;

    /**
     * Gets the transform root for the asset, which has no matching glTF node.
     *
     * This node exists for convenience, allowing users to transform the entire asset. For instanced
     * assets, this is a "super root" where each of its children is a root in a particular instance.
     * This allows users to transform all instances en masse if they wish to do so.
     */
    utils::Entity getRoot() const noexcept;

    /**
     * Pops a ready renderable off the queue, or returns 0 if no renderables have become ready.
     *
     * NOTE: To determine the progress percentage or completion status, please use
     * ResourceLoader#asyncGetLoadProgress. To get the number of ready renderables,
     * please use popRenderables().
     *
     * This method allows clients to progressively add the asset's renderables to the scene as
     * textures gradually become ready through asynchronous loading. For example, on every frame
     * progressive applications can do something like this:
     *
     *    while (utils::Entity e = popRenderable()) { scene.addEntity(e); }
     *
     * \see ResourceLoader#asyncBeginLoad
     * \see popRenderables()
     */
    utils::Entity popRenderable() noexcept;

    /**
     * Pops up to "count" ready renderables off the queue, or returns the available number.
     *
     * The given pointer should either be null or point to memory that can hold up to count
     * entities. If the pointer is null, returns the number of available renderables. Otherwise
     * returns the number of entities that have been written.
     *
     * \see ResourceLoader#asyncBeginLoad
     */
    size_t popRenderables(utils::Entity* entities, size_t count) noexcept;

    /** Gets all material instances. These are already bound to renderables. */
    const filament::MaterialInstance* const* getMaterialInstances() const noexcept;

    /** Gets all material instances (non-const). These are already bound to renderables. */
    filament::MaterialInstance* const* getMaterialInstances() noexcept;

    /** Gets the number of materials returned by getMaterialInstances(). */
    size_t getMaterialInstanceCount() const noexcept;

    /** Gets resource URIs for all externally-referenced buffers. */
    const char* const* getResourceUris() const noexcept;

    /** Gets the number of resource URIs returned by getResourceUris(). */
    size_t getResourceUriCount() const noexcept;

    /** Gets loading instructions for morph buffers. */
    size_t getMorphBindingCount() const noexcept;
    tsl::robin_map<std::string, std::vector<MorphBinding> *>& getMorphBindingsMap() noexcept;

    /** Gets the bounding box computed from the supplied min / max values in glTF accessors. */
    filament::Aabb getBoundingBox() const noexcept;

    /** Gets the NameComponentManager label for the given entity, if it exists. */
    const char* getName(utils::Entity) const noexcept;

    /** Returns the first entity with the given name, or 0 if none exist. */
    utils::Entity getFirstEntityByName(const char* name) noexcept;

    /**
     * Gets a list of entities with the given name.
     *
     * @param name Null-terminated string to match.
     * @param entities Pointer to an array to populate.
     * @param maxCount Maximum number of entities to retrieve.
     *
     * @return If entities is non-null, the number of entities written to the entity pointer.
     * Otherwise this returns the number of entities with the given name.
     */
    size_t getEntitiesByName(const char* name, utils::Entity* entities,
            size_t maxCount) const noexcept;

    /**
     * Gets a list of entities whose names start with the given prefix.
     *
     * @param prefix Null-terminated prefix string to match.
     * @param entities Pointer to an array to populate.
     * @param maxCount Maximum number of entities to retrieve.
     *
     * @return If entities is non-null, the number of entities written to the entity pointer.
     * Otherwise this returns the number of entities with the given prefix.
     */
    size_t getEntitiesByPrefix(const char* prefix, utils::Entity* entities,
            size_t maxCount) const noexcept;

    /**
     * Lazily creates the animation engine or returns it from the cache.
     *
     * The animator is owned by the asset and should not be manually deleted.
     * The first time this is called, it must be called before FilamentAsset::releaseSourceData().
     * If the asset is instanced, this returns a "master" animator that controls all instances.
     * To animate each instance individually, use \see FilamentInstance.
     */
    Animator* getAnimator() noexcept;

    Animator* getAnimator(cgltf_animation *animation) noexcept;

    void deleteAnimator() noexcept ;

    /**
     * Lazily creates a single LINES renderable that draws the transformed bounding-box hierarchy
     * for diagnostic purposes. The wireframe is owned by the asset so clients should not delete it.
     */
    utils::Entity getWireframe() noexcept;

    /**
     * Returns the Filament engine associated with the AssetLoader that created this asset.
     */
    filament::Engine* getEngine() const noexcept;

    /**
     * Reclaims CPU-side memory for URI strings, binding lists, and raw animation data.
     *
     * This should only be called after ResourceLoader::loadResources().
     * If using Animator, this should be called after getAnimator().
     */
    void releaseSourceData() noexcept;

    /**
     * Returns a weak reference to the underlying cgltf hierarchy. This becomes invalid after
     * calling releaseSourceData();
     */
    const void* getSourceAsset() noexcept;

    //AceEngine added
    std::shared_ptr<SourceNodeInformation> getSourceNodeInfoForEntity(utils::Entity entity);
    std::vector<gltfio::Skin> getSkins() const noexcept;

    /*! \cond PRIVATE */

    FilamentInstance** getAssetInstances() noexcept;
    size_t getAssetInstanceCount() const noexcept;

protected:
    FilamentAsset() noexcept = default;
    ~FilamentAsset() = default;

public:
    FilamentAsset(FilamentAsset const&) = delete;
    FilamentAsset(FilamentAsset&&) = delete;
    FilamentAsset& operator=(FilamentAsset const&) = delete;
    FilamentAsset& operator=(FilamentAsset&&) = delete;
    /*! \endcond */
};

struct MorphBaseData{
    uint32_t offset;      // byte count used only for vertex and index buffers
    uint32_t size;        // byte count used only for vertex and index buffers
    void** data;          // pointer to the resource data in the source asset (if loaded)
    std::vector<float> verticesData;
};

struct MorphWeight {
    bool enable;
    float weight;
};

/* Morph Bindings for calculate position in CPU */
struct MorphBinding {
    uint8_t bufferIndex;  // only used when the destination is a VertexBuffer
    filament::VertexBuffer* vertexBuffer;
    MorphBaseData baseData;
    std::vector<MorphBaseData> morphDatas;
    std::vector<float> result;
    int minMorphSize = -1;

    int getMinMorphSize() {
        if (minMorphSize >= 0) {
            return minMorphSize;
        }
        int minSize = baseData.verticesData.size();
        for(MorphBaseData &data : morphDatas){
            if(minSize > data.verticesData.size()){
                minSize = data.verticesData.size();
            }
        }
        minMorphSize = minSize;
        return minSize;
    }

    void updateMorphWeights(const std::vector<MorphWeight>& weights){
        int size = getMinMorphSize();
        int weightsSize = std::min(weights.size(), morphDatas.size());
        if(result.size() < size){
            result.resize(size);
        }
        memcpy(result.data(), baseData.verticesData.data(), result.size() * sizeof(float));
        for (size_t morphIndex = 0; morphIndex < weightsSize; morphIndex++) {
          if (!weights[morphIndex].enable) {
            continue;
          }
          for (size_t vertexIndex = 0; vertexIndex < size; vertexIndex++) {
            result[vertexIndex] += morphDatas[morphIndex].verticesData[vertexIndex] * weights[morphIndex].weight;
          }
        }
    }

    void setMorphWeights(filament::Engine *engine){
        vertexBuffer->setBufferAt(*engine, bufferIndex,
                                  filament::VertexBuffer::BufferDescriptor(result.data(), result.size() * sizeof(float)));
    }
};

} // namespace gltfio

#endif // GLTFIO_FILAMENTASSET_H
