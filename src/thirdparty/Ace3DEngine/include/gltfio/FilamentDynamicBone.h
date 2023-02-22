//
//  FilamentDynamicBone.h
//  PituMotionDemo_QZone
//
//  Created by soarjiang on 2019/8/7.
//  Copyright © 2019年 Pitu. All rights reserved.
//

#ifndef TNT_FILAMENT_DYNAMICBONE_H
#define TNT_FILAMENT_DYNAMICBONE_H

#include <filament/Engine.h>
#include <utils/EntityInstance.h>
#include <filament/TransformManager.h>
#include <utils/Entity.h>
#include <vector>
#include <map>
#include <string>
#include <gltfio/ModelDef.h>
#include <loader/TDynamicBone.h>

#define ENABLE_DEBUG_BONE 0
#define MERGE_ANIMATION_METHOD 0
#define ENABLE_KAPU_COLLIDER 0

LIGHT3D_NAMESPACE_BEGIN

class BoneNodeParticle {
public:
    BoneNodeParticle(utils::Entity e);
    ~BoneNodeParticle();
    
    int parentIndex;
    float damping;
    float elasticity;
    float stiffness;
    float boneLength;
    float boneRadius;   // 碰撞半径
    float gravityFactor;
    float maxColliderFactor; // 最大碰撞系数
    float objectScale;
    
    utils::Entity entity;

    filament::math::mat4f transform;
    filament::math::mat4f worldTransform;

    filament::math::float3 worldPosition;
    filament::math::float3 preWorldPosition;
    filament::math::float3 initLocalPosition;
    filament::math::float4 initLocalRotation;
    filament::math::float3 initScale;
    filament::math::float3 initWorldPosition;
    filament::math::mat4f parentWorldTransform;

#if MERGE_ANIMATION_METHOD
    filament::math::float3 positionDiff;
    filament::math::float4 rotationDiff;
    
    filament::math::float3 currentPosition;
    filament::math::float4 currentRotation;
#endif
    
private:
};

typedef struct {
    float radius;   // 碰撞体半径
    filament::math::float3 center;  // 碰撞体中心

    //kapu
    utils::Entity entity;
    T3D::T_DYNAMIC_BONE_COLLIDER_DIRECTION direction;

} ColliderObject;

class FilamentDynamicBone {
public:
    
    FilamentDynamicBone(filament::Engine *engine);
    ~FilamentDynamicBone();
    
    void appendParticles(utils::Entity e, int parentIndex, float boneLength);
    void update(float deltaTime);
    void initTransforms();
    void setBoneParams(std::vector<std::map<std::string, std::string>> boneParamsList);
    void setBoneParamsForKapu(T3D::TDynamicBone *boneModel);
    void setBoneParams(std::vector<std::map<std::string, float>> boneParamsList); // studio 工具
    void setBoneCollidersForKapu(std::vector<ColliderObject> colliderList);
    bool updateBoneParams(int boneIndex, const std::string& attributeName, float value);
    bool updateBoneNoRotationUpdate(int value);
    bool updateBoneGravity(float values[]);
    filament::math::float3
    inverseTransformDirectionRecursive(BoneNodeParticle *particle, filament::math::float3 direction,
                                       filament::TransformManager &tcm);

    void setColliderParams();
    std::vector <std::map<std::string, std::string>> boneParamsList;
    std::vector<ColliderObject> colliderList;
    bool noRotationUpdate;  // 1: 每步不更新旋转角
    int updateMode; // 0: 用 178 新改的公式更新, 1: 保持旧版, 防止有老素材效果不对时可还原到旧版, 默认或者不配为0
    bool enableJobSystem;
    filament::math::float3 gravity;
    filament::math::float3 headCenter;
    int resetType;
    bool disableAnimationReset = false;
    bool isFirstUpdate;
    bool debug = false;
    bool enable = true;

    float headRadius;
    float *headVertices;
    filament::math::mat4f headTransform;

    std::string rootName;
    std::string debugRootName;
    std::string debugModelName;

private:
    
