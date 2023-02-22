//
// Created by ericwywang on 2020/9/1.
//

#ifndef TNT_LOADER_H
#define TNT_LOADER_H

#include <vector>
#include <unordered_map>
#include <string>
#include <gltfio/ModelDef.h>
#include "loader/kapu_struct.h"
#include "loader/FTLoader.h"
#include "loader/GltfConverter.h"
#include "loader/MakeupLoader.h"
#include "loader/TDynamicBone.h"
#include "loader/KapuModel.h"

LIGHT3D_NAMESPACE_BEGIN

namespace CM {
    struct BinData {
        std::string binPath;
        bool needUpdate = true;
        cgltf_data *data = nullptr;
        std::map<std::string, fila_kapu_material *> kapu_materials;
        std::map<std::string, MorphData *> blendShapeMap;
        std::vector<T3D::TDynamicBone *> dynamicBones;
        std::vector<T3D::TDynamicBoneColliderBase *> colliders;
        std::vector<std::string> skeletonVec;

        ~BinData();
    };

    struct MakeupData {
        std::string binPath;
        bool needUpdate = true;
        std::string imageName;
        filament::math::float2 originSize;
        filament::math::float4 cropping;
    };

    class Loader {
    private:
        cgltf_size skinCount = 0;
        GltfConverter converter;
        KapuModel *faceModel = nullptr;
        std::string facePath = "";
//        std::vector<CM::Skeleton::Skeleton *> skeletons;
        std::map<KapuMaterialType, BinData *> binMap;
        std::map<KapuMaterialType, MakeupData> makeupMap;
        std::map<std::string, KapuMaterialType> typeMap;
        std::map<std::string, MorphData *> verticesMap;
        std::vector<std::string> skeletonVec;

        cgltf_animation *animation = nullptr;
        bool needUpdateAnimation = false;
        std::string animationPath = "";

        void initVerticeMapData();

        void loadFaceDataWithJsonPath(const std::string &path);

        void loadFaceDataWithJsonString(const std::string &jsonStr);

        void updateMorphDataForAnimojiExpression(std::map<std::string, float> &weightMap, MorphData &morphData);

        void updateMorphDataWithFaceModel();

    public:
        static const std::vector<KapuMaterialType> MODEL_ORDER;
        static const std::vector<KapuMaterialType> MAKEUP_ORDER;

        Loader() = default;

        ~Loader();

        int loadSkeletonFromPath(const std::string &path, std::vector<std::string> &skeletonVec);

        int skeleton2skin(std::vector<::CM::Skeleton::Skeleton *> &skeletons, std::vector<std::string> &skeletonVec);

        int updateSkeletonData(std::vector<::CM::Skeleton::Skeleton *> &skeletons);

        int loadAccessoryFromPath(KapuMaterialType type, const std::string &path);

        void setBinPath(std::map<std::string, std::string> pathMap);

        void clearAnimation();

        cgltf_data *getDataFromType(KapuMaterialType type);

        std::string getModelPathFromType(KapuMaterialType type);

        std::map<KapuMaterialType, MakeupData> &getMakeupMap() {
            return makeupMap;
        }

        bool getNeedUpdateFromType(KapuMaterialType type);

        void setNeedUpdateFromType(KapuMaterialType type, bool needUpdate);

        void reset();

        bool getNeedUpdateAnimation() {
            return needUpdateAnimation;
        };

        void setNeedUpdateAnimation(bool needUpdate) {
            needUpdateAnimation = needUpdate;
        };

        std::map<std::string, fila_kapu_material *> getKapuMaterialFromType(KapuMaterialType type);

        int loadMakeupFromPath(KapuMaterialType type, const std::string &path);

        int loadAnimationFromPath(const std::string &path);

        VerticesInfo *getVerticesData(std::string &nodeName);

        void setMorphData(std::string &nodeName, std::string *keyList, float **dataList, int count);

        cgltf_animation *getAnimation();

        std::vector<T3D::TDynamicBone *> getDynamicBoneData();

        std::map<std::string, MorphData *> getVerticeMap();

        // 获取当前的脸型数据 morph base data, 传入 3DMM 计算 blend shape target 数据
        std::vector<std::map<std::string, MorphData*>> getKapuFaceMorphData();

        // 根据捏脸数据 face.json 更新 morph base data 数据
        void updateMorphDataWithFaceJsonString(const std::string jsonStr);

        void updateMorphDataWithFaceJsonFile(const std::string &path);

        static void
        updateMorphWeights(std::map<std::string, float> &weightMap, MorphData &morphData, VerticesInfo &info);

        // 根据 animoji 表情检测值更新 morph result data
        void updateMorphDataWithBlendShapeWeights(std::map<std::string, float> &weightMap);

        KapuModel *getFaceModel() {
            return faceModel;
        }
    };
}

LIGHT3D_NAMESPACE_END

#endif //TNT_LOADER_H
