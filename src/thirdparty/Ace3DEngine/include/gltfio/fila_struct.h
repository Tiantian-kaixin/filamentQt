// Copyright (C) 2019 The Android Open Source Project
// Created by panda on 2020-02-21.
//

#ifndef LIBS_GLTFIO_INCLUDE_GLTFIO_FILA_STRUCT_H_
#define LIBS_GLTFIO_INCLUDE_GLTFIO_FILA_STRUCT_H_

#if (__has_include(<cgltf/cgltf.h>))
#include <cgltf/cgltf.h>
#else

#include "cgltf.h"

#endif

#include <string>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct fila_options : cgltf_options {
    std::string dataPath;    // 素材目录
} fila_options;

typedef struct fila_fur {
    char *noiseTexturePath;
    char *furTexturePath;
    cgltf_int shadingModel;
    cgltf_float length;
    cgltf_float spacing;
    cgltf_float gravity[4];
    cgltf_float wind[4];
    cgltf_float shadowColor[4];
    cgltf_float uvOffset[4];
    cgltf_float edgeSoftness;
    cgltf_float topEndAlpha;
    cgltf_float roughness;
    cgltf_float reflectance;
    cgltf_float anisotropy;
    cgltf_float anisotropyDirection[3];

} fila_fur;

typedef struct fila_skeleton {
    char *name;
    cgltf_float scale[3];
    cgltf_float translate[3];

} fila_skeleton;

typedef struct fila_expression_config {
    char *shapeName;
    float shapeRange[2];
    float controlledRange[2];
    char *controlledName;
} fila_expression_config;

typedef struct fila_node {
    char *name;
    cgltf_int faceMesh;
    cgltf_int blendOrder;
    fila_fur fur;
    cgltf_bool castShadow;
    cgltf_bool receiveShadow;
    cgltf_int enableExpressionConfigRemap;
    char **expressionOrderList;
    cgltf_size expressionOrderCount;
    fila_expression_config *expressionConfigList;
    cgltf_size expressionConfigCount;

} fila_node;

typedef struct fila_eye_node {
  char *name;
  cgltf_float eulerAngles[3];

} fila_eye_node;

typedef struct fila_light {
    cgltf_bool castShadow;
    cgltf_float direction[3];
    cgltf_float intensity;
    cgltf_float color[3];
} fila_light;

typedef struct fila_head_feather {
    cgltf_float offsetX;
    cgltf_float offsetY;
    cgltf_float sigma;
    cgltf_bool useSpecificHeadModel;
    cgltf_float scale[3];
    char *smallHeadName;
    char *bigHeadName;
} fila_head_feather;

typedef struct fila_dynamic_light {
    cgltf_float position[3];
    cgltf_float color[3];
    cgltf_float intensity;
    cgltf_float efficiency;
} fila_dynamic_light;

typedef struct fila_bloom {
    cgltf_float blurScale;
    char *entityName;
    cgltf_float mixAlpha;
    cgltf_float blurWeight;
    cgltf_int mixType;
} fila_bloom;

typedef struct fila_bone_param {
    cgltf_float elasticity;
    cgltf_float dampness;
    cgltf_float stiffiness;
    cgltf_float boneRadius;
    cgltf_float gravityFactor;
} fila_bone_param;

typedef struct fila_bone_collider_param {
    cgltf_float radius;
    cgltf_float positionX;
    cgltf_float positionY;
    cgltf_float positionZ;
} fila_bone_collider_param;

typedef struct fila_dynamic_bone {
    char *rootName;
    cgltf_int boneCount;
    cgltf_int noRotationUpdate;
    cgltf_float boneRadius;  //
    cgltf_float gravity[3];
    fila_bone_param *boneParamsList;
    cgltf_size boneParamsListCount;
    fila_bone_collider_param *boneColliderList;
    cgltf_size boneColliderListCount;
    cgltf_int resetType;
    //    cgltf_int enableJobSystem;
    cgltf_int updateMode;
} fila_dynamic_bone;

typedef struct fila_reflect_config {
    char *nodeName;
    char *baseColorImage;
    char *reflectImage;
    float zNormalScale;
    float blendFactor;
    int blendMode;
    float modulateAlpha;
    float reflectOffset[2];
} fila_reflect_config;

typedef struct fila_animations {
    char *name;
    cgltf_int triggerType;
    cgltf_int alwaysTriggered;
    cgltf_int playCount;
    cgltf_int isDefault;
    cgltf_float blendDuration;
    cgltf_float degreeXY;
    cgltf_bool isDegreeZ;
} fila_animations;

typedef struct fila_face_bind {
    char *name;
    cgltf_int faceIndex;
} fila_face_bind;

typedef struct fila_custom_param_sampler {
    char *file_data;
    cgltf_interpolation_type interpolation;
    cgltf_float *timelines;
    cgltf_size timelinesCount;
    cgltf_float *values;
    char **stringValues;
    cgltf_size valuesCount;

    void (*memory_free)(void *user, void *ptr);

    void *memory_user_data;
} fila_custom_param_sampler;

