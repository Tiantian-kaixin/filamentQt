//
// Created by ericwywang on 2020/6/22.
//

#ifndef TNT_NORMALCUBEMAPCOMPONENT_H
#define TNT_NORMALCUBEMAPCOMPONENT_H

#include "common_util.h"
#include <gltfio/gl_util.h>
#include "MaterialManager.h"

LIGHT3D_NAMESPACE_BEGIN

class TTThreeDMaterialModel;

class NormalCubemapComponent {
    int size = 2048;

    std::map<std::string, filament::Texture *> finalTextureMap;
    filament::Engine *mEngine;
    std::vector<std::vector<gl_util::Framebuffer>> framebufferVec;
    filament::Material *cubeMat = nullptr;
    filament::SwapChain *swapChain;
    SMART_PTR<MaterialManager> materialManager_;

    void addNodeForNormal(std::string &nodeName, gltfio::FilamentAsset *asset);

public:
    NormalCubemapComponent(filament::Engine *engine) : mEngine(engine) {};

    ~NormalCubemapComponent();

    void init(SMART_PTR<MaterialManager> materialManager);

    void setup(TTThreeDMaterialModel *materialModel, gltfio::FilamentAsset *asset);

    void render(filament::Renderer *renderer);

    filament::Texture *getCubeMap(const char *node);
};

LIGHT3D_NAMESPACE_END

#endif //TNT_NORMALCUBEMAPCOMPONENT_H
