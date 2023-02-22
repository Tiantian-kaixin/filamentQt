#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include <unordered_map>
#include "loader/TDefine.h"
#include "loader/TMathHelper.h"
#include "loader/UnityFloat3.h"
#include "loader/UnityFloat4.h"
#include "loader/UnityMatrix4.h"

namespace T3D {
    // float buffer offset
    const int T_OBJECT_3D_BUFFER_OFFSET_POSITION            = 0;            // 0, 1, 2      =>  position.xyz                object 3d
    const int T_OBJECT_3D_BUFFER_OFFSET_ROTATION_EULER      = 3;            // 3, 4, 5      =>  rotation.xyz                object 3d
    const int T_OBJECT_3D_BUFFER_OFFSET_QUATERNION          = 6;            // 6, 7, 8, 9   =>  quaternion.xyzw             object 3d
    const int T_OBJECT_3D_BUFFER_OFFSET_SCALE               = 10;           // 10, 11, 12   =>  scale.xyz                   object 3d
    const int T_OBJECT_3D_BUFFER_OFFSET_LOCAL_MATRIX        = 13;           // 13 - 28      =>  local matrix                object 3d
    const int T_OBJECT_3D_BUFFER_OFFSET_WORLD_MATRIX        = 29;           // 29 - 44      =>  world matrix                object 3d

    class TDynamicBone;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class TObject3D {
    public:
        TObject3D(int32_t* bufferInt, float* bufferFloat);
        TObject3D();
        virtual ~TObject3D();

        virtual T_OBJECT_TYPE getObjectType() { return T_OBJECT_TYPE::OBJECT_3D; }
        virtual void update(float dt);

    protected:
        virtual void preUpdate(float dt);
        virtual void updateSelf(float dt);
        virtual void lateUpdate(float dt);

    public:
        virtual void add(TObject3D* child);
        virtual void add(const std::vector<TObject3D*>& child_list);

        virtual void remove(TObject3D* child);
        virtual void remove(const std::vector<TObject3D*>& child_list);

        void enableDynamicBone(TObject3D *root, TDynamicBone *db);
        void disableDynamicBone(TObject3D *root);
        void resetDynamicBone();

    public:
        virtual void updateMatrix();
        virtual void updateMatrixWorld(bool force);
        virtual void updateWorldMatrix(bool updateParent, bool updateChildren);
        virtual void setObjectMatrix(float* matrix,float* matrixWorld);

        TObject3D* getChildByName(const std::string& name);
        float* getPropertyByName(const std::string& name, int& valueSize);

    public:
        UM::float3 inverseTransformDirection(const UM::float3& direction, bool scaleQuat = false);
        UM::float3 inverseTransformPoint(const UM::float3& point);

        UM::float3 transformDirection(const UM::float3& dir);
        UM::float3 transformPoint(const UM::float3& inPos);

        float getLossyScaleX();

        //*
        UM::float3 getWorldRightVector();
        UM::float3 getWorldUpVector();
        UM::float3 getWorldForwardVector();
        // */

        UM::float4 getWorldQuaternion();
        void setWorldQuaternion(const UM::float4& quaternion);

        UM::float3 getWorldPosition();
        void setWorldPosition(const UM::float3& position);

        void setPositionD(const UM::float3& position);
        void setQuaternionD(const UM::float4& quaternion);

        /*
        UM::float3 getPositionD();
        UM::float4 getQuaternionD();

        UM::matrix4 localToWorldMatrix();
        // */
        UM::float3 localToWorld(const UM::float3& position);
        UM::float3 worldToLocal(const UM::float3& position);

        int32_t                 entityId;

    protected:
        void inverseTransformPointParent(UM::float3& point);
        void inverseTransformRotationParent(UM::float4& rotation);

        UM::float3 inverseTransformDirectionNonRecursive(const UM::float3& direction, bool scaleQuat = false);

    public:
        inline void setName(const std::string& name) { _name = name; }
        inline const std::string& getName() const { return _name; }
        inline void setUUID(const std::string& uuid) { _uuid = uuid; }
        inline const std::string& getUUID() const { return _uuid; }

        inline float* getPosition() { return _bufferFloat + T_OBJECT_3D_BUFFER_OFFSET_POSITION; }
        inline float* getRotationEuler() { return _bufferFloat + T_OBJECT_3D_BUFFER_OFFSET_ROTATION_EULER; }
        inline float* getQuaternion() { return _bufferFloat + T_OBJECT_3D_BUFFER_OFFSET_QUATERNION; }
        void setQuaternion(float* quaternion);
        inline float* getScale() { return _bufferFloat + T_OBJECT_3D_BUFFER_OFFSET_SCALE; }
        inline float* getLocalMatrix() { return _matrix; }
        inline float* getWorldMatrix() { return _matrixWorld; }

        inline TObject3D* getParent() const { return _parent; }
        inline const std::vector<TObject3D*>& getChildren() const { return _children; }

        inline void setMatrixWorldNeedsUpdate(bool needsUpdate) { _matrixWorldNeedsUpdate = needsUpdate; }

        float*                  _matrix = nullptr;
        float*                  _matrixWorld = nullptr;

    protected:
        std::string             _name;
        std::string             _uuid;

        // 0: id
        // 1: update flag
        int32_t*                _bufferInt = nullptr;
        float*                  _bufferFloat = nullptr;

        TObject3D*              _parent = nullptr;
        std::vector<TObject3D*>   _children;

        bool                    _matrixWorldNeedsUpdate = true;

        bool                    _needUpdateLossyScaleX = true;
        float                   _lossyScaleX = 1.0f;
        std::unordered_map<TObject3D*, TDynamicBone*>  _dynamicBones;
    };
}
