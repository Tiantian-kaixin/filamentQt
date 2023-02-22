// Copyright 2020 Tencent Inc. All Rights Reserved.
//
// Created by ericwywang on 2020/1/20.
//

#ifndef ACE3DENGINE_LIBS_GLTFIO_INCLUDE_GLTFIO_HEADHELPER_H_
#define ACE3DENGINE_LIBS_GLTFIO_INCLUDE_GLTFIO_HEADHELPER_H_

#include <string>
#include <vector>
#include "gltfio/MaterialManager.h"
#include "gltfio/ModelDef.h"
#include "gltfio/TransformHelper.h"
#include "gltfio/common_util.h"

LIGHT3D_NAMESPACE_BEGIN
static constexpr int FACE_3D_VERTEX_COUNT = 1000;
static constexpr int FACE_3D_VERTEX_COUNT_3DMM = 3448;

class HeadHelper {
private:
  filament::Engine *engine_ = nullptr;
  filament::Scene *scene_ = nullptr;
  std::vector<filament::math::float3> headVertexBuffer;
  std::vector<filament::math::float2> headUVBuffer;
  std::vector<filament::math::float3> headNormalBuffer;
  std::vector<filament::math::short4> headQuatBuffer;

  std::vector<uint16_t> headIndexBuffer;
  std::vector<TTHeadData *> headAssetList;
  bool isInitSuccess = false;
  filament::Material *filamentHeadMaterial = nullptr;
  MaterialType headType;

public:
  HeadHelper(filament::Engine *engine, filament::Scene *scene,
             SMART_PTR<MaterialManager> materialManager,
             HeadModelConfig &config);
  ~HeadHelper();

  void loadHeadData(SMART_PTR<TTFilamentSetupParam> param, int maxFaceCount,
                    bool headReceiveShadows);
  void loadHeadData(int maxFaceCount, bool headReceiveShadows);

  void setTransform(SMART_PTR<LIGHT3D::TransformHelper> transformHelper,
                    int userIndex);

  void setTransform(int userIndex, filament::math::mat4f transform);

  void
  setSpecificHeadTransform(SMART_PTR<LIGHT3D::TransformHelper> transformHelper,
                           int userIndex, filament::math::float3 scale);

  void setFaceCount(int faceCount);

  bool useHeadModelYoutu();
  void updateVert(float *vert, int count, int userIndex);

  static void headFreeCallBack(void *buffer, size_t size, void *user) {
    delete[] (float *)buffer;
  }

  void setNeedHide(bool value) { needHide = value; }

  std::vector<utils::Entity> getHeadList();

  // 获取基准头和当前头的scale比例
  filament::math::float3 getHeadDiffScale(float *headVertices,
                                          int verticeCount);

  filament::MaterialInstance *getHeadMaterialInstance() {
    return filamentHeadMaterial->getDefaultInstance();
  }

  bool needHide = false;
};

LIGHT3D_NAMESPACE_END
#endif // ACE3DENGINE_LIBS_GLTFIO_INCLUDE_GLTFIO_HEADHELPER_H_
