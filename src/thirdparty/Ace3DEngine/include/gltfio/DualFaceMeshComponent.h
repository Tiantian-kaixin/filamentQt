//
// Created by ericwywang on 2020/5/14.
//

#ifndef TNT_DUALFACEMESHCOMPONENT_H
#define TNT_DUALFACEMESHCOMPONENT_H

#include <gltfio/ResourceHelper.h>
#include <gltfio/ModelDef.h>
#include <gltfio/gl_util.h>
#include "MaterialManager.h"
#include "gltfio/GeometryComponent.h"


#define ENABLE_DUALFACE_COMPONENT 1
#define kDebugFaceTexture 0
#define ENABLE_DEBUG_FACE_MESH 0

LIGHT3D_NAMESPACE_BEGIN

class DualFaceMeshComponent {
 private:
    filament::Engine *mEngine;
    filament::Scene *scene;

    filament::Material *mixMat_;
    utils::Path resourcePath_;
    utils::Path materialDir_;

    filament::math::float4 animatorWeights_ = filament::math::float4(0, 0, 0, 0);
    filament::Material *locationInitMat_ = nullptr;
    std::vector<filament::MaterialInstance *> matList_;
    int width;
    int height;
    int fov_;
    float dualScale_;
    std::map<utils::Entity, gl_util::Framebuffer> bufferMap_;
    std::map<utils::Entity, gltfio::FilamentAsset *> assetMap_;
    std::map<utils::Entity, gltfio::FilamentAsset *> dualMap_;

    std::vector<int> staticVerticesMap_,
            dynamicVerticesMap_,
            tessellationVerticesMap_;

    float *baseModelVertex_ = nullptr;
    float *detectVertexProj_ = nullptr;
    int baseModelVertexCount_ = 0;
    bool isCanUpdate_ = false;
    int faceMeshType_ = 0;

 public:
    ~DualFaceMeshComponent();
    DualFaceMeshComponent(filament::Engine* engine, filament::Scene* scene, const utils::Path& resourcePath, utils::Path& materialDir);

    void setup(TTThreeDMaterialModel *materialModel, gltfio::FilamentAsset *asset, SMART_PTR<gltfio::AssetLoader> assetLoader,
               SMART_PTR<ResourceHelper> resourceHelper);

    void updateAssetAnimatorWeights(filament::math::float4 weights);

    float *getAssetCurrentVertex();

    void resize(int width, int height, int fov);

    void initOffScreen();

    void setupFaceMaterial();

    void initConfig(SMART_PTR<MaterialManager> materialManager, int width, int height, int fov,
                    float scale);

    void setFaceMesh(filament::Texture *texture);

    void setMvpMatrix(filament::math::mat4f &orthMatrix,
                      filament::math::mat4f &perspectiveMatrix,
                      filament::math::mat4f &modelMatrix, float scale);

    void render(filament::Renderer *renderer);

    void setTransform(filament::math::mat4f &transform);

    void updateTransform();

    void updateVert2(float staticAlpha, const float *vert, const float *vert2,
                     filament::math::mat4f &modelConvertMatrix,
                     filament::math::mat4f &perspectiveMatrix, filament::math::mat4f &modelMatrix);

    void loopSlots(std::vector<gltfio::BufferSlot> &slots, gltfio::MorphBinding &morphBinding, const float *vert,
                   const float *vert2, filament::math::mat4f &modelConvertMatrix,
                   filament::math::mat4f &perspectiveMatrix, filament::math::mat4f &modelMatrix);

    void loopSlotsInner(gltfio::BufferSlot &slot, gltfio::MorphBinding &morphBinding, const float *vert,
                        const float *vert2, filament::math::mat4f &modelConvertMatrix,
                        filament::math::mat4f &perspectiveMatrix, filament::math::mat4f &modelMatrix);

    void updateVert2Inner(gltfio::FilamentAsset *asset, float staticAlpha, const float *vert, const float *vert2,
                          filament::math::mat4f &modelConvertMatrix,
                          filament::math::mat4f &perspectiveMatrix, filament::math::mat4f &modelMatrix);

    void updateVert(const float *vert, const float *vert2,
                    filament::math::mat4f &orthMatrix,
                    filament::math::mat4f &perspectiveMatrix,
                    filament::math::mat4f &modelMatrix,
                    filament::math::mat4f &modelConvertMatrix, float scale,
                    int userIndex, float staticAlpha = -1.0);

    static void onLoadedResource(void *buffer, size_t size, void *user) {
        delete[] (float *) buffer;
    }

    bool enableFaceMesh = false;    // 可供上层判断是否要传 camera texture

    void clearMesh(SMART_PTR<gltfio::AssetLoader> assetLoader);

    void getTessellationVerticesMapData(
        std::vector<int> &dynamicVerticesMap,
        std::vector<int> &staticVerticesMap,
        std::vector<int> &tessellationVerticesMap);

    filament::View *getFinalView();
};

LIGHT3D_NAMESPACE_END

#endif //TNT_DUALFACEMESHCOMPONENT_H
