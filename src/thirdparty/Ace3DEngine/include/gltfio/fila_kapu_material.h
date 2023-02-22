//
// Created by kevinxing on 2020/9/7.
//

#ifndef TNT_FILA_KAPU_MATERIAL_H
#define TNT_FILA_KAPU_MATERIAL_H

#include <gltfio/fila_struct.h>

typedef struct fila_kapu_texture {
    char** names;
    cgltf_size names_count;
    char* prop;

} fila_kapu_texture;

typedef struct fila_kapu_single_material {
    char* name;
    char* type;

    fila_kapu_texture* textures;
    cgltf_size textures_count;

    char* _EmissionColor;
    cgltf_float _Metallic;
    cgltf_float _Specular;
    cgltf_float _SpecularIntensity;
    cgltf_float _Roughness;
    cgltf_float _Glossiness;
    cgltf_float _CustomSpecularGlossiness;

    cgltf_float _Anisotropic;
    cgltf_float _AnisotropyDirection;

    cgltf_float _AOIntensity;
    cgltf_float _Hue;
    cgltf_float _Saturate;
    cgltf_float _Value;

    cgltf_float _Colorize;
    cgltf_float _DeltaH;
    cgltf_float _DeltaS;
    cgltf_float _DeltaV;
    cgltf_float _AlphaCutoff;

    cgltf_bool _NORMALMAP;
    cgltf_bool _MRSST_MAP;
    cgltf_bool _SKIN_MASK_MAP;
    cgltf_bool _ALPHAPREMULTIPLY_ON;

    cgltf_int _Mode;
    cgltf_int _BlendMode;

} fila_kapu_single_material;

typedef struct fila_kapu_material {
    fila_kapu_single_material* materials;
    cgltf_size materials_count;

    void (*memory_free)(void* user, void* ptr);
    void* memory_user_data;
} fila_kapu_material;

cgltf_result fila_kapu_material_parse(const cgltf_options* options, const void* data, cgltf_size size,
                                      fila_kapu_material** out_data);
void fila_kapu_material_free(fila_kapu_material* data);

#endif //TNT_FILA_KAPU_MATERIAL_H

//#define CGLTF_IMPLEMENTATION
#ifdef CGLTF_IMPLEMENTATION
static void init_fila_kapu_single_material(fila_kapu_single_material* data) {
    data->_Metallic = data->_Roughness = data->_Specular = data->_Anisotropic = data->_AnisotropyDirection =
            data->_Colorize = 0;
    data->_MRSST_MAP = data->_NORMALMAP = data->_SKIN_MASK_MAP = false;
    data->_ALPHAPREMULTIPLY_ON = true;
    data->_Mode = data->_BlendMode = 0;
    data->_AlphaCutoff = 0.5;
    data->_AOIntensity = data->_SpecularIntensity = 1.0;
    data->_Hue = data->_Saturate = data->_DeltaH = data->_DeltaS = data->_DeltaV = 0;
    data->_Value = 100;
}

static int fila_parse_json_kapu_material_texture(cgltf_options* options, jsmntok_t const* tokens, int i,
                                                const uint8_t* json_chunk, fila_kapu_texture * out_glb) {
    CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_OBJECT);

    int size = tokens[i].size;
    ++i;

    for (int j = 0; j < size; ++j) {
        CGLTF_CHECK_KEY(tokens[i]);

        if (cgltf_json_strcmp(tokens + i, json_chunk, "names") == 0) {
            i = cgltf_parse_json_string_array(options, tokens, i + 1, json_chunk,
                                              &out_glb->names,
                                              &out_glb->names_count);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "prop") == 0) {
            i = cgltf_parse_json_string(options, tokens, i + 1, json_chunk, &out_glb->prop);
        } else {
            i = cgltf_skip_json(tokens, i + 1);
        }

        if (i < 0) {
            return i;
        }
    }

    return i;
}

static int fila_parse_json_kapu_material_textures(cgltf_options* options, jsmntok_t const* tokens, int i,
                                                 const uint8_t* json_chunk, fila_kapu_single_material* out_data) {

    i = cgltf_parse_json_array(options, tokens, i, json_chunk, sizeof(fila_kapu_texture),
                               (void**)&out_data->textures, &out_data->textures_count);

    if (i < 0) {
        return i;
    }

    for (cgltf_size j = 0; j < out_data->textures_count; ++j) {
        i = fila_parse_json_kapu_material_texture(options, tokens, i, json_chunk, &out_data->textures[j]);
        if (i < 0) {
            return i;
        }
    }
    return i;
}

