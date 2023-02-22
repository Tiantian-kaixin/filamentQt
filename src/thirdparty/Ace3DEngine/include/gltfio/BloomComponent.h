// Created by ericwywang on 2020/1/19.
//

#ifndef LIBS_GLTFIO_INCLUDE_GLTFIO_BLOOMCOMPONENT_H_
#define LIBS_GLTFIO_INCLUDE_GLTFIO_BLOOMCOMPONENT_H_

#include <gltfio/gl_util.h>
#include <gltfio/Component.h>
#include <gltfio/MaterialManager.h>
#include "FaceBindHelper.h"

LIGHT3D_NAMESPACE_BEGIN

struct OverlayVertex {
    filament::math::float2 position;
    filament::math::float2 uv;
};

class BloomComponent : Component {
 private:
    filament::Engine *engine;
    filament::Scene *scene;
    filament::VertexBuffer *bloomVertexBuffer;
    filament::IndexBuffer *bloomIndexBuffer;

    std::vector<std::vector<gltfio::FilamentAsset *>> bloomList;
    std::vector<int32_t> bloomEntities;
    utils::Entity finalQuad;
    utils::Entity hblurQuad;
    utils::Entity vblurQuad;
    filament::Material *mixMat;
    filament::Material *blurMat;
    filament::Material *headMat;
    gl_util::Framebuffer primary;
    gl_util::Framebuffer bloom;
    gl_util::Framebuffer final;
    gl_util::Framebuffer hblur;
    gl_util::Framebuffer vblur;

    std::vector<utils::Entity> head;
    std::vector<filament::MaterialInstance*> matList;

 public:
    BloomComponent(filament::Engine *engine, filament::Scene *scene, filament::View *view,
                   filament::Camera *camera, SMART_PTR<MaterialManager> materialManager);
    ~BloomComponent();

    bool initConfig(int width, int height, int fovAngle, int maxFaceCount, float blurWeight,
                    float mixAlpha, int mixType);

    void setup(TTThreeDMaterialModel *materialModel, std::vector<gltfio::FilamentAsset *> *batchList, int faceIndex,
               SMART_PTR<gltfio::AssetLoader> assetLoader, SMART_PTR<ResourceHelper> resourceHelper, SMART_PTR<TextureManager> textureLoader) override;

    void setupHead(const std::vector<utils::Entity> &entities);

    void addDirectionLight(utils::Entity light);

    void setSkyBox(SMART_PTR<filament::Skybox> skybox);

    void setIndirectLight(SMART_PTR<filament::IndirectLight> ibl);

    void setMorphWeight(int userIndex, int materialIndex, const std::string &name,
                        const std::vector<gltfio::MorphWeight> &weights) override;

    void setMaterialImage(int userIndex, int materialIndex, const std::string &name,
                          const std::string &parameterName, filament::Texture *texture,
                          bool hideEntity) override;
    void setMaterialImage(int userIndex, int materialIndex, const std::string &name,
                          const std::string &parameterName, filament::Texture *texture);

    void setTransform(filament::math::mat4f &transform, int userIndex, int materialIndex) override;

    void addDynamicBone(SMART_PTR<DynamicBoneHelper> dynamicBoneHelper, int userIndex,
                        TTDynamicBoneParam *boneParam, int materialIndex) override;

    void setAnimator(SMART_PTR<LIGHT3D::AnimatorHelper> animatorHelper, int userIndex, int materialIndex) override;

    void setCurrentHeadCount(int n) override;

    void render(filament::Renderer *renderer);

    filament::View *getFinalView();

    void updateBloomViewSize(int width, int height, int fovAngle);

    void resizeBloomView(int width, int height, int fovAngle);

    void clearMesh(SMART_PTR<gltfio::AssetLoader> assetLoader);

    void showOrHideAsset(int userIndex, int materialIndex, bool show);

    void showOrHideAssetEntity(int userIndex, int materialIndex, const std::string &entityName,
                               bool show);

    void updateFaceBind(SMART_PTR<FaceBindHelper> helper, int userIndex, int materialIndex, TTThreeDMaterialModel *materialModel);
};

LIGHT3D_NAMESPACE_END

#endif  // LIBS_GLTFIO_INCLUDE_GLTFIO_BLOOMCOMPONENT_H_
