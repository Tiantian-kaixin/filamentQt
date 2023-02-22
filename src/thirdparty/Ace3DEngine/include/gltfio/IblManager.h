//
// Created by tianaoge on 2020/6/5.
//

#ifndef TNT_IBLMANAGER_H
#define TNT_IBLMANAGER_H

#include <string>
#include <map>
#include <filament/Texture.h>
#include <filament/Engine.h>
#include <gltfio/TextureManager.h>
#include <gltfio/log_util.h>
#include <utils/Path.h>

LIGHT3D_NAMESPACE_BEGIN

class IblManager {
public:
    IblManager(filament::Engine* engine) : engine_(engine) {
        
    }

    void setTextureManager(SMART_PTR<TextureManager> manger) {
        manager_ = manger;
    }

    bool loadIblFromKtx(std::string iblPath, filament::Texture **iblTexture,
                        filament::math::float3 *iblSH);

    bool loadIblFromPng(std::string iblPath, filament::Texture **iblTexture,
                        filament::math::float3 *iblSH);


private:
    SMART_PTR<TextureManager> manager_;
    filament::Engine* engine_;

};

LIGHT3D_NAMESPACE_END

#endif //TNT_IBLMANAGER_H
