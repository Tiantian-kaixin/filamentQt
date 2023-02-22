//
// Created by paynepan on 2020/8/12.
//

#ifndef TNT_MATERIAL_UTIL_H
#define TNT_MATERIAL_UTIL_H

#include <string>
#include <utils/EntityManager.h>
#include <filament/TextureSampler.h>
#include <gltfio/log_util.h>

LIGHT3D_NAMESPACE_BEGIN

namespace material {

    template <typename T>
    static bool setMaterialParameter(filament::MaterialInstance* mat, const char* name, T value){
        if (mat->getMaterial()->hasParameter(name)) {
            mat->setParameter(name, value);
            return true;
        }
        return false;
    }

    template <typename T>
    static bool setMaterialParameter(filament::MaterialInstance* mat, const char* name, T value, size_t count){
        if (mat->getMaterial()->hasParameter(name)) {
            mat->setParameter(name, value, count);
            return true;
        }
        return false;
    }

    static bool setMaterialParameter(filament::MaterialInstance* mat, const char* name, filament::Texture const* texture,
                                     filament::TextureSampler const& sampler){
        if (mat->getMaterial()->hasParameter(name)) {
            mat->setParameter(name, texture, sampler);
            return true;
        }
        return false;
    }

    static void setMaterialImage(filament::Engine* engine, filament::Texture* baseColorTexture, utils::Entity entity, const std::string& parameterName, bool hideEntity) {
        std::string name = parameterName;
        auto &rm = engine->getRenderableManager();
        auto instance = rm.getInstance(entity);
        size_t primCount = rm.getPrimitiveCount(instance);
        for (int primIndex = 0; primIndex < primCount; primIndex++) {
            auto mi = rm.getMaterialInstanceAt(instance, primIndex);
            if (baseColorTexture) {
                filament::TextureSampler sampler;
                if (name.empty()) {
                    name = "baseColorMap";
                }
                mi->setParameter(name.c_str(), baseColorTexture, sampler);
                uint8_t layerMask = hideEntity ? kHideEntityLayerMaskValue : kShowEntityLayerMaskValue;
                rm.setLayerMask(instance, 0xFF, layerMask);
            } else {
                LOGE << "[filament] The texture not exist" << LOGEND;
            }
        }
    }

    static void setMaterialImage(filament::Engine* engine, gltfio::FilamentAsset* asset, const std::string& entityName, const std::string& parameterName, filament::Texture* baseColorTexture) {
        auto entity = asset->getEntity(entityName.c_str());
        if (entity == nullptr) {
            return;
        }
        std::string name = parameterName;
        auto &rm = engine->getRenderableManager();
        auto instance = rm.getInstance(*entity);
        size_t primCount = rm.getPrimitiveCount(instance);
        for (int primIndex = 0; primIndex < primCount; primIndex++) {
            auto mi = rm.getMaterialInstanceAt(instance, primIndex);
            if (baseColorTexture) {
                filament::TextureSampler sampler;
                if (name.empty()) {
                    name = "baseColorMap";
                }
                mi->setParameter(name.c_str(), baseColorTexture, sampler);
            } else {
                LOGE << "[filament] The texture not exist" << LOGEND;
            }
        }
    }
}

LIGHT3D_NAMESPACE_END

#endif //TNT_MATERIAL_UTIL_H
