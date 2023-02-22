//
// Created by soarjiang on 2020/6/3.
//

#ifndef TNT_HEADFEATHERCOMPONENT_H
#define TNT_HEADFEATHERCOMPONENT_H

#include <gltfio/gl_util.h>
#include <gltfio/Component.h>
#include <gltfio/HeadHelper.h>
#include <gltfio/ModelDef.h>
#include "FaceBindHelper.h"

LIGHT3D_NAMESPACE_BEGIN

struct HeadFeatherVertex {
    filament::math::float2 position;
    filament::math::float2 uv;
};

class HeadFeatherComponent : Component {
 private:
    filament::Engine* engine = nullptr;
    filament::Scene* scene = nullptr;
    filament::VertexBuffer* headFeatherVertexBuffer = nullptr;
    filament::IndexBuffer* headFeatherIndexBuffer = nullptr;

    std::vector<std::vector<gltfio::FilamentAsset *>> headFeatherAssetList;
    gltfio::FilamentAsset *smallAsset = nullptr;
    gltfio::FilamentAsset *bigAsset = nullptr;

    utils::Entity finalQuad;
    filament::Material* mixMat = nullptr;
    filament::Material* headMat = nullptr;
    gl_util::Framebuffer primary; // 初始的 frame buffer, 由外界传入
    gl_util::Framebuffer second;  // 第二个不同 scale 头模型的 frame buffer
    gl_util::Framebuffer final;   // primary + second mix 后的结果

    std::vector<utils::Entity> head;    // 第二个指定 scale 的头模型
    bool useSpecificHeadModel;  // 是否用指定的头模型
    filament::math::float3 headScale = {1.2, 1.2, 1.2};
    filament::math::float3 assetScale = {1, 1, 1};
    filament::math::float3 headDiffScale = {1, 1, 1};
    std::string smallHeadName;
    std::string bigHeadName;
    std::vector<filament::MaterialInstance*> matList;

    utils::Path resourcePath;

    utils::Path materialDir;

    SMART_PTR<LIGHT3D::HeadHelper> headHelper;

    void updateHeadFeatherViewSize(int width, int height, int fovAngle);    // 仅在初始化的时候使用

 public:
    ~HeadFeatherComponent();

    void addDirectionLight(utils::Entity light);

    void setIndirectLight(SMART_PTR<filament::IndirectLight> ibl);

    HeadFeatherComponent(filament::Engine *engine, filament::Scene *scene, filament::View *view,
                         filament::Camera *camera, const utils::Path &resourcePath,
                         utils::Path &materialDir, SMART_PTR<MaterialManager> materialManager);

    bool initConfig(int width, int height, int fovAngle, int maxFaceCount, TTMaterialHeadFeatherParam *head_feather_params, SMART_PTR<TTFilamentSetupParam> param, SMART_PTR<MaterialManager> materialManager);

    void setup(TTThreeDMaterialModel *materialModel, std::vector<gltfio::FilamentAsset *> *batchList, int faceIndex,
               SMART_PTR<gltfio::AssetLoader> assetLoader, SMART_PTR<ResourceHelper> resourceHelper, SMART_PTR<TextureManager> textureLoader) override;

    void updateVert(float *vert, int count, int userIndex);

#if ENABLE_HEAD_FEATHER_DEBUG
    void setCameraTexture(filament::Texture *texture);
    void setMvpMatrix(filament::math::mat4f &transform, float scale);
#endif

    void setHeadTransform(SMART_PTR<LIGHT3D::TransformHelper> transformHelper, int userIndex);

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

    void resizeHeadFeatherView(int width, int height, int fovAngle);

    void clearMesh(SMART_PTR<gltfio::AssetLoader> assetLoader);

    void showOrHideAsset(int userIndex, int materialIndex, bool show);

    void showOrHideAssetEntity(int userIndex, int materialIndex, const std::string &entityName,
                               bool show);

    static void loadedResource(void *buffer, size_t size, void *user) {
        delete[] (uint8_t *) buffer;
    }

    void updateParams(float offsetX, float offsetY, float sigma);  // Debug Only

    void setHeadScale(filament::math::float3 assetScale, filament::math::float3 headDiffScale);

    void updateFaceBind(SMART_PTR<FaceBindHelper> helper, int userIndex, int materialIndex, TTThreeDMaterialModel *materialModel);

    SMART_PTR<LIGHT3D::HeadHelper> getFeatherHeadHelper();
};

LIGHT3D_NAMESPACE_END

#endif //TNT_HEADFEATHERCOMPONENT_H
