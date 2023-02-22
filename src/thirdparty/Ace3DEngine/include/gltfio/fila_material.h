#ifndef FILA_MATERIAL_H_INCLUDED__
#define FILA_MATERIAL_H_INCLUDED__

#include <stddef.h>
#include "fila_custommaterial.h"
#include <gltfio/fila_kapu_material.h>

#endif /* #ifndef FILA_MATERIAL_H_INCLUDED__ */
//#define CGLTF_IMPLEMENTATION
#ifdef CGLTF_IMPLEMENTATION

static void init_fila_data(fila_data *data) {
    //    toneMapping默认=2：即View::ToneMapping::ACES
    //    toneMapping=1时，使用View::ToneMapping::LINEAR
    //    toneMapping=其他值时，使用View::ToneMapping::NONE
    data->toneMapping = 2;
    data->faceMeshShadingModel = 0;
    data->fov = 60;
    data->dualScale = 100;
    data->featureTransformAdjustAlpha = 0;
    data->maxFaceCount = 1;
    data->headScale[0] = 1.0;
    data->headScale[1] = 1.0;
    data->headScale[2] = 1.0;
}

static void init_fila_glb_data(fila_glb *data) {
    data->iblIntensity = 110000;
    data->scaleRange[0] = 0.001;
    data->scaleRange[1] = 1000;
}

static void init_fila_sticker_data(fila_sticker *data) {
    memset(data->translate, 0, sizeof(float) * 3);
    memset(data->rotate, 0, sizeof(float) * 3);
    for (size_t i = 0; i < 3; i++) {
        data->scale[i] = 1;
    }
}

static void init_fila_node(fila_node *data) {
    data->castShadow = true;
    data->receiveShadow = true;
    data->blendOrder = 4; // As the default draw order of filament is 4. Please check `priority` in RenderableManager.h
    data->enableExpressionConfigRemap = true;
}

static void init_fila_fur_data(fila_fur *data) {
    for (size_t i = 0; i < 4; i++) {
        data->shadowColor[i] = 1;
    }
    for (size_t i = 0; i < 2; i++) {
        data->uvOffset[i] = 0;
    }
    for (size_t i = 0; i < 3; i++) {
        data->anisotropyDirection[i] = 0;
    }
    data->anisotropyDirection[0] = 1;
    data->edgeSoftness = data->topEndAlpha = data->roughness = 1;
    data->reflectance = data->anisotropy = 0;
    data->spacing = 0.15;
    data->shadingModel = 0;
}

static void init_fila_skeleton_data(fila_skeleton *data) {
    for (size_t i = 0; i < 3; i++) {
        data->scale[i] = 1;
        data->translate[i] = 0;
    }
}

static void init_fila_light_data(fila_light *data) {
    for (size_t i = 0; i < 3; i++) {
        data->direction[i] = 1;
    }
    for (size_t i = 0; i < 3; i++) {
        data->color[i] = 1;
    }
}

static void init_fila_feather_head_data(fila_head_feather *data) {
    data->offsetX = 1.0;
    data->offsetY = 1.0;
    data->sigma = 7.0;
    for (size_t i = 0; i < 3; i++) {
        data->scale[i] = 1.15;
    }
    data->useSpecificHeadModel = 0;
}

static void init_fila_expression_config(fila_expression_config *data) {
    data->shapeRange[0] = 0;
    data->shapeRange[1] = 1;
}

static cgltf_result fila_parse_json(cgltf_options *options, const uint8_t *json_chunk,
                                    cgltf_size size, fila_data **out_data);

static int fila_parse_json_nodes(cgltf_options *options, jsmntok_t const *tokens, int i,
                                 const uint8_t *json_chunk, fila_glb *out_data);

static int fila_parse_json_skeletons(cgltf_options *options, jsmntok_t const *tokens, int i,
                                     const uint8_t *json_chunk, fila_glb *out_data);

static int fila_parse_json_eye_node_list(cgltf_options *options, jsmntok_t const *tokens, int i,
                                         const uint8_t *json_chunk, fila_glb *out_data);

static int cgltf_parse_json_int_array(jsmntok_t const *tokens, int i, const uint8_t *json_chunk,
                                      int *out_array, int size) {
    CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_ARRAY);
    if (tokens[i].size != size) {
        return CGLTF_ERROR_JSON;
    }
    ++i;
    for (int j = 0; j < size; ++j) {
        CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_PRIMITIVE);
        out_array[j] = cgltf_json_to_int(tokens + i, json_chunk);
        ++i;
    }
    return i;
}

static int fila_parse_json_fur(cgltf_options *options, jsmntok_t const *tokens, int i,
                               const uint8_t *json_chunk, fila_fur *out_fur) {
    CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_OBJECT);

    int size = tokens[i].size;
    ++i;

    for (int j = 0; j < size; ++j) {
        CGLTF_CHECK_KEY(tokens[i]);

        if (cgltf_json_strcmp(tokens + i, json_chunk, "noiseTexturePath") == 0)  //
        {
            i = cgltf_parse_json_string(options, tokens, i + 1, json_chunk,
                                        &out_fur->noiseTexturePath);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "furTexturePath") == 0)  //
        {
            i = cgltf_parse_json_string(options, tokens, i + 1, json_chunk,
                                        &out_fur->furTexturePath);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "shadingModel") == 0) {
            ++i;
            out_fur->shadingModel = cgltf_json_to_int(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "length") == 0) {
            ++i;
            out_fur->length = cgltf_json_to_float(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "spacing") == 0) {
            ++i;
            out_fur->spacing = cgltf_json_to_float(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "gravity") == 0) {
            ++i;
            // note: we can't parse the precise number of elements since type may not have been
            // computed yet
            int min_size = tokens[i].size > 4 ? 4 : tokens[i].size;
            i = cgltf_parse_json_float_array(tokens, i, json_chunk, out_fur->gravity, min_size);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "wind") == 0) {
            ++i;
            // note: we can't parse the precise number of elements since type may not have been
            // computed yet
            int min_size = tokens[i].size > 4 ? 4 : tokens[i].size;
            i = cgltf_parse_json_float_array(tokens, i, json_chunk, out_fur->wind, min_size);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "shadowColor") == 0) {
            ++i;
            // note: we can't parse the precise number of elements since type may not have been
            // computed yet
            int min_size = tokens[i].size > 4 ? 4 : tokens[i].size;
            i = cgltf_parse_json_float_array(tokens, i, json_chunk, out_fur->shadowColor, min_size);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "anisotropyDirection") == 0) {
            ++i;
            // note: we can't parse the precise number of elements since type may not have been
            // computed yet
            int min_size = tokens[i].size > 3 ? 3 : tokens[i].size;
            i = cgltf_parse_json_float_array(tokens, i, json_chunk, out_fur->anisotropyDirection,
                                             min_size);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "uvOffset") == 0) {
            ++i;
            // note: we can't parse the precise number of elements since type may not have been
            // computed yet
            int min_size = tokens[i].size > 4 ? 4 : tokens[i].size;
            i = cgltf_parse_json_float_array(tokens, i, json_chunk, out_fur->uvOffset, min_size);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "edgeSoftness") == 0) {
            ++i;
            out_fur->edgeSoftness = cgltf_json_to_float(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "topEndAlpha") == 0) {
            ++i;
            out_fur->topEndAlpha = cgltf_json_to_float(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "roughness") == 0) {
            ++i;
            out_fur->roughness = cgltf_json_to_float(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "reflectance") == 0) {
            ++i;
            out_fur->reflectance = cgltf_json_to_float(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "anisotropy") == 0) {
            ++i;
            out_fur->anisotropy = cgltf_json_to_float(tokens + i, json_chunk);
            ++i;
        } else {
            i = cgltf_skip_json(tokens, i + 1);
        }

        if (i < 0) {
            return i;
        }
    }

    return i;
}