typedef struct fila_custom_params {
    char *type;
    char **values;
    cgltf_float valueRange[2];
    cgltf_size valuesCount;
    char *uniformName;
    char *valueType;
    char *samplerWrapMode;
    char *samplerMinFilter;
    char *valueSamplerConfigFile;
    cgltf_bool sRGBA;
    fila_custom_param_sampler *valueSampler;

    ~fila_custom_params() {
//        delete[] type;
//        for (size_t i = 0; i < valuesCount; i++) {
//            delete[] values[i];
//        }
//        delete[] values;
//        delete[] uniformName;
//        delete[] valueType;
//        delete[] samplerWrapMode;
//        delete[] valueSamplerConfigFile;
//        delete valueSampler;
    }
} fila_custom_params;

typedef struct fila_custom {
    char *matName;
    char *assetType;
    char **nodeNameList;
    cgltf_size nodeNameListCount;
    fila_custom_params *customParamsList;
    cgltf_size customParamsListCount;
} fila_custom;

typedef struct fila_glb {
    char *path;
    char *dualPath;
    char *iblPath;
    cgltf_int faceMeshType;
    char *faceMeshTextureKey;
    cgltf_int useClearIbl;
    cgltf_int furLayerCount;
    cgltf_int transparentLayerCount;
    cgltf_int iblIntensity;
    float *intensityKey;
    float *intensityValue;
    int intensitySize;
    cgltf_float blendShapeAdjustAlpha;
    cgltf_float iblRotation;
    cgltf_int iblDegree;
    cgltf_int positionType;
    cgltf_float fixedTime;
    cgltf_float scale[3];
    cgltf_float translate[3];
    cgltf_float rotate[3];
    cgltf_int flip[3];
    cgltf_int rotateType[3];
    cgltf_float autoScaleFactor[3];
    cgltf_float scaleRange[2];
    cgltf_float staticPosition[3];
    cgltf_float eulerAngles[3];

    fila_node *nodes;
    cgltf_size nodes_count;
    fila_skeleton *skeleton;
    cgltf_size skeleton_count;
    char **nodeAssociateGender;
    cgltf_size nodeAssociateGender_count;
    fila_light light;
    fila_dynamic_light *dynamicLightList;
    cgltf_size dynamicLightCount;
    fila_head_feather headFeather;
    fila_bloom bloom;
    fila_dynamic_bone *dynamicBoneList;
    cgltf_size dynamicBoneListCount;
    fila_reflect_config *reflectConfigList;
    cgltf_size reflectConfigCount;
    fila_animations *animationsList;
    cgltf_size animationsListCount;
    fila_face_bind *faceBindList;
    cgltf_size faceBindCount;
    fila_custom *customMaterials;
    cgltf_size customMaterialsCount;
    fila_eye_node *eyeNodeList;
    cgltf_size eyeNodeCount;
} fila_glb;

typedef struct fila_sticker {
    char *path;
    cgltf_size width;
    cgltf_size height;
    cgltf_size frames;
    cgltf_size frameDuration;
    cgltf_float translate[3];
    cgltf_float scale[3];
    cgltf_float rotate[3];
} fila_sticker;

typedef struct fila_BloomOptions {
    char *texturePath;
    float dirtStrength;
    float strength;
    uint32_t resolution;
    float anamorphism;
    uint8_t levels;
    char* blendMode;
    bool threshold;
    bool enabled;
}fila_BloomOptions;

typedef struct fila_data {
    void *file_data;
    cgltf_int fov;
    cgltf_int maxFaceCount;
    cgltf_int enableBloom;
    cgltf_int enableJobSystem;
    cgltf_int enableHeadFeather;
    cgltf_int toneMapping;
    cgltf_int kapuMaterialType;
    cgltf_int ssao;
    cgltf_int viewDepthPrepassDisable;
    cgltf_int faceMeshShadingModel;
    cgltf_float featureTransformAdjustAlpha;
    cgltf_float headScale[3];
    cgltf_float dualScale;

    cgltf_bool hideUserHeadModel;
    cgltf_bool headReceiveShadows;
    fila_glb *glbs;
    cgltf_size glbs_count;

    fila_sticker *stickers;
    cgltf_size stickers_count;
    fila_BloomOptions *bloomOptions;

    void (*memory_free)(void *user, void *ptr);

    void *memory_user_data;
} fila_data;

cgltf_result fila_parse(const cgltf_options *options, const void *data, cgltf_size size,
                        fila_data **out_data);

cgltf_result fila_parse_file(const cgltf_options *options, const char *path, fila_data **out_data);

void fila_free(fila_data *data);

#ifdef __cplusplus
}
#endif

#endif  // LIBS_GLTFIO_INCLUDE_GLTFIO_FILA_STRUCT_H_
