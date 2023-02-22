//
// Created by ericwywang on 2020/1/20.
//

#ifndef TNT_DYNAMICBONEHELPER_H
#define TNT_DYNAMICBONEHELPER_H

#include <gltfio/FilamentDynamicBone.h>
#include <gltfio/ModelDef.h>
#include <gltfio/FilamentAsset.h>
#include <utils/Entity.h>

LIGHT3D_NAMESPACE_BEGIN
class DynamicBoneHelper {
private:
    filament::Engine *engine_;
    std::vector<FilamentDynamicBoneManager> managerList;
    int currentFaceCount;

public:

    DynamicBoneHelper(filament::Engine *engine);

    void setupManager(std::vector<utils::Entity> &headList);

    utils::Entity *getEntityNameList(TTDynamicBoneParam *boneParam,
                                                 gltfio::FilamentAsset *asset);

    utils::Entity *getEntityNameList(TTDynamicBoneParam *boneParam,
                                     gltfio::FilamentInstance *asset);

    void addDynamicBone(int userIndex, TTDynamicBoneParam *boneParam, gltfio::FilamentAsset *asset);
    void addDynamicBone(int userIndex, TTDynamicBoneParam *boneParam, gltfio::FilamentInstance *asset);
    void addDynamicBone(int userIndex, TTDynamicBoneParam *boneParam, utils::Entity* entity);

    void setCurrentFaceCount(int value) {
        currentFaceCount = value;
    }

    void addDynamicBoneForKapu(int userIndex, T3D::TDynamicBone *boneModel,
                                               gltfio::FilamentAsset *asset);
    void clearDynamicBone(int userIndex);

    void updateVert(int userIndex, void *vert, int count, const filament::math::mat4f& transform);

    void updateTime(float time);

    void deleteBone(const std::string &rootName);

    bool updateBoneNoRotation(const std::string &rootName, int value);

    bool updateBoneParams(const std::string &rootName, int boneIndex, const std::string &attributeName, float value);

    bool updateBoneGravity(const std::string &rootName, float values[]);

    std::vector<FilamentDynamicBoneManager>& getManagerList();

    void setDebugMode(bool debug);

    void setEnableJobSystem(bool enableJobSystem);
};

LIGHT3D_NAMESPACE_END

#endif //TNT_DYNAMICBONEHELPER_H