static int fila_parse_json_light(cgltf_options *options, jsmntok_t const *tokens, int i,
                                 const uint8_t *json_chunk, fila_light *out_light) {
    CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_OBJECT);

    int size = tokens[i].size;
    ++i;

    for (int j = 0; j < size; ++j) {
        CGLTF_CHECK_KEY(tokens[i]);

        if (cgltf_json_strcmp(tokens + i, json_chunk, "intensity") == 0) {
            ++i;
            out_light->intensity = cgltf_json_to_int(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "castShadow") == 0) {
            ++i;
            out_light->castShadow = cgltf_json_to_bool(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "direction") == 0) {
            ++i;
            // note: we can't parse the precise number of elements since type may not have been
            // computed yet
            int min_size = tokens[i].size > 3 ? 3 : tokens[i].size;
            i = cgltf_parse_json_float_array(tokens, i, json_chunk, out_light->direction, min_size);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "color") == 0) {
            ++i;
            // note: we can't parse the precise number of elements since type may not have been
            // computed yet
            int min_size = tokens[i].size > 3 ? 3 : tokens[i].size;
            i = cgltf_parse_json_float_array(tokens, i, json_chunk, out_light->color, min_size);
        } else {
            i = cgltf_skip_json(tokens, i + 1);
        }

        if (i < 0) {
            return i;
        }
    }

    return i;
}

static int fila_parse_json_head_feather(cgltf_options *options, jsmntok_t const *tokens, int i,
                                        const uint8_t *json_chunk,
                                        fila_head_feather *out_head_feather) {
    CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_OBJECT);

    int size = tokens[i].size;
    ++i;

    for (int j = 0; j < size; ++j) {
        CGLTF_CHECK_KEY(tokens[i]);

        if (cgltf_json_strcmp(tokens + i, json_chunk, "offsetX") == 0) {
            ++i;
            out_head_feather->offsetX = cgltf_json_to_float(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "offsetY") == 0) {
            ++i;
            out_head_feather->offsetY = cgltf_json_to_float(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "sigma") == 0) {
            ++i;
            out_head_feather->sigma = cgltf_json_to_float(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "useSpecificHeadModel") == 0) {
            ++i;
            out_head_feather->useSpecificHeadModel = cgltf_json_to_int(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "smallHeadName") == 0) {
            i = cgltf_parse_json_string(options, tokens, i + 1, json_chunk,
                                        &out_head_feather->smallHeadName);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "bigHeadName") == 0) {
            i = cgltf_parse_json_string(options, tokens, i + 1, json_chunk,
                                        &out_head_feather->bigHeadName);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "scale") == 0) {
            ++i;
            // note: we can't parse the precise number of elements since type may not have been
            // computed yet
            int min_size = tokens[i].size > 3 ? 3 : tokens[i].size;
            i = cgltf_parse_json_float_array(tokens, i, json_chunk, out_head_feather->scale,
                                             min_size);
        } else {
            i = cgltf_skip_json(tokens, i + 1);
        }

        if (i < 0) {
            return i;
        }
    }

    return i;
}

static int
fila_parse_json_point_light(cgltf_options *options, jsmntok_t const *tokens, int i, const uint8_t *json_chunk,
                            fila_dynamic_light *out_point_light) {
    CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_OBJECT);

    int size = tokens[i].size;
    ++i;

    for (int j = 0; j < size; ++j) {
        CGLTF_CHECK_KEY(tokens[i]);

        if (cgltf_json_strcmp(tokens + i, json_chunk, "intensity") == 0) {
            ++i;
            out_point_light->intensity = cgltf_json_to_int(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "efficiency") == 0) {
            ++i;
            out_point_light->efficiency = cgltf_json_to_float(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "position") == 0) {
            ++i;
            // note: we can't parse the precise number of elements since type may not have been computed yet
            int min_size = tokens[i].size > 3 ? 3 : tokens[i].size;
            i = cgltf_parse_json_float_array(tokens, i, json_chunk, out_point_light->position, min_size);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "color") == 0) {
            ++i;
            // note: we can't parse the precise number of elements since type may not have been computed yet
            int min_size = tokens[i].size > 3 ? 3 : tokens[i].size;
            i = cgltf_parse_json_float_array(tokens, i, json_chunk, out_point_light->color, min_size);
        } else {
            i = cgltf_skip_json(tokens, i + 1);
        }

        if (i < 0) {
            return i;
        }
    }

    return i;
}

static int
fila_parse_json_point_light_list(cgltf_options *options, jsmntok_t const *tokens, int i, const uint8_t *json_chunk,
                                 fila_glb *out_glb) {
    i = cgltf_parse_json_array(options, tokens, i, json_chunk, sizeof(fila_dynamic_light),
                               (void **) &out_glb->dynamicLightList, &out_glb->dynamicLightCount);
    if (i < 0) {
        return i;
    }
    for (cgltf_size j = 0; j < out_glb->dynamicLightCount; ++j) {
        i = fila_parse_json_point_light(options, tokens, i, json_chunk, &out_glb->dynamicLightList[j]);
        if (i < 0) {
            return i;
        }
    }
    return i;
}

static int fila_parse_json_bloom(cgltf_options *options, jsmntok_t const *tokens, int i,
                                 const uint8_t *json_chunk, fila_bloom *out_data) {
    CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_OBJECT);

    int size = tokens[i].size;
    ++i;

    for (int j = 0; j < size; ++j) {
        CGLTF_CHECK_KEY(tokens[i]);

        if (cgltf_json_strcmp(tokens + i, json_chunk, "blurScale") == 0) {
            ++i;
            out_data->blurScale = cgltf_json_to_float(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "blurWeight") == 0) {
            ++i;
            out_data->blurWeight = cgltf_json_to_float(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "mixAlpha") == 0) {
            ++i;
            out_data->mixAlpha = cgltf_json_to_float(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "mixType") == 0) {
            ++i;
            out_data->mixType = cgltf_json_to_int(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "entityName") == 0) {
            i = cgltf_parse_json_string(options, tokens, i + 1, json_chunk, &out_data->entityName);
        }
    }

    return i;
}

static int fila_parse_json_bone_params(cgltf_options *options, jsmntok_t const *tokens, int i,
                                       const uint8_t *json_chunk, fila_bone_param *out_bone_param) {
    CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_OBJECT);

    int size = tokens[i].size;
    ++i;

    for (int j = 0; j < size; ++j) {
        CGLTF_CHECK_KEY(tokens[i]);

        if (cgltf_json_strcmp(tokens + i, json_chunk, "elasticity") == 0) {
            ++i;
            out_bone_param->elasticity = cgltf_json_to_float(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "dampness") == 0) {
            ++i;
            out_bone_param->dampness = cgltf_json_to_float(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "stiffiness") == 0) {
            ++i;
            out_bone_param->stiffiness = cgltf_json_to_float(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "boneRadius") == 0) {
            ++i;
            out_bone_param->boneRadius = cgltf_json_to_float(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "gravityFactor") == 0) {
            ++i;
            out_bone_param->gravityFactor = cgltf_json_to_float(tokens + i, json_chunk);
            ++i;
        } else {
            i = cgltf_skip_json(tokens, i + 1);
        }

        if (i < 0) {
            return i;
        }
    }

    return i;
}

