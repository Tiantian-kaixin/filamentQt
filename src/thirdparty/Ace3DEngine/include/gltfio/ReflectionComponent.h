//
// Created by kevinxing on 2020-02-14.
//

#ifndef TNT_REFLECTIONCOMPONENT_H
#define TNT_REFLECTIONCOMPONENT_H

#include <gltfio/ModelDef.h>
#include <gltfio/file_util.h>
#include <gltfio/TextureManager.h>
#include <gltfio/FilamentAsset.h>
#include "AssetLoader.h"

LIGHT3D_NAMESPACE_BEGIN

class ReflectionComponent {
private:
    std::vector<std::vector<gltfio::FilamentAsset*>> assetList;
    filament::Engine* engine_;
    SMART_PTR<TextureManager> texture_manager_;
    std::vector<TTThreeDMaterialModel *> materialModels;
    filament::Material* reflectionMaterial = nullptr;
    std::vector<filament::MaterialInstance*> matList;
    SMART_PTR<MaterialManager> materialManager_;
public:
    void init(filament::Engine *engine, SMART_PTR<TextureManager> texture_manager, SMART_PTR<MaterialManager> materialManager, std::vector<TTThreeDMaterialModel *>& materialModels) {
        engine_ = engine;
        texture_manager_ = texture_manager;
        this->materialModels = materialModels;
        materialManager_ = materialManager;
    }

    void setAssetsList(std::vector<std::vector<gltfio::FilamentAsset*>>& assetList){
        this->assetList = assetList;
        int maxFaceCount = assetList.size();
        int modelCount = materialModels.size();
        filament::TextureSampler
        sampler(filament::TextureSampler::MinFilter::LINEAR, filament::TextureSampler::MagFilter::LINEAR);

        for (int j = 0; j < modelCount; j++) {
            TTThreeDMaterialModel *model = materialModels[j];
            const auto &rcList = model->reflectConfigList;
            for (TTReflectConfig* config : rcList) {
                if (!reflectionMaterial) {
                    reflectionMaterial = materialManager_->GetBaseMaterial(MaterialType::REFLECTION);
                }
                filament::Texture *albedoTex = texture_manager_->loadTexture(config->baseColorImage, true);
                filament::Texture *reflectTex = texture_manager_->loadTexture(config->reflectImage, true);
                for (int i = 0; i < maxFaceCount; i++) {

                    auto &rm = engine_->getRenderableManager();
                    const utils::Entity *entity = assetList[i][j]->getEntity(config->nodeName.c_str());
                    int instance = rm.getInstance(*entity);
                    size_t primCount = rm.getPrimitiveCount(instance);

                    for (size_t primIndex = 0; primIndex < primCount; primIndex++) {
                        filament::MaterialInstance *matIns = reflectionMaterial->createInstance();
                        matIns->setParameter("baseColorMap", albedoTex, sampler);
                        matIns->setParameter("reflectMap", reflectTex, sampler);
                        matIns->setParameter("zNormalScale", config->zNormalScale);
                        matIns->setParameter("blendFactor", config->blendFactor);
                        matIns->setParameter("reflectOffset", config->reflectOffset);
                        matIns->setParameter("blendMode", config->blendMode);
                        matIns->setParameter("modulateAlpha", config->modulateAlpha);
                        rm.setMaterialInstanceAt(instance, primIndex, matIns);
                        matList.push_back(matIns);
                    }
                }
            }
        }
    }

    void updateParams(float zNormalScale, float blendFactor, filament::math::float2 reflectOffset,float modulateAlpha){
        int maxFaceCount = assetList.size();
        int modelCount = materialModels.size();
        for (int j = 0; j < modelCount; j++) {
            TTThreeDMaterialModel *model = materialModels[j];
            const auto &rcList = model->reflectConfigList;
            for(TTReflectConfig* config : rcList){
                for (int i = 0; i < maxFaceCount; i++) {

                    auto &rm = engine_->getRenderableManager();
                    const utils::Entity *entity = assetList[i][j]->getEntity(config->nodeName.c_str());
                    int instance = rm.getInstance(*entity);
                    size_t primCount = rm.getPrimitiveCount(instance);

                    for (size_t primIndex = 0; primIndex < primCount; primIndex++) {
                        filament::MaterialInstance *matIns = rm.getMaterialInstanceAt(instance, primIndex);
                        matIns->setParameter("zNormalScale", zNormalScale);
                        matIns->setParameter("blendFactor", blendFactor);
                        matIns->setParameter("reflectOffset", reflectOffset);
                        matIns->setParameter("modulateAlpha", modulateAlpha);
                    }
                }
            }
        }
    }

    void clearMesh() {
        for (auto mat : matList) {
            engine_->destroy(mat);
        }
        matList.clear();
    }

    void updateParams(std::vector<std::vector<TTReflectConfig *>> &reflect) {
        int maxFaceCount = assetList.size();
        int modelCount = materialModels.size();
        for (int j = 0; j < modelCount; j++) {
            TTThreeDMaterialModel* model = materialModels[j];
            const auto& rcList = model->reflectConfigList;
            for (int k = 0; k < rcList.size(); k++)
            {
                TTReflectConfig* config = rcList[k];
                for (int i = 0; i < maxFaceCount; i++) {

                    auto& rm = engine_->getRenderableManager();
                    const utils::Entity* entity = assetList[i][j]->getEntity(config->nodeName.c_str());
                    int instance = rm.getInstance(*entity);
                    size_t primCount = rm.getPrimitiveCount(instance);

                    for (size_t primIndex = 0; primIndex < primCount; primIndex++) {
                        filament::MaterialInstance* matIns = rm.getMaterialInstanceAt(instance, primIndex);
                        matIns->setParameter("zNormalScale", reflect[j][k]->zNormalScale);
                        matIns->setParameter("blendFactor", reflect[j][k]->blendFactor);
                        matIns->setParameter("reflectOffset", reflect[j][k]->reflectOffset);
                        matIns->setParameter("modulateAlpha", reflect[j][k]->modulateAlpha);
                    }
                }
            }
        }
    }

    void clearMesh(SMART_PTR<gltfio::AssetLoader> assetLoader) {
        for (auto mat: matList) {
            engine_->destroy(mat);
        }
    }
};

LIGHT3D_NAMESPACE_END

#endif //TNT_REFLECTIONCOMPONENT_H
