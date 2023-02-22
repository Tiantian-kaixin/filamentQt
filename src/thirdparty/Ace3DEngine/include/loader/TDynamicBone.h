#pragma once
#include <list>
#include <vector>
#include <string>
#include "loader/TDefine.h"
#include "loader/TMathHelper.h"
#include "loader/TUnityAnimationCurve.h"
#include "loader/UnityFloat3.h"
#include "loader/UnityFloat4.h"
#include "loader/TObject3D.h"

namespace T3D {
    class TObject3D;

    //*
    // 暂时屏蔽碰撞相关的代码，以后使用再说，不跟这次的版本
    class TDynamicBoneColliderBase {
    public:
        TDynamicBoneColliderBase(const std::string &boneName, T_DYNAMIC_BONE_COLLIDER_DIRECTION direction, T_DYNAMIC_BONE_COLLIDER_BOUND bound, float x, float y, float z)
            : _boneName(boneName)
            , _object(nullptr)
            , _direction(direction)
            , _bound(bound)
            , _center(x, y, z)
        {
        }
        virtual ~TDynamicBoneColliderBase() {}

        const std::string &getBoneName() const { return _boneName; }

        void bind(TObject3D *boneObject) { _object = boneObject; }
        void unbind() { _object = nullptr; }

        TObject3D* getObject() {return _object;}
        T_DYNAMIC_BONE_COLLIDER_DIRECTION getDirection() {return _direction;}
        virtual void Collide(UM::float3& inOutParticlePosition, float particleRadius) = 0;

    protected:
        std::string     _boneName;
        TObject3D*      _object = nullptr;

        T_DYNAMIC_BONE_COLLIDER_DIRECTION       _direction = T_DYNAMIC_BONE_COLLIDER_DIRECTION::DYNAMIC_BONE_COLLIDER_DIRECTION_Y;
        T_DYNAMIC_BONE_COLLIDER_BOUND           _bound = T_DYNAMIC_BONE_COLLIDER_BOUND::DYNAMIC_BONE_COLLIDER_BOUND_OUTSIDE;

        UM::float3          _center;
    };

    class TDynamicBonePlaneCollider : public TDynamicBoneColliderBase {
    public:
        TDynamicBonePlaneCollider(const std::string &boneName, T_DYNAMIC_BONE_COLLIDER_DIRECTION direction, T_DYNAMIC_BONE_COLLIDER_BOUND bound, float x, float y, float z)
            : TDynamicBoneColliderBase(boneName, direction, bound, x, y, z) {}
        virtual ~TDynamicBonePlaneCollider() {}

        virtual void Collide(UM::float3& inOutParticlePosition, float particleRadius);
    };

    class TDynamicBoneCollider : public TDynamicBoneColliderBase {
    public:
        TDynamicBoneCollider(const std::string &boneName, T_DYNAMIC_BONE_COLLIDER_DIRECTION direction, T_DYNAMIC_BONE_COLLIDER_BOUND bound, float x, float y, float z)
            : TDynamicBoneColliderBase(boneName, direction, bound, x, y, z) {}
        virtual ~TDynamicBoneCollider() {}

        virtual void Collide(UM::float3& inOutParticlePosition, float particleRadius);

        void setRadiusAndHeight(float radius, float height) {
            _radius = radius;
            _height = height;
        }

        float getRadius() {return _radius;}

    protected:
        void OutsideSphere(UM::float3& inOutParticlePosition, float particleRadius, const UM::float3& inSphereCenter, float sphereRadius);
        void InsideSphere(UM::float3& inOutParticlePosition, float particleRadius, const UM::float3& inSphereCenter, float sphereRadius);
        void OutsideCapsule(UM::float3& inOutParticlePosition, float particleRadius, const UM::float3& inCapsuleP0, const UM::float3& inCapsuleP1, float capsuleRadius);
        void InsideCapsule(UM::float3& inOutParticlePosition, float particleRadius, const UM::float3& inCapsuleP0, const UM::float3& inCapsuleP1, float capsuleRadius);

    private:
        float       _radius = 0.0f;
        float       _height = 0.0f;
    };
    // */

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    struct TParticle {
        TObject3D*      _object = nullptr;
        int             _parentIndex = -1;
        float           _damping = 0.0f;
        float           _elasticity = 0.0f;
        float           _stiffness = 0.0f;
        float           _inert = 0.0f;
        float           _radius = 0.0f;
        float           _boneLength = 0.0f;

        UM::float3          _position;
        UM::float3          _prevPosition;
        UM::float3          _endOffset;
        UM::float3          _initLocalPosition;
        UM::float4          _initLocalRotation = { 0.0f, 0.0f, 0.0f, 1.0f };
    };

    class TDynamicBone {
    public:

        TDynamicBone(
                const std::string &rootName,
                float damping,
                float elasticity,
                float stiffness,
                float inert,
                float radius
        );
        ~TDynamicBone();

        void update(float dt);

    public:
        void setupParticles();

        void initTransforms();

        void updateParameters();

        void resetParticlesPosition();

    protected:
        void appendParticles(TObject3D* object, int parentIndex, float boneLength);