static int fila_parse_json_bone_params_list(cgltf_options *options, jsmntok_t const *tokens, int i,
                                            const uint8_t *json_chunk,
                                            fila_dynamic_bone *out_dynamic_bone) {
    i = cgltf_parse_json_array(options, tokens, i, json_chunk, sizeof(cgltf_accessor),
                               (void **) &out_dynamic_bone->boneParamsList,
                               &out_dynamic_bone->boneParamsListCount);
    if (i < 0) {
        return i;
    }
    for (cgltf_size j = 0; j < out_dynamic_bone->boneParamsListCount; ++j) {
        i = fila_parse_json_bone_params(options, tokens, i, json_chunk,
                                        &out_dynamic_bone->boneParamsList[j]);
        if (i < 0) {
            return i;
        }
    }
    return i;
}

static int fila_parse_animation_node(cgltf_options *options, jsmntok_t const *tokens, int i,
                                     const uint8_t *json_chunk, fila_animations *out_animation) {
    CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_OBJECT);

    int size = tokens[i].size;
    ++i;

    for (int j = 0; j < size; ++j) {
        CGLTF_CHECK_KEY(tokens[i]);

        if (cgltf_json_strcmp(tokens + i, json_chunk, "name") == 0) {
            i = cgltf_parse_json_string(options, tokens, i + 1, json_chunk, &out_animation->name);
            //            printf("name: %s\n", out_animation->name);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "triggerType") == 0) {
            ++i;
            out_animation->triggerType = cgltf_json_to_int(tokens + i, json_chunk);
            ++i;
            //            printf("triggerType: %i\n", out_animation->triggerType);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "alwaysTriggered") == 0) {
            ++i;
            out_animation->alwaysTriggered = cgltf_json_to_int(tokens + i, json_chunk);
            ++i;
            //            printf("alwaysTriggered: %i\n", out_animation->alwaysTriggered);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "playCount") == 0) {
            ++i;
            out_animation->playCount = cgltf_json_to_int(tokens + i, json_chunk);
            ++i;
            //            printf("playCount: %i\n", out_animation->playCount);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "isDefault") == 0) {
            ++i;
            out_animation->isDefault = cgltf_json_to_int(tokens + i, json_chunk);
            ++i;
            //            printf("isDefault: %i\n", out_animation->playCount);
        } else {
            i = cgltf_skip_json(tokens, i + 1);
        }

        if (i < 0) {
            return i;
        }
    }

    return i;
}

static int fila_parse_json_face_bind(cgltf_options *options, jsmntok_t const *tokens, int i,
                                     const uint8_t *json_chunk, fila_face_bind *out_bind) {
    CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_OBJECT);

    int size = tokens[i].size;
    ++i;

    for (int j = 0; j < size; ++j) {
        CGLTF_CHECK_KEY(tokens[i]);

        if (cgltf_json_strcmp(tokens + i, json_chunk, "name") == 0) {
            i = cgltf_parse_json_string(options, tokens, i + 1, json_chunk,
                                        &out_bind->name);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "faceIndex") == 0) {
            ++i;
            out_bind->faceIndex = cgltf_json_to_int(tokens + i, json_chunk);
            ++i;
        } else {
            i = cgltf_skip_json(tokens, i + 1);
        }

        if (i < 0) {
            return i;
        }
    }

    return i;
}

static int fila_parse_json_face_bind_list(cgltf_options *options, jsmntok_t const *tokens, int i,
                                          const uint8_t *json_chunk, fila_glb *out_glb) {
    i = cgltf_parse_json_array(options, tokens, i, json_chunk, sizeof(fila_face_bind),
                               (void **) &out_glb->faceBindList, &out_glb->faceBindCount);
    if (i < 0) {
        return i;
    }
    for (cgltf_size j = 0; j < out_glb->faceBindCount; ++j) {
        i = fila_parse_json_face_bind(options, tokens, i, json_chunk,
                                      &out_glb->faceBindList[j]);
        if (i < 0) {
            return i;
        }
    }
    return i;
}

static int fila_parse_json_animation_list(cgltf_options *options, jsmntok_t const *tokens, int i,
                                          const uint8_t *json_chunk, fila_glb *out_data) {
    i = cgltf_parse_json_array(options, tokens, i, json_chunk, sizeof(cgltf_accessor),
                               (void **) &out_data->animationsList, &out_data->animationsListCount);
    if (i < 0) {
        return i;
    }

    for (cgltf_size j = 0; j < out_data->animationsListCount; ++j) {
        i = fila_parse_animation_node(options, tokens, i, json_chunk, &out_data->animationsList[j]);
        if (i < 0) {
            return i;
        }
    }
    return i;
}

static int fila_parse_json_bone_collider_params(cgltf_options *options, jsmntok_t const *tokens,
                                                int i, const uint8_t *json_chunk,
                                                fila_bone_collider_param *out_bone_param) {
    CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_OBJECT);

    int size = tokens[i].size;
    ++i;

    for (int j = 0; j < size; ++j) {
        CGLTF_CHECK_KEY(tokens[i]);

        if (cgltf_json_strcmp(tokens + i, json_chunk, "radius") == 0) {
            ++i;
            out_bone_param->radius = cgltf_json_to_float(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "positionX") == 0) {
            ++i;
            out_bone_param->positionX = cgltf_json_to_float(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "positionY") == 0) {
            ++i;
            out_bone_param->positionY = cgltf_json_to_float(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "positionZ") == 0) {
            ++i;
            out_bone_param->positionZ = cgltf_json_to_float(tokens + i, json_chunk);
            ++i;
        } else {
            i = cgltf_skip_json(tokens, i + 1);
        }

        if (i < 0) {
            return i;
        }
    }

    return i;
}

static int fila_parse_json_bone_collider_list(cgltf_options *options, jsmntok_t const *tokens,
                                              int i, const uint8_t *json_chunk,
                                              fila_dynamic_bone *out_dynamic_bone) {
    i = cgltf_parse_json_array(options, tokens, i, json_chunk, sizeof(cgltf_accessor),
                               (void **) &out_dynamic_bone->boneColliderList,
                               &out_dynamic_bone->boneColliderListCount);
    if (i < 0) {
        return i;
    }
    for (cgltf_size j = 0; j < out_dynamic_bone->boneColliderListCount; ++j) {
        i = fila_parse_json_bone_collider_params(options, tokens, i, json_chunk,
                                                 &out_dynamic_bone->boneColliderList[j]);
        if (i < 0) {
            return i;
        }
    }
    return i;
}

