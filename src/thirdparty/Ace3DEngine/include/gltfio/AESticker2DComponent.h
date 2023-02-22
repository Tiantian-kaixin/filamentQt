//
// Created by kevinxing on 2020/6/22.
//

#ifndef TNT_AESTICKER2DCOMPONENT_H
#define TNT_AESTICKER2DCOMPONENT_H

#include <filament/Engine.h>
#include <gltfio/MaterialManager.h>
#include <gltfio/TextureManager.h>
#include <gltfio/ModelDef.h>
LIGHT3D_NAMESPACE_BEGIN
class AESticker2DComponent {
 public:
    AESticker2DComponent(filament::Engine *engine, filament::Scene *scene, filament::View *view,
                         SMART_PTR<MaterialManager> materialManager,
                         SMART_PTR<TextureManager> textureManager);
    void updateModels(SMART_PTR<AESticker2DConfig> config);

 private:
    filament::Engine *engine_;
    filament::Scene *scene_;
    filament::View* view_;
    utils::Path material_path_;
    std::shared_ptr<MaterialManager> materialManager_;
    std::shared_ptr<TextureManager> textureManager_;
    std::vector<utils::Entity> entities_;

    void copyVertsAndIndices(gl_util::QuadConfig &config, AESticker2DModel &model);

    void destroyAndRemoveEntities(filament::Engine* engine, filament::Scene *scene, std::vector<utils::Entity> &entities);
};

LIGHT3D_NAMESPACE_END
#endif //TNT_AESTICKER2DCOMPONENT_H
