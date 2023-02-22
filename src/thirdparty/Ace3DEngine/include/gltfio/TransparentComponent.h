//
// Created by ericwywang on 2020/10/29.
//

#ifndef TNT_TRANSPARENTCOMPONENT_H
#define TNT_TRANSPARENTCOMPONENT_H

#include "transparent_utils.h"
#include "ModelDef.h"
#include "ResourceHelper.h"

#include <filament/Engine.h>
#include <filament/Scene.h>
#include <filament/Skybox.h>
#include <filament/View.h>
#include <filament/RenderTarget.h>
#include <filament/Camera.h>
#include <filament/Renderer.h>
#include "DynamicBoneHelper.h"
#include "AnimatorHelper.h"
#include "FaceBindHelper.h"

LIGHT3D_NAMESPACE_BEGIN

    using InstanceHandle = gltfio::FilamentInstance *;

    class TransparentComponent {

    private:
        filament::Engine *engine;

        const int num = 5;
        std::vector<InstanceHandle *> instancesList;
        std::vector<gltfio::FilamentAsset *> assetList;
        int fov = 30;
        int numInstances = num;
        filament::Skybox *skybox = nullptr;
        filament::View *finalView;
        filament::Scene *finalScene;
        filament::Camera *finalCamera;
        utils::Entity cameraEntity;
        utils::Entity finalQuad;

        SMART_PTR<TransparentUtils> transparent_utils = nullptr;

        SMART_PTR<TransparentUtils::Framebuffer> primary = nullptr;
        SMART_PTR<MaterialManager> materialManager = nullptr;

        TransparentUtils::Framebuffer *framebuffer;
        int framebufferCount = num;

        TransparentUtils::Framebuffer *mixFramebuffer;
        int mixBufferCount = num - 1;

        utils::Entity *quads;

        filament::Texture *blackTex = nullptr;
        std::vector<filament::MaterialInstance *> matVec;

    public:
        TransparentComponent(filament::Engine *engine, filament::View *view,
                             int width, int height, int fovAngle, int transparentLayer,
                             const SMART_PTR<MaterialManager> &materialManager);

        ~TransparentComponent();

        void setIndirectLight(SMART_PTR<filament::IndirectLight> ibl);

        void addDirectionLight(utils::Entity light);

        void
        setup(TTThreeDMaterialModel *materialModel, int userIndex, const SMART_PTR<gltfio::AssetLoader> &assetLoader,
              const SMART_PTR<ResourceHelper> &resourceHelper, const SMART_PTR<TextureManager> &textureManager);

        void setTransform(filament::math::mat4f &transform, int userIndex, int materialIndex);

        void addDynamicBone(SMART_PTR<DynamicBoneHelper> dynamicBoneHelper, int userIndex,
                            TTDynamicBoneParam* boneParam, int materialIndex);

        void setAnimator(SMART_PTR<LIGHT3D::AnimatorHelper> animatorHelper, int userIndex, int materialIndex);

        void setMorphWeight(int userIndex, int materialIndex, const std::string &name,
                            const std::vector<gltfio::MorphWeight> &weights);

        void setCurrentHeadCount(int n);

        void render(filament::Renderer *renderer);

        void clearMesh(SMART_PTR<gltfio::AssetLoader> assetLoader);

        void resizeTransparentView(int width, int height, int fovAngle);

        void updateFaceBind(SMART_PTR<FaceBindHelper> helper, int userIndex, int materialIndex, TTThreeDMaterialModel *materialModel);

        filament::View *getFinalView();
    };

LIGHT3D_NAMESPACE_END


#endif //TNT_TRANSPARENTCOMPONENT_H