static int fila_parse_json_dynamic_bone(cgltf_options *options, jsmntok_t const *tokens, int i,
                                        const uint8_t *json_chunk,
                                        fila_dynamic_bone *out_dynamic_bone) {
    CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_OBJECT);

    int size = tokens[i].size;
    ++i;

    for (int j = 0; j < size; ++j) {
        CGLTF_CHECK_KEY(tokens[i]);  //

        if (cgltf_json_strcmp(tokens + i, json_chunk, "rootName") == 0) {
            i = cgltf_parse_json_string(options, tokens, i + 1, json_chunk,
                                        &out_dynamic_bone->rootName);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "boneCount") == 0) {
            ++i;
            out_dynamic_bone->boneCount = cgltf_json_to_int(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "resetType") == 0) {
            ++i;
            out_dynamic_bone->resetType = cgltf_json_to_int(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "noRotationUpdate") == 0) {
            ++i;
            out_dynamic_bone->noRotationUpdate = cgltf_json_to_int(tokens + i, json_chunk);
            ++i;
        }
            //        else if (cgltf_json_strcmp(tokens+i, json_chunk, "enableJobSystem") == 0)
            //        {
            //            ++i;
            //            out_dynamic_bone->enableJobSystem = cgltf_json_to_int(tokens+i, json_chunk);
            //            ++i;
            //        }
        else if (cgltf_json_strcmp(tokens + i, json_chunk, "updateMode") == 0) {
            ++i;
            out_dynamic_bone->updateMode = cgltf_json_to_int(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "colliderList") == 0) {
            i = fila_parse_json_bone_collider_list(options, tokens, i + 1, json_chunk,
                                                   out_dynamic_bone);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "gravity") == 0) {
            ++i;
            // note: we can't parse the precise number of elements since type may not have been
            // computed yet
            int min_size = tokens[i].size > 3 ? 3 : tokens[i].size;
            i = cgltf_parse_json_float_array(tokens, i, json_chunk, out_dynamic_bone->gravity,
                                             min_size);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "boneParamsList") == 0) {
            i = fila_parse_json_bone_params_list(options, tokens, i + 1, json_chunk,
                                                 out_dynamic_bone);
        } else {
            i = cgltf_skip_json(tokens, i + 1);
        }

        if (i < 0) {
            return i;
        }
    }

    return i;
}

static int fila_parse_json_reflect_config(cgltf_options *options, jsmntok_t const *tokens, int i,
                                          const uint8_t *json_chunk,
                                          fila_reflect_config *out_reflect_config) {
    CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_OBJECT);

    int size = tokens[i].size;
    ++i;

    for (int j = 0; j < size; ++j) {
        CGLTF_CHECK_KEY(tokens[i]);  //

        if (cgltf_json_strcmp(tokens + i, json_chunk, "nodeName") == 0) {
            i = cgltf_parse_json_string(options, tokens, i + 1, json_chunk,
                                        &out_reflect_config->nodeName);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "baseColorImage") == 0) {
            i = cgltf_parse_json_string(options, tokens, i + 1, json_chunk,
                                        &out_reflect_config->baseColorImage);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "reflectImage") == 0) {
            i = cgltf_parse_json_string(options, tokens, i + 1, json_chunk,
                                        &out_reflect_config->reflectImage);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "zNormalScale") == 0) {
            ++i;
            out_reflect_config->zNormalScale = cgltf_json_to_float(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "blendFactor") == 0) {
            ++i;
            out_reflect_config->blendFactor = cgltf_json_to_float(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "modulateAlpha") == 0) {
            ++i;
            out_reflect_config->modulateAlpha = cgltf_json_to_float(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "blendMode") == 0) {
            ++i;
            out_reflect_config->blendMode = cgltf_json_to_int(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "reflectOffset") == 0) {
            ++i;
            // note: we can't parse the precise number of elements since type may not have been
            // computed yet
            int min_size = tokens[i].size > 2 ? 2 : tokens[i].size;
            i = cgltf_parse_json_float_array(tokens, i, json_chunk,
                                             out_reflect_config->reflectOffset, min_size);
        } else {
            i = cgltf_skip_json(tokens, i + 1);
        }

        if (i < 0) {
            return i;
        }
    }

    return i;
}

static int fila_parse_json_reflect_config_list(cgltf_options *options, jsmntok_t const *tokens,
                                               int i, const uint8_t *json_chunk,
                                               fila_glb *out_glb) {
    i = cgltf_parse_json_array(options, tokens, i, json_chunk, sizeof(fila_reflect_config),
                               (void **) &out_glb->reflectConfigList, &out_glb->reflectConfigCount);
    if (i < 0) {
        return i;
    }
    for (cgltf_size j = 0; j < out_glb->reflectConfigCount; ++j) {
        i = fila_parse_json_reflect_config(options, tokens, i, json_chunk,
                                           &out_glb->reflectConfigList[j]);
        if (i < 0) {
            return i;
        }
    }
    return i;
}

static int fila_parse_json_dynamic_bone_list(cgltf_options *options, jsmntok_t const *tokens, int i,
                                             const uint8_t *json_chunk, fila_glb *out_glb) {
    i = cgltf_parse_json_array(options, tokens, i, json_chunk, sizeof(cgltf_accessor),
                               (void **) &out_glb->dynamicBoneList, &out_glb->dynamicBoneListCount);
    if (i < 0) {
        return i;
    }
    for (cgltf_size j = 0; j < out_glb->dynamicBoneListCount; ++j) {
        i = fila_parse_json_dynamic_bone(options, tokens, i, json_chunk,
                                         &out_glb->dynamicBoneList[j]);
        if (i < 0) {
            return i;
        }
    }
    return i;
}