        void updateParticles1();
        void updateParticles2();
        void skipUpdateParticles();

        void applyParticlesToTransforms();

    public:
        const std::string &getRootName() const { return _rootName; }

        float getDamping() const { return _damping; }
        float getElasticity() const { return _elasticity; }
        float getStiffness() const { return _stiffness; }

        float getObjectScale() const {return _objectScale;}
        float getRadius() const {return _radius;}
        std::vector<TDynamicBoneColliderBase*> getColliders() {return _colliders;}

        void setUpdateRate(float updateRate) { _updateRate = updateRate; }
        void setEndLength(float endLength) { _endLength = endLength; }
        void setFreezeAxis(T_DYNAMIC_BONE_FREEZE_AXIS freezeAxis) { _freezeAxis = freezeAxis; }

        void setEndOffset(float *endOffset) {
            _endOffset[0] = endOffset[0];
            _endOffset[1] = endOffset[1];
            _endOffset[2] = endOffset[2];
        }

        void setGravity(float *gravity) {
            _gravity[0] = gravity[0];
            _gravity[1] = gravity[1];
            _gravity[2] = gravity[2];
        }

        void setForce(float *force) {
            _force[0] = force[0];
            _force[1] = force[1];
            _force[2] = force[2];
        }

        void bind(TObject3D *bindRoot, TObject3D *root);

        void unbind(bool disable = true);

        /* 这个接口不提供，直接在初始化的时候就设置进来
        inline void setWeight(float w) {
            if (_weight != w) {
                if (w == 0.0f) {
                    initTransforms();
                }
                else if (_weight == 0.0f) {
                    resetParticlesPosition();
                }

                _weight = w;
            }
        }
        inline float getWeight() const { return _weight; }
        // */

        void addCollider(TDynamicBoneColliderBase *collider);
        void removeCollider(TDynamicBoneColliderBase *collider);
        void removeCollider(const std::string &name);

    private:
        std::string _rootName;

        TObject3D*  _bindRoot = nullptr;
        TObject3D*  _root = nullptr;

        float       _updateRate = 60.0f;
        float       _time = 0.0f;

        // 这玩意是 unity 才有的，我们这里没有，注释掉注释掉
        //T_DYNAMIC_BONE_UPDATE_MODE      _updateMode = T_DYNAMIC_BONE_UPDATE_MODE::DYNAMIC_BONE_UPDATE_MODE_NORMAL;

        // How much the bones slowed down.
        float       _damping = 0.1f;            // range: 0 - 1
        // 暂时没有这个动画曲线啥事
        // AnimationCurve*     _dampingDistrib = nullptr;

        // How much the force applied to return each bone to original orientation.
        float       _elasticity = 0.1f;         // range: 0 - 1
        // 暂时没有这个动画曲线啥事
        // AnimationCurve*     _elasticityDistrib = nullptr;

        // How much bone's original orientation are preserved.
        float       _stiffness = 0.1f;          // range: 0 - 1
        // 暂时没有这个动画曲线啥事
        // AnimationCurve*     _stiffnessDistrib = nullptr;

        // How much character's position change is ignored in physics simulation.
        float       _inert = 0.0f;              // range: 0 - 1
        // 暂时没有这个动画曲线啥事
        // AnimationCurve*     _inertDistrib = nullptr;

        // Each bone can be a sphere to collide with colliders. Radius describe sphere's size.
        float       _radius = 10.0f;
        // 暂时没有这个动画曲线啥事
        // AnimationCurve*     _radiusDistrib = nullptr;

        // If End Length is not zero, an extra bone is generated at the end of transform hierarchy.
        float       _endLength = 0.0f;

        // If End Offset is not zero, an extra bone is generated at the end of transform hierarchy.
        UM::float3      _endOffset;

        // The force apply to bones. Partial force apply to character's initial pose is cancelled out.
        UM::float3      _gravity;

        // The force apply to bones.
        UM::float3      _force;

        // Collider objects interact with the bones.
        std::vector<TDynamicBoneColliderBase*>    _colliders;

        //*
        // 暂时屏蔽，这个是从外部传进来的，或者是从配置加载的
        // Bones exclude from physics simulation.
        std::vector<TObject3D*>           _exclusions;
        // */

        //*
        // 这个也先屏蔽掉
        // Constrain bones to move on specified plane.
        T_DYNAMIC_BONE_FREEZE_AXIS      _freezeAxis = T_DYNAMIC_BONE_FREEZE_AXIS::DYNAMIC_BONE_FREEZE_AXIS_NONE;
        // */

        /*
        // 在我们这里，不存在这种距离的问题，注释掉注释掉
        // Disable physics simulation automatically if character is far from camera or player.
        bool            _distantDisable = false;
        bool            _distantDisabled = false;
        TObject3D*      _referenceObject = nullptr;
        float           _distanceToObject = 20.0f;
        // */

        //
        UM::float3          _localGravity;
        UM::float3          _objectMove;
        UM::float3          _objectPrevPosition;

        float           _boneTotalLength = 0.0f;

        float           _objectScale = 1.0f;
        float           _weight = 1.0f;

        std::vector<TParticle*>   _particles;
    };
}