static int fila_parse_json_kapu_single_material(cgltf_options* options, jsmntok_t const* tokens, int i,
                               const uint8_t* json_chunk, fila_kapu_single_material* out_glb) {
    CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_OBJECT);

    int size = tokens[i].size;
    ++i;

    for (int j = 0; j < size; ++j) {
        CGLTF_CHECK_KEY(tokens[i]);

        if (cgltf_json_strcmp(tokens + i, json_chunk, "name") == 0) {
            i = cgltf_parse_json_string(options, tokens, i + 1, json_chunk, &out_glb->name);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "type") == 0) {
            i = cgltf_parse_json_string(options, tokens, i + 1, json_chunk, &out_glb->type);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "_EmissionColor") == 0) {
            i = cgltf_parse_json_string(options, tokens, i + 1, json_chunk, &out_glb->_EmissionColor);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "_Metallic") == 0) {
            ++i;
            out_glb->_Metallic = cgltf_json_to_float(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "_Roughness") == 0) {
            ++i;
            out_glb->_Roughness = cgltf_json_to_float(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "_Glossiness") == 0) {
            ++i;
            out_glb->_Glossiness = cgltf_json_to_float(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "_CustomSpecularGlossiness") == 0) {
            ++i;
            out_glb->_CustomSpecularGlossiness = cgltf_json_to_float(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "_Specular") == 0) {
            ++i;
            out_glb->_Specular = cgltf_json_to_float(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "_SpecularIntensity") == 0) {
            ++i;
            out_glb->_SpecularIntensity = cgltf_json_to_float(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "_Anisotropic") == 0) {
            ++i;
            out_glb->_Anisotropic = cgltf_json_to_float(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "_AnisotropyDirection") == 0) {
            ++i;
            out_glb->_AnisotropyDirection = cgltf_json_to_float(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "_AOIntensity") == 0) {
            ++i;
            out_glb->_AOIntensity = cgltf_json_to_float(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "_Hue") == 0) {
            ++i;
            out_glb->_Hue = cgltf_json_to_float(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "_Saturate") == 0) {
            ++i;
            out_glb->_Saturate = cgltf_json_to_float(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "_Value") == 0) {
            ++i;
            out_glb->_Value = cgltf_json_to_float(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "_deltaH") == 0) {
            ++i;
            out_glb->_DeltaH = cgltf_json_to_float(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "_deltaS") == 0) {
            ++i;
            out_glb->_DeltaS = cgltf_json_to_float(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "_deltaV") == 0) {
            ++i;
            out_glb->_DeltaV = cgltf_json_to_float(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "_AlphaCutoff") == 0) {
            ++i;
            out_glb->_AlphaCutoff = cgltf_json_to_float(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "_ColorizeMode") == 0) {
            ++i;
            out_glb->_Colorize = cgltf_json_to_float(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "_NORMALMAP") == 0) {
            ++i;
            out_glb->_NORMALMAP = cgltf_json_to_bool(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "_MRSST_MAP") == 0) {
            ++i;
            out_glb->_MRSST_MAP = cgltf_json_to_bool(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "_SKIN_MASK_MAP") == 0) {
            ++i;
            out_glb->_SKIN_MASK_MAP = cgltf_json_to_bool(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "_ALPHAPREMULTIPLY_ON") == 0) {
            ++i;
            out_glb->_ALPHAPREMULTIPLY_ON = cgltf_json_to_bool(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "_Mode") == 0) {
            ++i;
            out_glb->_Mode = cgltf_json_to_int(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "_BlendMode") == 0) {
            ++i;
            out_glb->_BlendMode = cgltf_json_to_int(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "textures") == 0) {
            i = fila_parse_json_kapu_material_textures(options, tokens, i + 1, json_chunk, out_glb);
        } else {
            i = cgltf_skip_json(tokens, i + 1);
        }

        if (i < 0) {
            return i;
        }
    }

    return i;
}

static int fila_parse_json_kapu_single_materials(cgltf_options* options, jsmntok_t const* tokens, int i,
                                const uint8_t* json_chunk, fila_kapu_material* out_data) {

    i = cgltf_parse_json_array(options, tokens, i, json_chunk, sizeof(fila_kapu_single_material),
                               (void**)&out_data->materials, &out_data->materials_count);

    if (i < 0) {
        return i;
    }

    for (cgltf_size j = 0; j < out_data->materials_count; ++j) {
        init_fila_kapu_single_material(&out_data->materials[j]);
        i = fila_parse_json_kapu_single_material(options, tokens, i, json_chunk, &out_data->materials[j]);
        if (i < 0) {
            return i;
        }
    }
    return i;
}