static int fila_parse_json_glb(cgltf_options *options, jsmntok_t const *tokens, int i,
                               const uint8_t *json_chunk, fila_glb *out_glb) {
    CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_OBJECT);

    int size = tokens[i].size;
    ++i;

    init_fila_light_data(&out_glb->light);
    init_fila_feather_head_data(&out_glb->headFeather);

    for (int j = 0; j < size; ++j) {
        CGLTF_CHECK_KEY(tokens[i]);

        if (cgltf_json_strcmp(tokens + i, json_chunk, "path") == 0) {
            i = cgltf_parse_json_string(options, tokens, i + 1, json_chunk, &out_glb->path);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "dualPath") == 0) {
            i = cgltf_parse_json_string(options, tokens, i + 1, json_chunk, &out_glb->dualPath);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "iblPath") == 0) {
            i = cgltf_parse_json_string(options, tokens, i + 1, json_chunk, &out_glb->iblPath);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "faceMeshTextureKey") == 0) {
            i = cgltf_parse_json_string(options, tokens, i + 1, json_chunk,
                                        &out_glb->faceMeshTextureKey);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "useClearIbl") == 0) {
            ++i;
            out_glb->useClearIbl = cgltf_json_to_int(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "furLayerCount") == 0) {
            ++i;
            out_glb->furLayerCount = cgltf_json_to_int(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "faceMeshType") == 0) {
          ++i;
          out_glb->faceMeshType = cgltf_json_to_int(tokens + i, json_chunk);
          ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "transparentLayerCount") == 0) {
            ++i;
            out_glb->transparentLayerCount = cgltf_json_to_int(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "iblIntensity") == 0) {
            ++i;
            out_glb->iblIntensity = cgltf_json_to_int(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "blendShapeAdjustAlpha") == 0) {
            ++i;
            out_glb->blendShapeAdjustAlpha = cgltf_json_to_float(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "iblRotation") == 0) {
            ++i;
            out_glb->iblRotation = cgltf_json_to_float(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "iblDegree") == 0) {
            ++i;
            out_glb->iblDegree = cgltf_json_to_int(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "fixedTime") == 0) {
            ++i;
            out_glb->fixedTime = cgltf_json_to_float(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "positionType") == 0) {
            ++i;
            out_glb->positionType = cgltf_json_to_int(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "scale") == 0) {
            ++i;
            // note: we can't parse the precise number of elements since type may not have been
            // computed yet
            int min_size = tokens[i].size > 3 ? 3 : tokens[i].size;
            i = cgltf_parse_json_float_array(tokens, i, json_chunk, out_glb->scale, min_size);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "translate") == 0) {
            ++i;
            // note: we can't parse the precise number of elements since type may not have been
            // computed yet
            int min_size = tokens[i].size > 3 ? 3 : tokens[i].size;
            i = cgltf_parse_json_float_array(tokens, i, json_chunk, out_glb->translate, min_size);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "staticPosition") == 0) {
            ++i;
            // note: we can't parse the precise number of elements since type may not have been
            // computed yet
            int min_size = tokens[i].size > 3 ? 3 : tokens[i].size;
            i = cgltf_parse_json_float_array(tokens, i, json_chunk, out_glb->staticPosition,
                                             min_size);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "rotate") == 0) {
            ++i;
            // note: we can't parse the precise number of elements since type may not have been
            // computed yet
            int min_size = tokens[i].size > 3 ? 3 : tokens[i].size;
            i = cgltf_parse_json_float_array(tokens, i, json_chunk, out_glb->rotate, min_size);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "eulerAngles") == 0) {
            ++i;
            // note: we can't parse the precise number of elements since type may not have been
            // computed yet
            int min_size = tokens[i].size > 3 ? 3 : tokens[i].size;
            i = cgltf_parse_json_float_array(tokens, i, json_chunk, out_glb->eulerAngles, min_size);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "flip") == 0) {
            ++i;
            // note: we can't parse the precise number of elements since type may not have been
            // computed yet
            int min_size = tokens[i].size > 3 ? 3 : tokens[i].size;
            i = cgltf_parse_json_int_array(tokens, i, json_chunk, out_glb->flip, min_size);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "rotateType") == 0) {
            ++i;
            // note: we can't parse the precise number of elements since type may not have been
            // computed yet
            int min_size = tokens[i].size > 3 ? 3 : tokens[i].size;
            i = cgltf_parse_json_int_array(tokens, i, json_chunk, out_glb->rotateType, min_size);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "autoScaleFactor") == 0) {
            ++i;
            // note: we can't parse the precise number of elements since type may not have been
            // computed yet
            int min_size = tokens[i].size > 3 ? 3 : tokens[i].size;
            i = cgltf_parse_json_float_array(tokens, i, json_chunk, out_glb->autoScaleFactor,
                                             min_size);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "scaleRange") == 0) {
            ++i;
            // note: we can't parse the precise number of elements since type may not have been
            // computed yet
            int min_size = tokens[i].size > 2 ? 2 : tokens[i].size;
            i = cgltf_parse_json_float_array(tokens, i, json_chunk, out_glb->scaleRange,
                                             min_size);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "light") == 0) {
            i = fila_parse_json_light(options, tokens, i + 1, json_chunk, &out_glb->light);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "headFeather") == 0) {
            i = fila_parse_json_head_feather(options, tokens, i + 1, json_chunk,
                                             &out_glb->headFeather);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "dynamicLightList") == 0) {
            i = fila_parse_json_point_light_list(options, tokens, i + 1, json_chunk, out_glb);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "animationList") == 0) {
            i = fila_parse_json_animation_list(options, tokens, i + 1, json_chunk, out_glb);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "faceBindList") == 0) {
            i = fila_parse_json_face_bind_list(options, tokens, i + 1, json_chunk, out_glb);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "dynamicBoneList") == 0) {
            i = fila_parse_json_dynamic_bone_list(options, tokens, i + 1, json_chunk, out_glb);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "reflectConfigList") == 0) {
            i = fila_parse_json_reflect_config_list(options, tokens, i + 1, json_chunk, out_glb);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "nodeList") == 0) {
            i = fila_parse_json_nodes(options, tokens, i + 1, json_chunk, out_glb);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "eyeNodeList") == 0) {
            i = fila_parse_json_eye_node_list(options, tokens, i + 1, json_chunk, out_glb);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "skeleton") == 0) {
            i = fila_parse_json_skeletons(options, tokens, i + 1, json_chunk, out_glb);
        }
            //        else if (cgltf_json_strcmp(tokens+i, json_chunk, "gender") == 0)
            //        {
            //        	++i;
            //            out_glb->gender = cgltf_json_to_int(tokens+i, json_chunk);
            ////            printf("gender: %i\n", out_glb->gender);
            //            ++i;
            //        }
        else if (cgltf_json_strcmp(tokens + i, json_chunk, "nodeAssociateGender") == 0) {
            i = cgltf_parse_json_string_array(options, tokens, i + 1, json_chunk,
                                              &out_glb->nodeAssociateGender,
                                              &out_glb->nodeAssociateGender_count);
            //			printf("nodeAssociateGender: %s\n", out_glb->nodeAssociateGender[0]);
            //			printf("nodeAssociateGender_count: %i\n",
            //out_glb->nodeAssociateGender_count);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "intensityKey") == 0) {
            ++i;
            out_glb->intensitySize = tokens[i].size;
            out_glb->intensityKey = new float[out_glb->intensitySize];
            i = cgltf_parse_json_float_array(tokens, i, json_chunk, out_glb->intensityKey,
                                             out_glb->intensitySize);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "intensityValue") == 0) {
            ++i;
            out_glb->intensitySize =
                    out_glb->intensitySize > tokens[i].size ? tokens[i].size : out_glb->intensitySize;
            out_glb->intensityValue = new float[out_glb->intensitySize];
            i = cgltf_parse_json_float_array(tokens, i, json_chunk, out_glb->intensityValue,
                                             out_glb->intensitySize);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "bloom") == 0) {
            i = fila_parse_json_bloom(options, tokens, i + 1, json_chunk, &out_glb->bloom);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "customMaterials") == 0) {
            i = fila_parse_custom_materials(options, tokens, i + 1, json_chunk, out_glb);
        } else {
            i = cgltf_skip_json(tokens, i + 1);
        }

        if (i < 0) {
            return i;
        }
    }

    return i;
}

static int fila_parse_json_sticker(cgltf_options *options, jsmntok_t const *tokens, int i,
                                   const uint8_t *json_chunk, fila_sticker *out_sticker) {
    CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_OBJECT);

    int size = tokens[i].size;
    ++i;

    for (int j = 0; j < size; ++j) {
        CGLTF_CHECK_KEY(tokens[i]);

        if (cgltf_json_strcmp(tokens + i, json_chunk, "path") == 0) {
            i = cgltf_parse_json_string(options, tokens, i + 1, json_chunk, &out_sticker->path);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "scale") == 0) {
            ++i;
            // note: we can't parse the precise number of elements since type may not have been
            // computed yet
            int min_size = tokens[i].size > 3 ? 3 : tokens[i].size;
            i = cgltf_parse_json_float_array(tokens, i, json_chunk, out_sticker->scale, min_size);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "translate") == 0) {
            ++i;
            // note: we can't parse the precise number of elements since type may not have been
            // computed yet
            int min_size = tokens[i].size > 3 ? 3 : tokens[i].size;
            i = cgltf_parse_json_float_array(tokens, i, json_chunk, out_sticker->translate,
                                             min_size);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "rotate") == 0) {
            ++i;
            // note: we can't parse the precise number of elements since type may not have been
            // computed yet
            int min_size = tokens[i].size > 3 ? 3 : tokens[i].size;
            i = cgltf_parse_json_float_array(tokens, i, json_chunk, out_sticker->rotate, min_size);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "frameDuration") == 0) {
            ++i;
            out_sticker->frameDuration = cgltf_json_to_int(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "frames") == 0) {
            ++i;
            out_sticker->frames = cgltf_json_to_int(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "width") == 0) {
            ++i;
            out_sticker->width = cgltf_json_to_int(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "height") == 0) {
            ++i;
            out_sticker->height = cgltf_json_to_int(tokens + i, json_chunk);
            ++i;
        } else {
            i = cgltf_skip_json(tokens, i + 1);
        }

        if (i < 0) {
            return i;
        }
    }

    return i;
}

