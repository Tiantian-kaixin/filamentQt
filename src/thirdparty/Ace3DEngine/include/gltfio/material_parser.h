//
// Created by kevinxing on 2019-11-04.
//

#ifndef TNT_MATERIAL_PARSER_H
#define TNT_MATERIAL_PARSER_H

#include <string>
#include <gltfio/file_util.h>
#include "custom_shader_parser.h"
#include <gltfio/log_util.h>
#include <gltfio/json.hpp>

LIGHT3D_NAMESPACE_BEGIN

    namespace parser {

        using namespace nlohmann;

        template<typename String, typename OutputIt>
        void components_separated_by_string(const String &string, const String &separator, OutputIt out) {
            typename String::size_type comp_start = 0, comp_end;
            while ((comp_end = string.find(separator, comp_start)) != String::npos) {
                *out++ = string.substr(comp_start, comp_end - comp_start);
                comp_start = comp_end + separator.size();
            }
            if (comp_start || string.size())
                *out++ = string.substr(comp_start);
        }

        template<typename String, typename OutputContainer = std::vector<String>>
        OutputContainer components_separated_by_string(const String &string, const String &separator) {
            OutputContainer container;
            components_separated_by_string(string, separator, std::back_inserter(container));

            return container;
        }

        TTThreeDMaterialModel *parseModel(fila_glb *glbAsset, const utils::Path &materialDir, bool loadGlb);

        TTStickerModel *parseStickerModel(fila_sticker *glbAsset, const utils::Path &materialDir);

        void parseExpressionOrderList(std::unordered_map<std::string, size_t> &expOrderMap, char **expOrderList,
                                      cgltf_size expOrderCount);

        void parseExpressionConfigList(std::vector<SMART_PTR<TTExpressionConfig>> &expConfigs,
                                       fila_expression_config *expConfigList, cgltf_size expConfigCount);

        TTFilamentSetupParam *parseParams(fila_data *sourceAsset, const utils::Path &materialDir,
                                          bool loadGlb) {
            auto *param = new TTFilamentSetupParam;
            param->fovAngle = sourceAsset->fov;
            param->dualScale = sourceAsset->dualScale;

            constexpr int kMinFaceCount = 1;
            constexpr int kMaxFaceCount = 3;
            param->maxFaceCount = 1;
//                    std::min(kMaxFaceCount, std::max(sourceAsset->maxFaceCount, kMinFaceCount));

            param->featureTransformAdjustAlpha = sourceAsset->featureTransformAdjustAlpha;
            param->hideUserHeadModel = sourceAsset->hideUserHeadModel;
            param->headReceiveShadows = sourceAsset->headReceiveShadows;
            param->enableBloom = sourceAsset->enableBloom;
            param->enableJobSystem = sourceAsset->enableJobSystem;
            param->enableHeadFeather = sourceAsset->enableHeadFeather;
            param->headScale = filament::math::float3(sourceAsset->headScale[0], sourceAsset->headScale[1],
                                                      sourceAsset->headScale[2]);
            param->kapuMaterialType = sourceAsset->kapuMaterialType;

            param->toneMapping = sourceAsset->toneMapping;
            param->ssao = sourceAsset->ssao;
            param->faceMeshShadingModel = sourceAsset->faceMeshShadingModel;
            param->viewDepthPrepassDisable = sourceAsset->viewDepthPrepassDisable;
            param->materialDir = materialDir;
            for (uint32_t i = 0; i < sourceAsset->glbs_count; i++) {
                if (sourceAsset->glbs[i].path) {
                    auto model = parseModel(&sourceAsset->glbs[i], materialDir, loadGlb);
                    model->modelDir = ".";
                    param->materialModels.push_back(model);
                }
            }
            for (uint32_t i = 0; i < sourceAsset->stickers_count; i++) {
                if (sourceAsset->stickers[i].path) {
                    param->stickers.push_back(parseStickerModel(&sourceAsset->stickers[i], materialDir));
                }
            }
            return param;
        }

        TTMaterialHeadFeatherParam *parseHeadFeather(fila_head_feather *headFeatherAsset,
                                                     const utils::Path &materialDir) {
            auto *headFeather = new TTMaterialHeadFeatherParam();
            headFeather->sigma = headFeatherAsset->sigma;
            headFeather->offsetX = headFeatherAsset->offsetX;
            headFeather->offsetY = headFeatherAsset->offsetY;
            headFeather->useSpecificHeadModel = headFeatherAsset->useSpecificHeadModel;
            headFeather->scale = filament::math::float3(
                    headFeatherAsset->scale[0], headFeatherAsset->scale[1], headFeatherAsset->scale[2]);
            if (headFeatherAsset->smallHeadName != nullptr) {
                headFeather->smallHeadName = headFeatherAsset->smallHeadName;
            } else {
                headFeather->smallHeadName = "head_small.glb";
            }
            if (headFeatherAsset->bigHeadName != nullptr) {
                headFeather->bigHeadName = headFeatherAsset->bigHeadName;
            } else {
                headFeather->bigHeadName = "head_big.glb";
            }
            return headFeather;
        }

        TTStickerModel *parseStickerModel(fila_sticker *stickerAsset, const utils::Path &materialDir) {
            auto *model = new TTStickerModel();
            model->path = stickerAsset->path;
            model->width = stickerAsset->width;
            model->height = stickerAsset->height;
            model->frames = stickerAsset->frames;
            model->frameDuration = stickerAsset->frameDuration;
            model->translate = filament::math::float3(
                    stickerAsset->translate[0], stickerAsset->translate[1], stickerAsset->translate[2]);
            model->scale = filament::math::float3(stickerAsset->scale[0], stickerAsset->scale[1],
                                                  stickerAsset->scale[2]);
            model->rotate = filament::math::float3(stickerAsset->rotate[0], stickerAsset->rotate[1],
                                                   stickerAsset->rotate[2]);
            return model;
        }

        TTMaterialLightParam *parseLight(fila_light *lightAsset, const utils::Path &materialDir) {
            auto *light = new TTMaterialLightParam();
            light->castShadow = lightAsset->castShadow;
            light->direction = filament::math::float3(lightAsset->direction[0], lightAsset->direction[1],
                                                      lightAsset->direction[2]);
            light->intensity = lightAsset->intensity;
            light->color =
                    filament::math::float3(lightAsset->color[0], lightAsset->color[1], lightAsset->color[2]);
            return light;
        }

        std::vector<TTMaterialDynamicLightParam *>
        parseDynamicLightList(fila_dynamic_light *dynamicLightList, int dynamicLightCount) {
            std::vector<TTMaterialDynamicLightParam *> vector;
            for (int i = 0; i < dynamicLightCount; i++) {
                auto *light = new TTMaterialDynamicLightParam();
                light->intensity = dynamicLightList[i].intensity;
                light->efficiency = dynamicLightList[i].efficiency;
                light->color = filament::math::float3(dynamicLightList[i].color[0],
                                                      dynamicLightList[i].color[1],
                                                      dynamicLightList[i].color[2]);
                light->position = filament::math::float3(dynamicLightList[i].position[0],
                                                         dynamicLightList[i].position[1],
                                                         dynamicLightList[i].position[2]);
                vector.push_back(light);
            }
            return vector;
        }

        TTMaterialBloomParam *parseBloom(fila_bloom *bloomAsset) {
            auto *bloom = new TTMaterialBloomParam();
            bloom->blurScale = bloomAsset->blurScale;
            bloom->blurWeight = bloomAsset->blurWeight;
            bloom->mixAlpha = bloomAsset->mixAlpha;
            bloom->mixType = bloomAsset->mixType;
            if (bloomAsset->entityName) {
                bloom->bloomEntityNameList =
                        components_separated_by_string<std::string>(bloomAsset->entityName, ";");
            }
            return bloom;
        }

        TTMaterialFurParam *parseFur(fila_fur *furAsset, const utils::Path &materialDir) {
            auto *fur = new TTMaterialFurParam();
            fur->noiseTexturePath = materialDir.getAbsolutePath().concat(furAsset->noiseTexturePath);
            fur->furTexturePath = materialDir.getAbsolutePath().concat(furAsset->furTexturePath);
            fur->shadingModel =
                    furAsset->shadingModel == 1 ? filament::Shading::LIT : filament::Shading::UNLIT;
            fur->length = furAsset->length;
            fur->spacing = furAsset->spacing;
            fur->gravity = filament::math::float4(furAsset->gravity[0], furAsset->gravity[1],
                                                  furAsset->gravity[2], furAsset->gravity[3]);
            fur->wind = filament::math::float4(furAsset->wind[0], furAsset->wind[1], furAsset->wind[2],
                                               furAsset->wind[3]);
            fur->shadowColor = filament::math::float4(furAsset->shadowColor[0], furAsset->shadowColor[1],
                                                      furAsset->shadowColor[2], furAsset->shadowColor[3]);
            filament::math::float4 &shadow_color = fur->shadowColor;
            if (shadow_color[0] > 1.0001f || shadow_color[1] > 1.0001f ||
                shadow_color[2] > 1.0001f) {  // TODO(@dianxin) : patch code. 兼容一个素材工具的配置逻辑.
                shadow_color = {(shadow_color.rgb - 0.5f) / 255,
                                shadow_color.a};  // RGB value from [0-255] to [0-1]
            }
            fur->uvOffset = filament::math::float4(furAsset->uvOffset[0], furAsset->uvOffset[1],
                                                   furAsset->uvOffset[2], furAsset->uvOffset[3]);
            fur->anisotropyDirection =
                    filament::math::float3(furAsset->anisotropyDirection[0], furAsset->anisotropyDirection[1],
                                           furAsset->anisotropyDirection[2]);

            fur->edgeSoftness = furAsset->edgeSoftness;
            fur->topEndAlpha = furAsset->topEndAlpha;
            fur->roughness = furAsset->roughness;
            fur->reflectance = furAsset->reflectance;
            fur->anisotropy = furAsset->anisotropy;
            return fur;
        }

        std::vector<TTDynamicBoneParam *> parseDynamicBoneList(fila_dynamic_bone *dynamicBoneList,
                                                               int dynamicBoneListCount,
                                                               const utils::Path &materialDir,
                                                               bool loadGlb) {
            int boneCount = dynamicBoneListCount;
            std::vector<TTDynamicBoneParam *> vec;
            for (int i = 0; i < boneCount; ++i) {
                fila_dynamic_bone boneModel = dynamicBoneList[i];
                std::vector<std::string> items =
                        components_separated_by_string<std::string>(boneModel.rootName, ";");
                int itemCount = items.size();
                for (int k = 0; k < itemCount; ++k) {
                    TTDynamicBoneParam *param = new TTDynamicBoneParam;
                    std::string str1 = items[k];  //
                    param->rootName = str1;
                    param->boneCount = boneModel.boneCount;
                    int count = boneModel.boneParamsListCount;
                    std::vector<std::map<std::string, std::string>> paramsMapVector;
                    for (int j = 0; j < count; ++j) {
                        std::map<std::string, std::string> map;
                        fila_bone_param boneParam = boneModel.boneParamsList[j];
                        map["gravityFactor"] = std::to_string(boneParam.gravityFactor);
                        map["boneRadius"] = std::to_string(boneParam.boneRadius);
                        map["stiffiness"] = std::to_string(boneParam.stiffiness);
                        map["dampness"] = std::to_string(boneParam.dampness);
                        map["elasticity"] = std::to_string(boneParam.elasticity);
                        auto search1 = map.find("gravityFactor");
                        if (search1 == map.end()) {
                            LOGE << "gravityFactor not exist" << LOGEND;
                        }

                        paramsMapVector.push_back(map);
                    }
                    param->boneParamsList = paramsMapVector;

                    std::vector<std::map<std::string, std::string>> colliderVector;
                    count = boneModel.boneColliderListCount;
                    for (int j = 0; j < count; j++) {
                        std::map<std::string, std::string> map;
                        fila_bone_collider_param boneColliderParam = boneModel.boneColliderList[j];
                        map["radius"] = std::to_string(boneColliderParam.radius);
                        map["positionX"] = std::to_string(boneColliderParam.positionX);
                        map["positionY"] = std::to_string(boneColliderParam.positionY);
                        map["positionZ"] = std::to_string(boneColliderParam.positionZ);
                        colliderVector.push_back(map);
                    }
                    param->colliderList = colliderVector;
                    param->boneRadius = boneModel.boneRadius;
                    param->noRotationUpdate = boneModel.noRotationUpdate;
                    param->resetType = boneModel.resetType;
                    param->updateMode = boneModel.updateMode;
                    param->gravity = filament::math::float3(boneModel.gravity[0], boneModel.gravity[1],
                                                            boneModel.gravity[2]);
                    vec.push_back(param);
                }
            }
            return vec;
        }

        std::vector<TTReflectConfig *> parseReflectConfigList(fila_reflect_config *reflectConfigList,
                                                              int reflectConfigCount,
                                                              const utils::Path &materialDir) {
            std::vector<TTReflectConfig *> vec;
            for (int i = 0; i < reflectConfigCount; ++i) {
                fila_reflect_config reflectConfigSrc = reflectConfigList[i];
                TTReflectConfig *reflectConfig = new TTReflectConfig;
                reflectConfig->nodeName = reflectConfigSrc.nodeName;
                reflectConfig->baseColorImage =
                        materialDir.getAbsolutePath().concat(reflectConfigSrc.baseColorImage);
                reflectConfig->reflectImage =
                        materialDir.getAbsolutePath().concat(reflectConfigSrc.reflectImage);
                reflectConfig->zNormalScale = reflectConfigSrc.zNormalScale;
                reflectConfig->blendFactor = reflectConfigSrc.blendFactor;
                reflectConfig->modulateAlpha = reflectConfigSrc.modulateAlpha;
                reflectConfig->blendMode = reflectConfigSrc.blendMode;
                reflectConfig->reflectOffset = filament::math::float2(reflectConfigSrc.reflectOffset[0],
                                                                      reflectConfigSrc.reflectOffset[1]);
                vec.push_back(reflectConfig);
            }
            return vec;
        }

        TriggerableAnimationItem parseAnimation(fila_animations &animations) {
            TriggerableAnimationItem result;
            result.name = animations.name;
            result.count = animations.playCount;
            result.blendDuration = animations.blendDuration;
            result.degreeXY = animations.degreeXY;
            result.isDegreeZ = animations.isDegreeZ;
            result.isDefault = animations.isDefault == 1;
            return result;
        }

        TTFaceBindConfig parseFaceBind(fila_face_bind &faceBind) {
            TTFaceBindConfig config;
            config.entityName = faceBind.name;
            config.faceIndex = faceBind.faceIndex;
            return config;
        }

        std::vector<TTSkeleton *> parseSkeletonList(fila_skeleton *skeletonList, int skeletonCount) {
            std::vector<TTSkeleton *> vec;
            for (int i = 0; i < skeletonCount; ++i) {
                fila_skeleton skeletonSrc = skeletonList[i];
                TTSkeleton *skeleton = new TTSkeleton;
                skeleton->nodeName = skeletonSrc.name;
                skeleton->translate = filament::math::float3(
                        skeletonSrc.translate[0], skeletonSrc.translate[1], skeletonSrc.translate[2]);
                skeleton->scale = filament::math::float3(skeletonSrc.scale[0], skeletonSrc.scale[1],
                                                         skeletonSrc.scale[2]);
                vec.push_back(skeleton);
            }
            return vec;
        }

        TTThreeDMaterialModel *parseModel(fila_glb *glbAsset, const utils::Path &materialDir,
                                          bool loadGlb) {
            auto *model = new TTThreeDMaterialModel();
            model->modelDir = materialDir;

            model->modelName = glbAsset->path;
            // TODO(@Dianxin) : hard code defect
            if (LIGHT3D::file::hasExtension(model->modelName, "bin")) {
                model->glbLoadType = TTThreeDMaterialModel::GlbLoadType::bin;
            } else if (LIGHT3D::file::hasExtension(model->modelName, "glb") ||
                       LIGHT3D::file::hasExtension(model->modelName, "dat")) {
                model->glbLoadType = TTThreeDMaterialModel::GlbLoadType::glb;
            } else if (LIGHT3D::file::hasExtension(model->modelName, "gltf") ||
                       LIGHT3D::file::hasExtension(model->modelName, "datf")) {
                model->glbLoadType = TTThreeDMaterialModel::GlbLoadType::gltf;
            }

            if (loadGlb && model->glbLoadType != TTThreeDMaterialModel::GlbLoadType::bin) {
                std::string modelName = materialDir.getAbsolutePath().concat(glbAsset->path);
                std::vector<uint8_t> buffer = LIGHT3D::file::loadFile(modelName);
                model->glbData.swap(buffer);

                if (glbAsset->dualPath != nullptr) {
                    auto dualPath = materialDir.getAbsolutePath().concat(glbAsset->dualPath);
                    if (LIGHT3D::file::fileExist(dualPath)) {
                        auto bufferD = LIGHT3D::file::loadFile(modelName);
                        model->dualGlb.swap(bufferD);
                    }
                }
            }

            if (glbAsset->dualPath != nullptr) {
                model->dualPath = glbAsset->dualPath;
            }
            model->faceMeshType = glbAsset->faceMeshType;
            model->modelName = glbAsset->path;
            // TODO(@Dianxin) : hard code defect
            if (LIGHT3D::file::hasExtension(model->modelName, "glb") ||
                LIGHT3D::file::hasExtension(model->modelName, "dat")) {
                model->glbLoadType = TTThreeDMaterialModel::GlbLoadType::glb;
            } else if (LIGHT3D::file::hasExtension(model->modelName, "gltf") ||
                       LIGHT3D::file::hasExtension(model->modelName, "datf")) {
                model->glbLoadType = TTThreeDMaterialModel::GlbLoadType::gltf;
            }
            model->furLayerCount = glbAsset->furLayerCount;
            model->transparentLayerCount = glbAsset->transparentLayerCount;

            if (glbAsset->iblPath) {
                model->iblPath = materialDir.getAbsolutePath().concat(glbAsset->iblPath);
            }
            model->useClearIbl = glbAsset->useClearIbl;
            model->iblIntensity = glbAsset->iblIntensity;
            model->iblRotation = glbAsset->iblRotation;
            model->iblDegree = glbAsset->iblDegree;
            model->needFaceMesh = 0;
            model->fixedTime = glbAsset->fixedTime;
            if (glbAsset->faceMeshTextureKey) {
                model->faceMeshTextureKey = glbAsset->faceMeshTextureKey;
            }
            model->intensityKey = std::vector<float>(glbAsset->intensityKey,
                                                     glbAsset->intensityKey + glbAsset->intensitySize);
            model->intensityValue = std::vector<float>(glbAsset->intensityValue,
                                                       glbAsset->intensityValue + glbAsset->intensitySize);
            model->positionType = glbAsset->positionType;
            model->scale =
                    filament::math::float3(glbAsset->scale[0], glbAsset->scale[1], glbAsset->scale[2]);
            model->translate = model->staticPosition = filament::math::float3(
                    glbAsset->translate[0], glbAsset->translate[1], glbAsset->translate[2]);
#if (FILAMENT_ENV_IOS || (IOS))
            if (glbAsset->staticPosition[0] > 1e-5 || glbAsset->staticPosition[1] > 1e-5 ||
                glbAsset->staticPosition[2] > 1e-5) {
                model->staticPosition = filament::math::float3(
                    glbAsset->staticPosition[0], glbAsset->staticPosition[1], glbAsset->staticPosition[2]);
            }
#endif
            model->rotate =
                    filament::math::float3(glbAsset->rotate[0], glbAsset->rotate[1], glbAsset->rotate[2]);
            model->eulerAngles = filament::math::float3(glbAsset->eulerAngles[0] / 180.0 * M_PI,
                                                        glbAsset->eulerAngles[1] / 180.0 * M_PI,
                                                        glbAsset->eulerAngles[2] / 180.0 * M_PI);
            if (model->eulerAngles[0] > 1e-5 || model->eulerAngles[1] > 1e-5 ||
                model->eulerAngles[2] > 1e-5) {
                model->rotate = model->eulerAngles;
            }
            model->flip = filament::math::int3(glbAsset->flip[0], glbAsset->flip[1], glbAsset->flip[2]);
            model->rotateType = filament::math::int3(glbAsset->rotateType[0], glbAsset->rotateType[1],
                                                     glbAsset->rotateType[2]);
            model->autoScaleFactor = filament::math::float3(
                    glbAsset->autoScaleFactor[0], glbAsset->autoScaleFactor[1], glbAsset->autoScaleFactor[2]);

            for (int i = 0; i < 2; ++i) {
                model->scaleRange[i] = glbAsset->scaleRange[i];
            }

            LOGD << "eye node list count:" << glbAsset->eyeNodeCount << LOGEND;
            for (size_t index = 0; index < glbAsset->eyeNodeCount; index++) {
              auto &node = glbAsset->eyeNodeList[index];
              if (!node.name) {
                continue;
              }
              TTEyeNode *eyeNode = new TTEyeNode;
              eyeNode->name = node.name;
              eyeNode->eulerAngles = filament::math::float3(node.eulerAngles[0], node.eulerAngles[1], node.eulerAngles[2]);
              model->eyeNodeList.push_back(eyeNode);
            }

            for (size_t nodeIndex = 0; nodeIndex < glbAsset->nodes_count; nodeIndex++) {
                auto &node = glbAsset->nodes[nodeIndex];
                if (!node.name) {
                    continue;
                }

                TTNodeModel *nodeModel = new TTNodeModel;

                if (node.fur.furTexturePath != nullptr) {
                    nodeModel->furParam = parseFur(&node.fur, materialDir);
                }

                nodeModel->name = node.name;
                nodeModel->faceMesh = node.faceMesh;
                nodeModel->blendOrder = node.blendOrder;
                nodeModel->castShadow = node.castShadow;
                nodeModel->receiveShadow = node.receiveShadow;
                nodeModel->enableExpressionConfigRemap = node.enableExpressionConfigRemap;
                parseExpressionOrderList(nodeModel->expressionOrderList, node.expressionOrderList,
                                         node.expressionOrderCount);
                parseExpressionConfigList(nodeModel->expressionConfigList, node.expressionConfigList,
                                          node.expressionConfigCount);
                if (node.faceMesh) {
                    model->needFaceMesh = 1;
                }
                model->nodes.push_back(nodeModel);

                model->triggerNodeName.emplace_back(node.name);
            }
            model->lightParam = parseLight(&glbAsset->light, materialDir);
            model->dynamicLightListParam = parseDynamicLightList(glbAsset->dynamicLightList,
                                                                 glbAsset->dynamicLightCount);
            model->headFeatherParam = parseHeadFeather(&glbAsset->headFeather, materialDir);
            model->boneParams = parseDynamicBoneList(glbAsset->dynamicBoneList,
                                                     glbAsset->dynamicBoneListCount, materialDir, loadGlb);
            model->reflectConfigList = parseReflectConfigList(glbAsset->reflectConfigList,
                                                              glbAsset->reflectConfigCount, materialDir);
            model->skeletonList = parseSkeletonList(glbAsset->skeleton, glbAsset->skeleton_count);

            model->bloomParam = parseBloom(&glbAsset->bloom);
            model->nodeAssociateGenderCount = glbAsset->nodeAssociateGender_count;
            for (size_t nodeIndex = 0; nodeIndex < glbAsset->nodeAssociateGender_count; nodeIndex++) {
                std::string node = glbAsset->nodeAssociateGender[nodeIndex];
                model->nodeAssociateGender.push_back(node);
            }

            for (int j = 0; j < glbAsset->animationsListCount; ++j) {
                auto item = parseAnimation(glbAsset->animationsList[j]);
                model->animationList.push_back(item);
            }

            for (int j = 0; j < glbAsset->faceBindCount; ++j) {
                auto item = parseFaceBind(glbAsset->faceBindList[j]);
                model->faceParams.push_back(item);
            }

            model->customMaterialList = LIGHT3D::parser::parseShader(glbAsset->customMaterials,
                                                                     glbAsset->customMaterialsCount,
                                                                     materialDir);
            for (auto &shader: model->customMaterialList) {
                if (shader->needNormal) {
                    model->needNormal = true;
                    break;
                }
            }
            return model;
        }

        void parseExpressionConfigList(std::vector<SMART_PTR<TTExpressionConfig>> &expConfigs,
                                       fila_expression_config *expConfigList, cgltf_size expConfigCount) {
            for (size_t i = 0; i < expConfigCount; i++) {
                SMART_PTR<TTExpressionConfig> config = MAKE_SMART_PTR<TTExpressionConfig>();
                config->shapeName = expConfigList[i].shapeName ? expConfigList[i].shapeName : "";
                config->controlledName =
                        expConfigList[i].controlledName ? expConfigList[i].controlledName : "";
                config->shapeRange =
                        TTShapeRange(expConfigList[i].shapeRange[0], expConfigList[i].shapeRange[1]);
                config->controlledRange =
                        TTShapeRange(expConfigList[i].controlledRange[0], expConfigList[i].controlledRange[1]);
                expConfigs.emplace_back(config);
            }
        }

        void parseExpressionOrderList(std::unordered_map<std::string, size_t> &expOrderMap,
                                      char **expOrderList, cgltf_size expOrderCount) {
            for (size_t i = 0; i < expOrderCount; i++) {
                expOrderMap.insert(std::make_pair(expOrderList[i], i));
            }
        }

        void parseAOOptions(SMART_PTR<TTFilamentSetupParam> param, TTFilamentInitParam &initParam) {
            json rootParser = json::parse(std::string((const char *) initParam.jsonStr, initParam.jsonLength),
                    nullptr, false, true);
            if (rootParser.find("fxaa") != rootParser.end()) {
                param->fxaa = rootParser["fxaa"];
            }
            if (rootParser.find("msaa") != rootParser.end()) {
                param->msaa = rootParser["msaa"];
            }
            if (rootParser.find("aoType") != rootParser.end()) {
                param->aoType = rootParser["aoType"];
            }

            filament::View::AmbientOcclusionOptions &ssaoOptions = param->ssaoOptions;
            if (rootParser.find("ssaoOptions") != rootParser.end()) {
                json parser = rootParser["ssaoOptions"];
                //LOGE << "hbao json" << hbaoJson << LOGEND;
                if (parser.find("radius") != parser.end()) {
                    ssaoOptions.radius = parser["radius"];
                }
                if (parser.find("bias") != parser.end()) {
                    ssaoOptions.bias = parser["bias"];
                }
                if (parser.find("intensity") != parser.end()) {
                    ssaoOptions.intensity = parser["intensity"];
                }
                if (parser.find("power") != parser.end()) {
                    ssaoOptions.power = parser["power"];
                }
            }

        }

        void parseBloomOptions(const SMART_PTR<TTFilamentSetupParam>& param,
                               fila_data *sourceAsset,
                               const SMART_PTR<TextureManager>& textureManager) {
            if (sourceAsset->bloomOptions != nullptr && sourceAsset->bloomOptions->enabled) {
                if (sourceAsset->bloomOptions->texturePath != nullptr) {
                    param->bloomOptions.dirt = textureManager->loadTexture(
                            sourceAsset->bloomOptions->texturePath, true);
                }
                param->bloomOptions.dirtStrength = sourceAsset->bloomOptions->dirtStrength;
                param->bloomOptions.strength = sourceAsset->bloomOptions->strength;
                param->bloomOptions.resolution = sourceAsset->bloomOptions->resolution;
                param->bloomOptions.anamorphism = sourceAsset->bloomOptions->anamorphism;
                param->bloomOptions.levels = sourceAsset->bloomOptions->levels;
                if (sourceAsset->bloomOptions->blendMode == "INTERPOLATE") {
                    param->bloomOptions.blendMode =
                            filament::View::BloomOptions::BlendMode::INTERPOLATE;
                } else {
                    param->bloomOptions.blendMode =
                            filament::View::BloomOptions::BlendMode::ADD;
                }
                param->bloomOptions.threshold = sourceAsset->bloomOptions->threshold;
                param->bloomOptions.enabled = sourceAsset->bloomOptions->enabled;
            }
        }
    }

LIGHT3D_NAMESPACE_END

#endif  // TNT_MATERIAL_PARSER_H