static int fila_parse_kapu_material_json_root(cgltf_options* options, jsmntok_t const* tokens, int i,
                                const uint8_t* json_chunk, fila_kapu_material* out_data) {
    i = fila_parse_json_kapu_single_materials(options, tokens, i + 1, json_chunk, out_data);
    return i;
}

cgltf_result fila_parse_kapu_material_json(cgltf_options* options, const uint8_t* json_chunk, cgltf_size size,
                                           fila_kapu_material** out_data) {
    jsmn_parser parser = {0, 0, 0};

    if (options->json_token_count == 0) {
        int token_count = jsmn_parse(&parser, (const char*)json_chunk, size, NULL, 0);

        if (token_count <= 0) {
            return cgltf_result_invalid_json;
        }

        options->json_token_count = token_count;
    }

    jsmntok_t* tokens = (jsmntok_t*)options->memory.alloc(
            options->memory.user_data, sizeof(jsmntok_t) * (options->json_token_count + 1));

    if (!tokens) {
        return cgltf_result_out_of_memory;
    }

    jsmn_init(&parser);

    int token_count =
            jsmn_parse(&parser, (const char*)json_chunk, size, tokens, options->json_token_count);

    if (token_count <= 0) {
        options->memory.free(options->memory.user_data, tokens);
        return cgltf_result_invalid_json;
    }

    // this makes sure that we always have an UNDEFINED token at the end of the stream
    // for invalid JSON inputs this makes sure we don't perform out of bound reads of token data
    tokens[token_count].type = JSMN_UNDEFINED;

    fila_kapu_material* data =
            (fila_kapu_material*)options->memory.alloc(options->memory.user_data, sizeof(fila_kapu_material));

    if (!data) {
        options->memory.free(options->memory.user_data, tokens);
        return cgltf_result_out_of_memory;
    }

    memset(data, 0, sizeof(fila_kapu_material));
    data->memory_free = options->memory.free;
    data->memory_user_data = options->memory.user_data;

    int i = fila_parse_kapu_material_json_root(options, tokens, 0, json_chunk, data);

    options->memory.free(options->memory.user_data, tokens);

    if (i < 0) {
        fila_kapu_material_free(data);
        return (i == CGLTF_ERROR_NOMEM) ? cgltf_result_out_of_memory : cgltf_result_invalid_json;
    }

    *out_data = data;

    return cgltf_result_success;
}

cgltf_result fila_kapu_material_parse(const cgltf_options* options, const void* data, cgltf_size size,
                                      fila_kapu_material** out_data) {
    cgltf_options* fixed_options = const_cast<cgltf_options*>(options);
    if (fixed_options->memory.alloc == NULL) {
        fixed_options->memory.alloc = &cgltf_default_alloc;
    }
    if (fixed_options->memory.free == NULL) {
        fixed_options->memory.free = &cgltf_default_free;
    }

    cgltf_result json_result = fila_parse_kapu_material_json(fixed_options, (const uint8_t*)data, size, out_data);
    return json_result;
}

void fila_kapu_material_free(fila_kapu_material* data) {
    if (!data) {
        return;
    }

    if (data->memory_user_data == NULL) return;


    for (cgltf_size i = 0; i < data->materials_count; ++i) {
        cgltf_default_free(data->memory_user_data, data->materials[i].name);
        cgltf_default_free(data->memory_user_data, data->materials[i].type);
        cgltf_default_free(data->memory_user_data, data->materials[i]._EmissionColor);
        for (cgltf_size j = 0; j < data->materials[i].textures_count; j++) {
            fila_kapu_texture& texture = data->materials[i].textures[j];
            cgltf_default_free(data->memory_user_data, texture.prop);
            for(cgltf_size k = 0; k < texture.names_count; k++) {
                delete[] texture.names[k];
                texture.names[k] = nullptr;
            }
        }
        cgltf_default_free(data->memory_user_data, data->materials[i].textures);
    }

    cgltf_default_free(data->memory_user_data, data->materials);

    cgltf_default_free(data->memory_user_data, data);
}

#endif /* #ifdef CGLTF_IMPLEMENTATION */