cgltf_result fila_parse(const cgltf_options *options, const void *data, cgltf_size size,
                        fila_data **out_data) {
    cgltf_options *fixed_options = const_cast<cgltf_options *>(options);
    if (fixed_options->memory.alloc == NULL) {
        fixed_options->memory.alloc = &cgltf_default_alloc;
    }
    if (fixed_options->memory.free == NULL) {
        fixed_options->memory.free = &cgltf_default_free;
    }

    cgltf_result json_result = fila_parse_json(fixed_options, (const uint8_t *) data, size, out_data);
    return json_result;
}

cgltf_result fila_parse_file(const cgltf_options *options, const char *path, fila_data **out_data) {
    if (options == NULL) {
        return cgltf_result_invalid_options;
    }

    void *(*memory_alloc)(void *, cgltf_size) =
    options->memory.alloc ? options->memory.alloc : &cgltf_default_alloc;
    void (*memory_free)(void *, void *) =
    options->memory.free ? options->memory.free : &cgltf_default_free;

    FILE *file = fopen(path, "rb");
    if (!file) {
        return cgltf_result_file_not_found;
    }

    fseek(file, 0, SEEK_END);

    long length = ftell(file);
    if (length < 0) {
        fclose(file);
        return cgltf_result_io_error;
    }

    fseek(file, 0, SEEK_SET);

    char *file_data = (char *) memory_alloc(options->memory.user_data, length);
    if (!file_data) {
        fclose(file);
        return cgltf_result_out_of_memory;
    }

    cgltf_size file_size = (cgltf_size) length;
    cgltf_size read_size = fread(file_data, 1, file_size, file);

    fclose(file);

    if (read_size != file_size) {
        memory_free(options->memory.user_data, file_data);
        return cgltf_result_io_error;
    }

    cgltf_result result = fila_parse(options, file_data, file_size, out_data);

    if (result != cgltf_result_success) {
        memory_free(options->memory.user_data, file_data);
        return result;
    }

    (*out_data)->file_data = file_data;

    return cgltf_result_success;
}

static int fila_parse_json_glbs(cgltf_options *options, jsmntok_t const *tokens, int i,
                                const uint8_t *json_chunk, fila_data *out_data) {
    //    printf("[OVERFLOW]parse fila_parse_json_glbs,  sizeof(fila_glb)=%d,
    //    sizeof(cgltf_accessor)=%d\n",  sizeof(fila_glb),  sizeof(cgltf_accessor)); [OVERFLOW]parse
    //    fila_parse_json_glbs,  sizeof(fila_glb)=312,  sizeof(cgltf_accessor)=304
    i = cgltf_parse_json_array(options, tokens, i, json_chunk, sizeof(fila_glb),
                               (void **) &out_data->glbs, &out_data->glbs_count);
    //    i = cgltf_parse_json_array(options, tokens, i, json_chunk, sizeof(cgltf_accessor),
    //    (void**)&out_data->glbs, &out_data->glbs_count);
    if (i < 0) {
        return i;
    }

    for (cgltf_size j = 0; j < out_data->glbs_count; ++j) {
        init_fila_glb_data(&out_data->glbs[j]);
        i = fila_parse_json_glb(options, tokens, i, json_chunk, &out_data->glbs[j]);
        if (i < 0) {
            return i;
        }
    }
    return i;
}

static int fila_parse_json_stickers(cgltf_options *options, jsmntok_t const *tokens, int i,
                                    const uint8_t *json_chunk, fila_data *out_data) {
    //    printf("[OVERFLOW]parse fila_parse_json_glbs,  sizeof(fila_glb)=%d,
    //    sizeof(cgltf_accessor)=%d\n",  sizeof(fila_glb),  sizeof(cgltf_accessor)); [OVERFLOW]parse
    //    fila_parse_json_glbs,  sizeof(fila_glb)=312,  sizeof(cgltf_accessor)=304
    i = cgltf_parse_json_array(options, tokens, i, json_chunk, sizeof(fila_sticker),
                               (void **) &out_data->stickers, &out_data->stickers_count);
    //    i = cgltf_parse_json_array(options, tokens, i, json_chunk, sizeof(cgltf_accessor),
    //    (void**)&out_data->glbs, &out_data->glbs_count);
    if (i < 0) {
        return i;
    }

    for (cgltf_size j = 0; j < out_data->stickers_count; ++j) {
        init_fila_sticker_data(&out_data->stickers[j]);
        i = fila_parse_json_sticker(options, tokens, i, json_chunk, &out_data->stickers[j]);
        if (i < 0) {
            return i;
        }
    }
    return i;
}

static int fila_parse_json_skeleton(cgltf_options *options, jsmntok_t const *tokens, int i,
                                    const uint8_t *json_chunk, fila_skeleton *out_skeleton) {
    CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_OBJECT);

    int size = tokens[i].size;
    ++i;

    for (int j = 0; j < size; ++j) {
        CGLTF_CHECK_KEY(tokens[i]);

        if (cgltf_json_strcmp(tokens + i, json_chunk, "name") == 0) {
            i = cgltf_parse_json_string(options, tokens, i + 1, json_chunk, &out_skeleton->name);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "scale") == 0) {
            ++i;
            // note: we can't parse the precise number of elements since type may not have been
            // computed yet
            int min_size = tokens[i].size > 3 ? 3 : tokens[i].size;
            i = cgltf_parse_json_float_array(tokens, i, json_chunk, out_skeleton->scale, min_size);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "position") == 0) {
            ++i;
            // note: we can't parse the precise number of elements since type may not have been
            // computed yet
            int min_size = tokens[i].size > 3 ? 3 : tokens[i].size;
            i = cgltf_parse_json_float_array(tokens, i, json_chunk, out_skeleton->translate,
                                             min_size);
        } else {
            i = cgltf_skip_json(tokens, i + 1);
        }

        if (i < 0) {
            return i;
        }
    }

    return i;
}

static int fila_parse_json_skeletons(cgltf_options *options, jsmntok_t const *tokens, int i,
                                     const uint8_t *json_chunk, fila_glb *out_data) {
    i = cgltf_parse_json_array(options, tokens, i, json_chunk, sizeof(fila_skeleton),
                               (void **) &out_data->skeleton, &out_data->skeleton_count);
    if (i < 0) {
        return i;
    }

    for (cgltf_size j = 0; j < out_data->skeleton_count; ++j) {
        init_fila_skeleton_data(&out_data->skeleton[j]);
        i = fila_parse_json_skeleton(options, tokens, i, json_chunk, &out_data->skeleton[j]);
        if (i < 0) {
            return i;
        }
    }
    return i;
}

