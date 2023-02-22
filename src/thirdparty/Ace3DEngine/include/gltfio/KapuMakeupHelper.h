//
// Created by kevinxing on 2020/9/16.
//

#ifndef TNT_KAPUMAKEUPHELPER_H
#define TNT_KAPUMAKEUPHELPER_H

#include <filament/Engine.h>
#include <filament/Texture.h>
#include <loader/Loader.h>
#include <gltfio/TextureManager.h>

LIGHT3D_NAMESPACE_BEGIN

using namespace filament;

class KapuMakeupHelper {
public:
    KapuMakeupHelper(Engine *engine, SMART_PTR<TextureManager> textureManager, SMART_PTR<MaterialManager> materialManager) {
    };
    ~KapuMakeupHelper() {
    }

    void setUniformParams(const std::string& modelDir, filament::Texture* baseTexture, std::map<KapuMaterialType, CM::MakeupData>& makeupMap);

    void updateHSVUniforms();

    filament::Texture* getMakeupTexture() {
        return nullptr;
    }

    utils::Entity getMakeupEntity() {
    }

    void render(Renderer* renderer) {
    }

    filament::View *getOffscreenView() {
        return nullptr;
    }

    void setLoader(CM::Loader* loader) {
    }

private:
};

LIGHT3D_NAMESPACE_END

#endif //TNT_KAPUMAKEUPHELPER_H
