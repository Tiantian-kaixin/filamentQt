//
// Created by ericwywang on 2020/3/4.
//

#ifndef TNT_FACEMESHCOMPONENT_H
#define TNT_FACEMESHCOMPONENT_H

#include <gltfio/ResourceHelper.h>
#include <gltfio/ModelDef.h>
#include <gltfio/gl_util.h>
#include <gltfio/MaterialManager.h>

LIGHT3D_NAMESPACE_BEGIN
// TODO(@dianxin) : FaceMeshComponent can inherit from Component.
class FaceMeshComponent {
    filament::Engine* mEngine = nullptr;
    filament::Material* faceMat = nullptr;
    std::vector<filament::MaterialInstance* > matList;
    std::map<utils::Entity, gltfio::FilamentAsset*> assetMap;
public:
    FaceMeshComponent(filament::Engine *engine, filament::Scene *scene);

    void initConfig(SMART_PTR<MaterialManager> materialManager, SMART_PTR<TTFilamentSetupParam> param, int maxFaceCount);

    void setup(TTThreeDMaterialModel* materialModel, gltfio::FilamentAsset *asset, SMART_PTR<gltfio::AssetLoader> assetLoader, SMART_PTR<ResourceHelper> resourceHelper);

    void setupFaceMaterial();

    void setFaceMesh(filament::Texture* texture);

    void updateTransform();

    void clearMesh(SMART_PTR<gltfio::AssetLoader> assetLoader);
};

LIGHT3D_NAMESPACE_END
#endif //TNT_FACEMESHCOMPONENT_H
