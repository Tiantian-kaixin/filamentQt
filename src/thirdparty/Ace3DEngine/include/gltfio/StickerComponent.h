//
// Created by kevinxing on 2020/6/17.
//

#ifndef TNT_STICKERCOMPONENT_H
#define TNT_STICKERCOMPONENT_H


#include <gltfio/Component.h>
#include <gltfio/MaterialManager.h>
#include <unordered_map>

LIGHT3D_NAMESPACE_BEGIN

struct QuadItem{
    utils::Entity entity;
    filament::MaterialInstance* matInstance;
};

class StickerComponent {
public:
    StickerComponent(filament::Engine* engine, filament::Scene* scene, SMART_PTR<MaterialManager> materialManager, SMART_PTR<TextureManager> textureManager, SMART_PTR<TTFilamentSetupParam> param, utils::Path materialPath);

    std::vector<utils::Entity> getEntities();

    void update(float renderTime);

    void setFaceCount(uint32_t faceCount);

    void setTransform(const filament::math::mat4f &transform, int userIndex);

private:
    bool debug = false;
    std::unordered_map<std::string, QuadItem> quadItems;
    filament::Engine *engine_;
    filament::Scene *scene_;
    utils::Path material_path_;
    std::shared_ptr<MaterialManager> materialManager_;
    std::shared_ptr<TextureManager> textureManager_;
    std::vector<TTStickerModel*> models;
    std::vector<filament::math::mat4f> transforms_;

    void createStickers(std::vector<TTStickerModel*>& models);
    static void adjustVerts(gl_util::Vertex* verts, size_t width, size_t height);

    filament::math::mat4f getTransform(std::vector<filament::math::mat4f> &transforms);
};

LIGHT3D_NAMESPACE_END
#endif //TNT_STICKERCOMPONENT_H
