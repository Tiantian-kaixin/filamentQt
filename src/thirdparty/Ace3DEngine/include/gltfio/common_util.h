//
// Created by ericwywang on 2020/1/17.
//

#ifndef TNT_COMMON_UTIL_H
#define TNT_COMMON_UTIL_H

#define kHideEntityLayerMaskValue 0x00
#define kShowEntityLayerMaskValue 0xff

#define kProjectionNear 0.1f
#define kProjectionFar 10000.f

#define ENABLE_LOG 0

#define LIGHT3D light3d
#define LIGHT3D_NAMESPACE_BEGIN namespace LIGHT3D {
#define LIGHT3D_NAMESPACE_END }

#define SMART_PTR std::shared_ptr
#define MAKE_SMART_PTR std::make_shared

#include <filament/TransformManager.h>
#include <filament/RenderableManager.h>
#include <filament/Fence.h>
#include <filament/Camera.h>
#include <filament/Viewport.h>
#include <utils/EntityManager.h>
#include <filament/Renderer.h>
#include <filament/Scene.h>

#include <gltfio/FilamentAsset.h>
#include <gltfio/AssetLoader.h>
#include <gltfio/ResourceLoader.h>
#include <gltfio/file_util.h>
#include <gltfio/math_util.h>
#include <gltfio/log_util.h>

LIGHT3D_NAMESPACE_BEGIN

#if defined(__APPLE__)
#include <TargetConditionals.h>
#endif

#define FILAMENT_ENV_IOS (TARGET_OS_IOS || (IOS))

inline void showOrHideAsset(filament::Scene *scene, gltfio::FilamentAsset *asset, bool show) {
    int count = (int) asset->getEntityCount();
    for (int i = count - 1; i >= 0; --i) {
        utils::Entity entity = asset->getEntities()[i];
        if (show) {
            scene->addEntity(entity);
        } else {
            scene->remove(entity);
        }
    }
}

// 通过layerMask来控制entity的显示和隐藏
inline void showOrHideAsset(filament::RenderableManager &rm, gltfio::FilamentAsset *asset, bool show) {
    int layerMaskValue = show ? kShowEntityLayerMaskValue : kHideEntityLayerMaskValue;
    int count = (int) asset->getEntityCount();
    for (int i = count - 1; i >= 0; --i) {
        utils::Entity entity = asset->getEntities()[i];
        auto instance = rm.getInstance(entity);
        rm.setLayerMask(instance, 0xFF, layerMaskValue);
    }
}

inline void
showOrHideAssetEntity(filament::Scene *scene, gltfio::FilamentAsset *asset, const std::string &entityName, bool show) {
    const utils::Entity *entity = asset->getEntity(entityName.c_str());
    if (entity == nullptr) {
        return;
    }
    if (show) {
        scene->addEntity(*entity);
    } else {
        scene->remove(*entity);
    }
}

inline void
showOrHideAssetEntity(filament::RenderableManager &rm, gltfio::FilamentAsset *asset, const std::string &entityName,
                      bool show) {
    int layerMaskValue = show ? kShowEntityLayerMaskValue : kHideEntityLayerMaskValue;
    const utils::Entity *entity = asset->getEntity(entityName.c_str());
    if (entity == nullptr) {
        return;
    }
    auto instance = rm.getInstance(*entity);
    rm.setLayerMask(instance, 0xFF, layerMaskValue);
}

inline void showOrHideEntity(filament::RenderableManager &rm, const utils::Entity entity, bool show) {
    int layerMaskValue = show ? kShowEntityLayerMaskValue : kHideEntityLayerMaskValue;
    auto instance = rm.getInstance(entity);
    rm.setLayerMask(instance, 0xFF, layerMaskValue);
}

inline char* stringToCharArray(std::string str) {
    char* array = new char[str.size() + 1];
    memset(array, 0, str.size() + 1);
    strcpy(array, str.data());
    return array;
}

LIGHT3D_NAMESPACE_END

#endif //TNT_COMMON_UTIL_H
