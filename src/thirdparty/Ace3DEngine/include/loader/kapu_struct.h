//
// Created by panda on 2020-02-21.
//

#ifndef __KAPU_STRUCT_H__
#define __KAPU_STRUCT_H__

#include "gltfio/fila_material.h"

#include <string>
#include <map>
#include "math/vec3.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef struct kapu_skeleton{
    char* name = nullptr;
    cgltf_float position[3];
    cgltf_float scale[3];
} kapu_skeleton;

typedef struct kapu_hsv{
    char* name = nullptr;
    cgltf_float h;
    cgltf_float s;
    cgltf_float v;
} kapu_hsv;

typedef struct kapu_blendShape{
    char *name = nullptr;
    cgltf_float value;
} kapu_blendShape;

typedef struct kapu_data{
    cgltf_size skeletonSize;
    kapu_skeleton* skeleton;
    cgltf_size hsvDeltaSize;
    kapu_hsv* hsv = nullptr;
    cgltf_size blendShapeSize;
    kapu_blendShape* blendShape;
} kapu_data;

cgltf_result kapu_parse(
        const cgltf_options* options,
        const void* data,
        cgltf_size size,
        kapu_data** out_data);

cgltf_result kapu_parse_file(
        const cgltf_options* options,
        const char* path,
        kapu_data** out_data);

void kapu_free(const cgltf_options* options, kapu_data* data);

#ifdef __cplusplus
}
#endif

#ifdef CGLTF_IMPLEMENTATION

static cgltf_result kapu_parse_json(cgltf_options* options, const uint8_t* json_chunk, cgltf_size size, kapu_data** out_data);

void kapu_free(const cgltf_options* options, kapu_data* data) {
    for (int i = 0; i < data->hsvDeltaSize; i++) {
        options->memory.free(options->memory.user_data, data->hsv[i].name);
    }
    options->memory.free(options->memory.user_data, data->hsv);

    for (int i = 0; i < data->skeletonSize; i++) {
        options->memory.free(options->memory.user_data, data->skeleton[i].name);
    }
    options->memory.free(options->memory.user_data, data->skeleton);

    for (int i = 0; i < data->blendShapeSize; i++) {
        options->memory.free(options->memory.user_data, data->blendShape[i].name);
    }
    options->memory.free(options->memory.user_data, data->blendShape);

    options->memory.free(options->memory.user_data, data);
};

cgltf_result kapu_parse(const cgltf_options* options, const void* data, cgltf_size size, kapu_data** out_data) {
    cgltf_options* fixed_options = const_cast<cgltf_options*>(options);
    if (fixed_options->memory.alloc == NULL) {
        fixed_options->memory.alloc = &cgltf_default_alloc;
    }
    if (fixed_options->memory.free == NULL) {
        fixed_options->memory.free = &cgltf_default_free;
    }

    cgltf_result json_result = kapu_parse_json(fixed_options, (const uint8_t*)data, size, out_data);
    return json_result;
}

cgltf_result kapu_parse_file(const cgltf_options* options, const char* path, kapu_data** out_data) {
    if (options == NULL) {
        return cgltf_result_invalid_options;
    }

    void* (*memory_alloc)(void*, cgltf_size) = options->memory.alloc ? options->memory.alloc : &cgltf_default_alloc;
    void (*memory_free)(void*, void*) = options->memory.free ? options->memory.free : &cgltf_default_free;

    FILE* file = fopen(path, "rb");
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

    char* file_data = (char*)memory_alloc(options->memory.user_data, length);
    if (!file_data) {
        fclose(file);
        return cgltf_result_out_of_memory;
    }

    cgltf_size file_size = (cgltf_size)length;
    cgltf_size read_size = fread(file_data, 1, file_size, file);

    fclose(file);

    if (read_size != file_size) {
        memory_free(options->memory.user_data, file_data);
        return cgltf_result_io_error;
    }

    cgltf_result result = kapu_parse(options, file_data, file_size, out_data);

    if (result != cgltf_result_success) {
        memory_free(options->memory.user_data, file_data);
        return result;
    }

    memory_free(options->memory.user_data, file_data);

    return cgltf_result_success;
}

static int kapu_parse_json_hsv(cgltf_options* options, jsmntok_t const* tokens, int i, const uint8_t* json_chunk, kapu_hsv* out_hsv) {
    CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_OBJECT);

    int size = tokens[i].size;
    ++i;

    for (int j = 0; j < size; ++j) {
        CGLTF_CHECK_KEY(tokens[i]);

        if (cgltf_json_strcmp(tokens + i, json_chunk, "H") == 0) {
            ++i;
            out_hsv->h =
                cgltf_json_to_float(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "S") == 0) {
            ++i;
            out_hsv->s =
                cgltf_json_to_float(tokens + i, json_chunk);
            ++i;
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "V") == 0) {
            ++i;
            out_hsv->v =
                cgltf_json_to_float(tokens + i, json_chunk);
            ++i;
        } else {
            i = cgltf_skip_json(tokens, i + 1);
        }
    }
    return i;
}

static int kapu_parse_json_hsvs(cgltf_options* options, jsmntok_t const* tokens, int i, const uint8_t* json_chunk, kapu_data* out_data)
{
    int size = tokens[i].size;
    ++i;
    out_data->hsv = (kapu_hsv*)cgltf_calloc(options, sizeof(kapu_hsv), size);
    out_data->hsvDeltaSize = size;

    for (int j = 0; j < size; ++j) {
        CGLTF_CHECK_KEY(tokens[i]);
        i = cgltf_parse_json_string(options, tokens, i, json_chunk, &out_data->hsv[j].name);
        i = kapu_parse_json_hsv(options, tokens, i, json_chunk, &out_data->hsv[j]);
        if (i < 0) {
            return i;
        }
    }
    return i;
}