    std::vector<BoneNodeParticle *> particleArray;
    filament::Engine *engine;
    utils::Entity rootEntity;
    filament::math::float3 localGravity;
    float updateRate;
    bool isCanUpdate;
    long colliderTime;

    // kapu
    bool isKapuBone;        // 标记是否是 kapu 的骨骼模型，后续做一些特殊处理
    std::vector<T3D::TDynamicBoneColliderBase*> kapuColliders;

    bool checkRootBonePosition(filament::math::float3 worldPosition);
    bool checkResetState(filament::math::float3 pos, filament::math::float4 rot);
    void updateParticles1();
    void updateParticles2();
    void reset();
    void resetParticlesPosition();

    void applyParticlesToTransforms();
    void outsideSphere(filament::math::float3 &particlePosition, float particleRadius, float maxColliderFactor);

    filament::math::mat4f getEntityWorldTransform(utils::Entity entity, filament::TransformManager &tcm);
    filament::math::mat4f getEntityTransform(utils::Entity entity, filament::TransformManager &tcm);
    filament::math::float3 getEntityWorldPosition(utils::Entity entity, filament::TransformManager &tcm);
    filament::math::float3 getEntityPosition(utils::Entity entity, filament::TransformManager &tcm);
    filament::math::float4 getEntityRotation(utils::Entity entity, filament::TransformManager &tcm);
    filament::math::float4 getEntityWorldRotation(utils::Entity entity, filament::TransformManager &tcm);
    filament::math::float3 getEntityScale(utils::Entity entity, filament::TransformManager &tcm);
    utils::Entity *getEntityChildList(utils::Entity entity, filament::TransformManager &tcm);
    size_t getEntityChildCount(utils::Entity entity, filament::TransformManager &tcm);
    
    void setEntityPosition(utils::Entity entity, filament::TransformManager &tcm, filament::math::float3 position);
    void setEntityRotation(utils::Entity entity, filament::TransformManager &tcm, filament::math::float4 rotation);
    void setEntityScale(utils::Entity entity, filament::TransformManager &tcm, filament::math::float3 scale);
    void setEntityTransform(utils::Entity entity, filament::TransformManager &tcm, filament::math::float3 position, filament::math::float4 rotation, filament::math::float3 scale);
};


class FilamentDynamicBoneManager {
    public:
        FilamentDynamicBoneManager(filament::Engine *engine, utils::Entity headEntity);
        ~FilamentDynamicBoneManager();
    /*
        boneParamsList: 配置 stiffiness, damping, elasticity;
        noRotationUpdate: 甩动是否更新旋转角, 如果不是初始角度为 0 的关节, 计算的旋转角可能不对, 可以设置不更新
        gravity: 设置一个重力
     */
        FilamentDynamicBone* addNewDynamicBone(utils::Entity entity, TTDynamicBoneParam *boneParam);
        FilamentDynamicBone* addNewDynamicBoneForKapu(utils::Entity entity, T3D::TDynamicBone *boneModel, std::vector<ColliderObject> colliderList);
        FilamentDynamicBone* addNewDynamicBone(utils::Entity entity);
        void update(float deltaTime);
        void setHeadVertices(void *headVertices, int verticeCount, filament::math::mat4f headTransform);
    
        filament::Engine *engine;
        utils::Entity headEntity;
        filament::math::mat4f headTransform;
        std::vector<FilamentDynamicBone *> dynamicBoneList;

        std::vector<std::string> rootNameList;
        void setDebugMode(bool debug);
        void setEnableJobSystem(bool enableJobSystem);
        bool checkRootBoneEnable(FilamentDynamicBone *rootBone); // 3D工具骨骼编辑使用
    private:
        void getHeadBounds(float *headVertices, int verticeCount, filament::math::float3 &minXPoint, filament::math::float3 &maxXPoint);
        bool debug = false;     // 开启3D工具模式时, debug 设为 true
        bool enableJobSystem;
};
LIGHT3D_NAMESPACE_END
#endif //TNT_FILAMENT_DYNAMICBONE_H
