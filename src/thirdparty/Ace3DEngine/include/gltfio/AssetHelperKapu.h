//
// Created by ericwywang on 2020/1/19.
//

#ifndef TNT_ASSETHELPERKAPU_H
#define TNT_ASSETHELPERKAPU_H

//#include <gltfio/FurComponent.h>
//#include <gltfio/BloomComponent.h>
//#include <gltfio/TextureManager.h>
//#include <gltfio/FaceMeshComponent.h>
//#include <gltfio/ReflectionComponent.h>
//#include <gltfio/DynamicGlbComponent.h>
//#include <gltfio/CgltfDataManager.h>
//#include <gltfio/GeometryComponent.h>
//#include <gltfio/DualFaceMeshComponent.h>
//#include <gltfio/ResourceHelper.h>
//#include <gltfio/HeadFeatherComponent.h>
//#include <gltfio/CustomShaderComponent.h>
#include <loader/Loader.h>
//#include <gltfio/KapuMakeupHelper.h>

LIGHT3D_NAMESPACE_BEGIN

class AssetHelperKapu {
   public:
    AssetHelperKapu(filament::Engine *engine, filament::Scene *scene,
                SMART_PTR<TextureManager> textureManager,
                SMART_PTR<MaterialManager> materialManager, gltfio::stb_image_handle* handle,
                utils::Path resourcePath);
    ~AssetHelperKapu();

    std::vector<std::map<KapuMaterialType, gltfio::FilamentAsset*>> assetList;

    std::vector<std::map<KapuMaterialType, std::vector<TTCustomShader*>>> customShaderMap;

    void setTransform(filament::math::mat4f transform, int userIndex, KapuMaterialType materialType);

    void setupMesh();

    void createMakeupImage();

//    void updateMaterialParams(SMART_PTR<CustomShaderComponent> customShaderComponent);

    void clearMesh();

    void removeAsset(gltfio::FilamentAsset *asset);

    void initConfig(std::vector<SMART_PTR<TTThreeDMaterialModel>> param);

    void setLoaders(const std::vector<CM::Loader *> &loaders);

    void updateBlendShapeWeights(int personIndex, std::map<std::string, float>& weightMap);

//    void updateAnimojiWeights(int personIndex, std::map<std::string, float>& weightMap);

    std::vector<std::vector<gltfio::FilamentAsset *>> getAssetList();

//    void updateAnimator(SMART_PTR<AnimatorHelper> animatorHelper);

//    void setupDynamicBone(SMART_PTR<DynamicBoneHelper> dynamicBoneHelper);

    void removeAnimator();

    std::vector<std::map<std::string, MorphData*>> getMorphData(int index);

    void updateExpressionMorphData(int index, std::string nodeName, float **dataList, std::string *expList, int expCount);
    void updateMorph();

//    void render(Renderer *renderer);

    std::vector<filament::View*> getOffscreenViews();

//    void updateHSVUniforms(SMART_PTR<CustomShaderComponent> customShaderComponent);

    double getAnimLen() {
        return 0;
    }

    bool getNeedDynamicBone() {
        return false;
    }

private:
};

LIGHT3D_NAMESPACE_END

#endif  // TNT_ASSETHELPERKAPU_H
