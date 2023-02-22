//
// Created by guangjinhuo on 2019-02-14.
//

#pragma once

#include "loader/Config.h"
#include "loader/TlvReader.h"
#include "loader/TDynamicBone.h"

namespace cm {

    T3D::TDynamicBoneColliderBase * loadDynamicBoneCollider(CMTLVReader &reader);
    T3D::TDynamicBone * loadDynamicBone(CMTLVReader &reader);
}
