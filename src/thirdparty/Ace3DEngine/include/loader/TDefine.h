#pragma once

namespace T3D {
    enum T_UPDATE_FLAG {
        MATRIX_AUTO_UPDATE      = 0x00000001,
        POSITION                = 0x00000002,
        ROTATION_EULER          = 0x00000004,
        QUATERNION              = 0x00000008,
        SCALE                   = 0x00000010,
    };

    enum T_OBJECT_TYPE {
        OBJECT_3D,
        SKINNED_MESH,
    };

    enum T_INTERPOLANT_TYPE {
        INTERPOLANT_LINEAR,
        INTERPOLANT_QUATERNION_LINEAR,
        INTERPOLANT_SMOOTH,
        INTERPOLANT_DISCRETE,
    };

    enum T_TRACK_TYPE {
        TRACK_VECTOR        = 1,    // Position / Scale Track
        TRACK_QUATERNION    = 2,    // Rotation Track
        TRACK_NUMBER        = 3,    // Morph Track
    };

    enum T_LOOP_MODE {
        LOOP_ONCE       = 2200,
        LOOP_REPEAT     = 2201,
        LOOP_PING_PONG  = 2202,
    };

    enum T_EVENT_TYPE {
        ANIMATION_ACTION_FINISHED  = 1,
        ANIMATION_ACTION_LOOP      = 2,
    };

    //*
    // 暂时屏蔽碰撞相关的代码，以后使用再说，不跟这次的版本
    enum T_DYNAMIC_BONE_COLLIDER_DIRECTION {
        DYNAMIC_BONE_COLLIDER_DIRECTION_X,
        DYNAMIC_BONE_COLLIDER_DIRECTION_Y,
        DYNAMIC_BONE_COLLIDER_DIRECTION_Z,
    };

    enum T_DYNAMIC_BONE_COLLIDER_BOUND {
        DYNAMIC_BONE_COLLIDER_BOUND_OUTSIDE,
        DYNAMIC_BONE_COLLIDER_BOUND_INSIDE,
    };
    // */

    /*
    // 这玩意是 unity 才有的，我们这里没有，注释掉注释掉
    enum T_DYNAMIC_BONE_UPDATE_MODE {
        DYNAMIC_BONE_UPDATE_MODE_NORMAL,
        DYNAMIC_BONE_UPDATE_MODE_ANIMATE_PHYSICS,
        DYNAMIC_BONE_UPDATE_MODE_UNSCALED_TIME,
    };
    // */

    //*
    // 这个也先屏蔽掉
    enum T_DYNAMIC_BONE_FREEZE_AXIS {
        DYNAMIC_BONE_FREEZE_AXIS_NONE,
        DYNAMIC_BONE_FREEZE_AXIS_X,
        DYNAMIC_BONE_FREEZE_AXIS_Y,
        DYNAMIC_BONE_FREEZE_AXIS_Z,
    };
    // */
}