static int fila_parse_json_expression_config(cgltf_options *options, jsmntok_t const *tokens, int i,
                                             const uint8_t *json_chunk,
                                             fila_expression_config *out_config) {
    CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_OBJECT);

    int size = tokens[i].size;
    ++i;

    for (int j = 0; j < size; ++j) {
        CGLTF_CHECK_KEY(tokens[i]);  //

        if (cgltf_json_strcmp(tokens + i, json_chunk, "shapeName") == 0) {
            i = cgltf_parse_json_string(options, tokens, i + 1, json_chunk, &out_config->shapeName);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "controlledName") == 0) {
            i = cgltf_parse_json_string(options, tokens, i + 1, json_chunk,
                                        &out_config->controlledName);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "shapeRange") == 0) {
            ++i;
            int min_size = tokens[i].size > 2 ? 2 : tokens[i].size;
            i = cgltf_parse_json_float_array(tokens, i, json_chunk, out_config->shapeRange,
                                             min_size);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "controlledRange") == 0) {
            ++i;
            int min_size = tokens[i].size > 2 ? 2 : tokens[i].size;
            i = cgltf_parse_json_float_array(tokens, i, json_chunk, out_config->controlledRange,
                                             min_size);
        } else {
            i = cgltf_skip_json(tokens, i + 1);
        }

        if (i < 0) {
            return i;
        }
    }

    return i;
}

static int fila_parse_json_expression_configs(cgltf_options *options, jsmntok_t const *tokens,
                                              int i, const uint8_t *json_chunk,
                                              fila_node *out_node) {
    i = cgltf_parse_json_array(options, tokens, i, json_chunk, sizeof(fila_expression_config),
                               (void **) &out_node->expressionConfigList,
                               &out_node->expressionConfigCount);
    if (i < 0) {
        return i;
    }

    for (cgltf_size j = 0; j < out_node->expressionConfigCount; ++j) {
        init_fila_expression_config(&out_node->expressionConfigList[j]);
        i = fila_parse_json_expression_config(options, tokens, i, json_chunk,
                                              &out_node->expressionConfigList[j]);
        if (i < 0) {
            return i;
        }
    }
    return i;
}

static int fila_parse_json_node(cgltf_options *options, jsmntok_t const *tokens, int i,
                                const uint8_t *json_chunk, fila_node *out_node) {
    CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_OBJECT);

    int size = tokens[i].size;
    ++i;

    for (int j = 0; j < size; ++j) {
        CGLTF_CHECK_KEY(tokens[i]);

        if (cgltf_json_strcmp(tokens + i, json_chunk, "name") == 0) {
            i = cgltf_parse_json_string(options, tokens, i + 1, json_chunk, &out_node->name);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "faceMesh") == 0) {
            ++i;
            out_node->faceMesh = cgltf_json_to_int(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "blendOrder") == 0) {
            ++i;
            out_node->blendOrder = cgltf_json_to_int(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "castShadow") == 0) {
            ++i;
            out_node->castShadow = cgltf_json_to_bool(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "receiveShadow") == 0) {
            ++i;
            out_node->receiveShadow = cgltf_json_to_bool(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "enableExpressionConfigRemap") == 0) {
            ++i;
            out_node->enableExpressionConfigRemap = cgltf_json_to_bool(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "fur") == 0) {
            i = fila_parse_json_fur(options, tokens, i + 1, json_chunk, &out_node->fur);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "expressionOrderList") == 0) {
            i = cgltf_parse_json_string_array(options, tokens, i + 1, json_chunk,
                                              &out_node->expressionOrderList,
                                              &out_node->expressionOrderCount);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "expressionConfigList") == 0) {
            i = fila_parse_json_expression_configs(options, tokens, i + 1, json_chunk, out_node);
        } else {
            i = cgltf_skip_json(tokens, i + 1);
        }

        if (i < 0) {
            return i;
        }
    }

    return i;
}

static int fila_parse_json_eye_node(cgltf_options *options, jsmntok_t const *tokens, int i,
                                    const uint8_t *json_chunk, fila_eye_node *out_node) {
    CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_OBJECT);

    int size = tokens[i].size;
    ++i;

    for (int j = 0; j < size; ++j) {
        CGLTF_CHECK_KEY(tokens[i]);

        if (cgltf_json_strcmp(tokens + i, json_chunk, "name") == 0) {
            i = cgltf_parse_json_string(options, tokens, i + 1, json_chunk, &out_node->name);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "eulerAngles") == 0) {
            ++i;
            int min_size = tokens[i].size > 3 ? 3 : tokens[i].size;
            i = cgltf_parse_json_float_array(tokens, i, json_chunk, out_node->eulerAngles,
                                             min_size);
        } else {
            i = cgltf_skip_json(tokens, i + 1);
        }

        if (i < 0) {
            return i;
        }
    }

    return i;
}

static int fila_parse_json_eye_node_list(cgltf_options *options, jsmntok_t const *tokens, int i,
                                         const uint8_t *json_chunk, fila_glb *out_data) {
    i = cgltf_parse_json_array(options, tokens, i, json_chunk, sizeof(cgltf_accessor),
                               (void **) &out_data->eyeNodeList, &out_data->eyeNodeCount);
    if (i < 0) {
        return i;
    }

    for (cgltf_size j = 0; j < out_data->eyeNodeCount; ++j) {
        i = fila_parse_json_eye_node(options, tokens, i, json_chunk, &out_data->eyeNodeList[j]);
        if (i < 0) {
            return i;
        }
    }
    return i;
}

static int fila_parse_json_bloom_options(cgltf_options *options, jsmntok_t const *tokens, int i,
                                         const uint8_t *json_chunk,
                                         fila_data *out_data) {
    cgltf_size bloomOptionsCount = 1;
    i = cgltf_parse_json_array(options, tokens, i, json_chunk, sizeof(fila_BloomOptions),
                               reinterpret_cast<void **>(&out_data->bloomOptions) ,
                               &bloomOptionsCount);
    if (i < 0) {
        return i;
    }

    CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_OBJECT);

    int size = tokens[i].size;
    ++i;

    for (int j = 0; j < size; ++j) {
        CGLTF_CHECK_KEY(tokens[i]);

        if (cgltf_json_strcmp(tokens + i, json_chunk, "dirt") == 0) {
            i = cgltf_parse_json_string(options, tokens, i + 1, json_chunk,
                                        &out_data->bloomOptions->texturePath);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "dirtStrength") == 0) {
            ++i;
            out_data->bloomOptions->dirtStrength = cgltf_json_to_float(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "strength") == 0) {
            ++i;
            out_data->bloomOptions->strength = cgltf_json_to_float(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "resolution") == 0) {
            ++i;
            out_data->bloomOptions->resolution = cgltf_json_to_int(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "anamorphism") == 0) {
            ++i;
            out_data->bloomOptions->anamorphism = cgltf_json_to_float(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "levels") == 0) {
            ++i;
            out_data->bloomOptions->levels = cgltf_json_to_int(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "blendMode") == 0) {
            i = cgltf_parse_json_string(options, tokens, i + 1, json_chunk,
                                        &out_data->bloomOptions->blendMode);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "threshold") == 0) {
            ++i;
            out_data->bloomOptions->threshold = cgltf_json_to_bool(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "enabled") == 0) {
            ++i;
            out_data->bloomOptions->enabled = cgltf_json_to_bool(tokens + i, json_chunk);
            ++i;
        }

        if (i < 0) {
            return i;
        }
    }
    return i;
}