static int kapu_parse_json_blendShapes(cgltf_options* options, jsmntok_t const* tokens, int i, const uint8_t* json_chunk, kapu_data* out_data)
{
    int size = tokens[i].size;
    ++i;
    out_data->blendShape = (kapu_blendShape*)cgltf_calloc(options, sizeof(kapu_blendShape), size);
    out_data->blendShapeSize = size;

    for (int j = 0; j < size; ++j) {
        CGLTF_CHECK_KEY(tokens[i]);
        i = cgltf_parse_json_string(options, tokens, i, json_chunk, &out_data->blendShape[j].name);
        out_data->blendShape[j].value = cgltf_json_to_float(tokens + i, json_chunk);
        ++i;
    }
    return i;
}

static int kapu_parse_json_skeleton(cgltf_options* options, jsmntok_t const* tokens, int i, const uint8_t* json_chunk, kapu_skeleton* out_skeleton) {
    CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_OBJECT);

    int size = tokens[i].size;
    ++i;

    for (int j = 0; j < size; ++j) {
        CGLTF_CHECK_KEY(tokens[i]);

        if (cgltf_json_strcmp(tokens + i, json_chunk, "name") == 0) {
            i = cgltf_parse_json_string(options, tokens, i + 1, json_chunk, &out_skeleton->name);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "scale") == 0) {
            ++i;
            // note: we can't parse the precise number of elements since type may not have been computed yet
            int min_size = tokens[i].size > 3 ? 3 : tokens[i].size;
            i = cgltf_parse_json_float_array(tokens, i, json_chunk, out_skeleton->scale, min_size);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "position") == 0) {
            ++i;
            // note: we can't parse the precise number of elements since type may not have been computed yet
            int min_size = tokens[i].size > 3 ? 3 : tokens[i].size;
            i = cgltf_parse_json_float_array(tokens, i, json_chunk, out_skeleton->position, min_size);
        } else {
            i = cgltf_skip_json(tokens, i + 1);
        }
    }
    return i;
}

static int kapu_parse_json_skeletons(cgltf_options* options, jsmntok_t const* tokens, int i, const uint8_t* json_chunk, kapu_data* out_data) {
    i = cgltf_parse_json_array(options, tokens, i, json_chunk, sizeof(kapu_skeleton), (void**)&out_data->skeleton, &out_data->skeletonSize);
    if (i < 0) {
        return i;
    }

    for (cgltf_size j = 0; j < out_data->skeletonSize; ++j) {
        i = kapu_parse_json_skeleton(options, tokens, i, json_chunk, &out_data->skeleton[j]);
        if (i < 0) {
            return i;
        }
    }
    return i;
}

static int kapu_parse_json_root(cgltf_options* options, jsmntok_t const* tokens, int i, const uint8_t* json_chunk, kapu_data* out_data) {
    CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_OBJECT);

    int size = tokens[i].size;
    ++i;

    for (int j = 0; j < size; ++j) {
        CGLTF_CHECK_KEY(tokens[i]);

        if (cgltf_json_strcmp(tokens + i, json_chunk, "skeleton") == 0) {
            i = kapu_parse_json_skeletons(options, tokens, i + 1, json_chunk, out_data);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "blendshape") == 0) {
            i = kapu_parse_json_blendShapes(options, tokens, i + 1, json_chunk, out_data);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "deltaHSV") == 0) {
            i = kapu_parse_json_hsvs(options, tokens, i + 1, json_chunk, out_data);
        } else {
            i = cgltf_skip_json(tokens, i + 1);
        }
        if (i < 0) {
            return i;
        }
    }
    return i;
}

cgltf_result kapu_parse_json(cgltf_options* options, const uint8_t* json_chunk, cgltf_size size, kapu_data** out_data) {
    jsmn_parser parser = { 0, 0, 0 };

    if (options->json_token_count == 0) {
        int token_count = jsmn_parse(&parser, (const char*)json_chunk, size, NULL, 0);

        if (token_count <= 0) {
            return cgltf_result_invalid_json;
        }

        options->json_token_count = token_count;
    }

    jsmntok_t* tokens = (jsmntok_t*)options->memory.alloc(options->memory.user_data, sizeof(jsmntok_t) * (options->json_token_count + 1));

    if (!tokens) {
        return cgltf_result_out_of_memory;
    }

    jsmn_init(&parser);

    int token_count = jsmn_parse(&parser, (const char*)json_chunk, size, tokens, options->json_token_count);

    if (token_count <= 0) {
        options->memory.free(options->memory.user_data, tokens);
        return cgltf_result_invalid_json;
    }

    // this makes sure that we always have an UNDEFINED token at the end of the stream
    // for invalid JSON inputs this makes sure we don't perform out of bound reads of token data
    tokens[token_count].type = JSMN_UNDEFINED;

    kapu_data* data = (kapu_data*)options->memory.alloc(options->memory.user_data, sizeof(kapu_data));

    if (!data) {
        options->memory.free(options->memory.user_data, tokens);
        return cgltf_result_out_of_memory;
    }

    memset(data, 0, sizeof(kapu_data));

    int i = kapu_parse_json_root(options, tokens, 0, json_chunk, data);

    options->memory.free(options->memory.user_data, tokens);

    if (i < 0) {
        kapu_free(options, data);
        return (i == CGLTF_ERROR_NOMEM) ? cgltf_result_out_of_memory : cgltf_result_invalid_json;
    }

    *out_data = data;

    return cgltf_result_success;
}

#endif /* #ifdef CGLTF_IMPLEMENTATION */

#endif //TNT_KAPU_STRUCT_H