//
// 支持2种方式的配置来动态的显示或隐藏glb：
//
// * 通过nodeAssociateGender配置来决定glb在男女的时候的显示情况，可以支持到node的粒度
// * 通过charmRange配置来随机化的显示，不可支持到node的粒度
//
// Created by panda on 2020-03-06.
//

#ifndef TNT_DYNAMICGLBCOMPONENT_H
#define TNT_DYNAMICGLBCOMPONENT_H

#include <map>
#include <string>
#include <filament/RenderableManager.h>
#include <gltfio/FilamentAsset.h>
#include <gltfio/common_util.h>
#include <gltfio/ModelDef.h>

LIGHT3D_NAMESPACE_BEGIN
class DynamicGlbComponent
{
private:
    filament::Engine *engine;
    filament::Scene *scene;
    SMART_PTR<TTFilamentSetupParam> param;

    void showOrHideEntityWithGender(filament::RenderableManager &rm, gltfio::FilamentAsset *asset,
                                    int gender, size_t nodeAssociateGenderCount,
                                    std::vector<std::string> nodeAssociateGender) {
        std::string all = "*";
        int count = (int)asset->getEntityCount();
        for (int i = count - 1; i >= 0; --i) {
            const utils::Entity entity = asset->getEntities()[i];
            auto instance = rm.getInstance(entity);
            const char *name = asset->getNodeByEntity(entity);
            for (size_t j = 0; j < nodeAssociateGenderCount; j += 2) {
                std::string nn = nodeAssociateGender[j];
                int gg = std::stoi(nodeAssociateGender[j + 1]);
                bool show = (gender == gg) && (nn == all || name == nn);
                LIGHT3D::showOrHideEntity(rm, entity, show);
            }
        }
    }

 public:
    DynamicGlbComponent(filament::Engine *engine, filament::Scene *scene,
                        SMART_PTR<TTFilamentSetupParam> param);

    /**
     * 动态处理glb的显示，同时处理charmRange和gender的逻辑
     *
     * @param assetList 所有的asset列表
     * @param charmRange 多个glb配置了charmRange，charmRange的数组
     * @param rangeCount charmRange的数量，目前没用
     * @param genders 多个glb配置了gender，genders的数组
     * @param genderCount gender的数量
     */
    void changeAssetDynamic(std::vector<std::vector<gltfio::FilamentAsset *>> assetList,
                            int *charmRange, int rangeCount, int *genders, int genderCount);

    /**
     * 为AR添加
     *
     * @param assetList 所有的asset列表
     * @param show true显示，false不显示
     */
    void showOrHide(std::vector<std::vector<gltfio::FilamentAsset *>> assetList, bool show);
};
LIGHT3D_NAMESPACE_END
#endif //TNT_DYNAMICGLBCOMPONENT_H