static int fila_parse_json_nodes(cgltf_options *options, jsmntok_t const *tokens, int i,
                                 const uint8_t *json_chunk, fila_glb *out_data) {
    i = cgltf_parse_json_array(options, tokens, i, json_chunk, sizeof(cgltf_accessor),
                               (void **) &out_data->nodes, &out_data->nodes_count);
    if (i < 0) {
        return i;
    }

    for (cgltf_size j = 0; j < out_data->nodes_count; ++j) {
        init_fila_node(&out_data->nodes[j]);
        init_fila_fur_data(&out_data->nodes[j].fur);
        i = fila_parse_json_node(options, tokens, i, json_chunk, &out_data->nodes[j]);
        if (i < 0) {
            return i;
        }
    }
    return i;
}

static int fila_parse_json_root(cgltf_options *options, jsmntok_t const *tokens, int i,
                                const uint8_t *json_chunk, fila_data *out_data) {
    CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_OBJECT);

    int size = tokens[i].size;
    ++i;

    for (int j = 0; j < size; ++j) {
        CGLTF_CHECK_KEY(tokens[i]);

        if (cgltf_json_strcmp(tokens + i, json_chunk, "hideUserHeadModel") == 0) {
            ++i;
            out_data->hideUserHeadModel = cgltf_json_to_int(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "headReceiveShadows") == 0) {
            ++i;
            out_data->headReceiveShadows = cgltf_json_to_bool(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "headScale") == 0) {
            ++i;
            // note: we can't parse the precise number of elements since type may not have been
            // computed yet
            int min_size = tokens[i].size > 3 ? 3 : tokens[i].size;
            i = cgltf_parse_json_float_array(tokens, i, json_chunk, out_data->headScale, min_size);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "fov") == 0) {
            ++i;
            if (cgltf_json_to_int(tokens + i, json_chunk) > 0) {
                out_data->fov = cgltf_json_to_int(tokens + i, json_chunk);
            }
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "dualScale") == 0) {
            ++i;
            out_data->dualScale = cgltf_json_to_float(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "enableBloom") == 0) {
            ++i;
            out_data->enableBloom = cgltf_json_to_int(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "enableJobSystem") == 0) {
            ++i;
            out_data->enableJobSystem = cgltf_json_to_int(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "enableHeadFeather") == 0) {
            ++i;
            out_data->enableHeadFeather = cgltf_json_to_int(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "kapuMaterialType") == 0) {
            ++i;
            out_data->kapuMaterialType = cgltf_json_to_int(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "maxFaceCount") == 0) {
            ++i;
            out_data->maxFaceCount = cgltf_json_to_int(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "toneMapping") == 0) {
            ++i;
            out_data->toneMapping = cgltf_json_to_int(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "viewDepthPrepassDisable") == 0) {
            ++i;
            out_data->viewDepthPrepassDisable = cgltf_json_to_int(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "faceMeshShadingModel") == 0) {
            ++i;
            out_data->faceMeshShadingModel = cgltf_json_to_float(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "featureTransformAdjustAlpha") == 0) {
            ++i;
            out_data->featureTransformAdjustAlpha = cgltf_json_to_float(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "ssao") == 0) {
            ++i;
            out_data->ssao = cgltf_json_to_int(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "glbList") == 0) {
            i = fila_parse_json_glbs(options, tokens, i + 1, json_chunk, out_data);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "stickers") == 0) {
            i = fila_parse_json_stickers(options, tokens, i + 1, json_chunk, out_data);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "bloomOptions") == 0) {
            i = fila_parse_json_bloom_options(options, tokens, i + 1, json_chunk,
                                              out_data);
        } else {
            i = cgltf_skip_json(tokens, i + 1);
        }

        if (i < 0) {
            return i;
        }
    }

    return i;
}

cgltf_result fila_parse_json(cgltf_options *options, const uint8_t *json_chunk, cgltf_size size,
                             fila_data **out_data) {
    jsmn_parser parser = {0, 0, 0};

    if (options->json_token_count == 0) {
        int token_count = jsmn_parse(&parser, (const char *) json_chunk, size, NULL, 0);

        if (token_count <= 0) {
            return cgltf_result_invalid_json;
        }

        options->json_token_count = token_count;
    }

    jsmntok_t *tokens = (jsmntok_t *) options->memory.alloc(
            options->memory.user_data, sizeof(jsmntok_t) * (options->json_token_count + 1));

    if (!tokens) {
        return cgltf_result_out_of_memory;
    }

    jsmn_init(&parser);

    int token_count =
            jsmn_parse(&parser, (const char *) json_chunk, size, tokens, options->json_token_count);

    if (token_count <= 0) {
        options->memory.free(options->memory.user_data, tokens);
        return cgltf_result_invalid_json;
    }

    // this makes sure that we always have an UNDEFINED token at the end of the stream
    // for invalid JSON inputs this makes sure we don't perform out of bound reads of token data
    tokens[token_count].type = JSMN_UNDEFINED;

    fila_data *data =
            (fila_data *) options->memory.alloc(options->memory.user_data, sizeof(fila_data));

    if (!data) {
        options->memory.free(options->memory.user_data, tokens);
        return cgltf_result_out_of_memory;
    }

    memset(data, 0, sizeof(fila_data));
    init_fila_data(data);
    data->memory_free = options->memory.free;
    data->memory_user_data = options->memory.user_data;

    int i = fila_parse_json_root(options, tokens, 0, json_chunk, data);

    options->memory.free(options->memory.user_data, tokens);

    if (i < 0) {
        fila_free(data);
        return (i == CGLTF_ERROR_NOMEM) ? cgltf_result_out_of_memory : cgltf_result_invalid_json;
    }

    *out_data = data;

    return cgltf_result_success;
}

void fila_free(fila_data *data) {
    if (!data) {
        return;
    }

    for (cgltf_size i = 0; i < data->glbs_count; ++i) {
        data->memory_free(data->memory_user_data, data->glbs[i].path);
        data->memory_free(data->memory_user_data, data->glbs[i].iblPath);
        data->memory_free(data->memory_user_data, data->glbs[i].dualPath);
        data->memory_free(data->memory_user_data, data->glbs[i].faceMeshTextureKey);
        data->memory_free(data->memory_user_data, data->glbs[i].intensityKey);
        data->memory_free(data->memory_user_data, data->glbs[i].intensityValue);

        for (cgltf_size j = 0; j < data->glbs[i].dynamicBoneListCount; ++j) {
            data->memory_free(data->memory_user_data, data->glbs[i].dynamicBoneList[j].rootName);
            data->memory_free(data->memory_user_data, data->glbs[i].dynamicBoneList[j].boneParamsList);
            data->memory_free(data->memory_user_data, data->glbs[i].dynamicBoneList[j].boneColliderList);
        }
        data->memory_free(data->memory_user_data, data->glbs[i].dynamicBoneList);

    }

    data->memory_free(data->memory_user_data, data->glbs);

    if (data->file_data) {
        data->memory_free(data->memory_user_data, data->file_data);
    }

    data->memory_free(data->memory_user_data, data);
}

#endif /* #ifdef CGLTF_IMPLEMENTATION */
