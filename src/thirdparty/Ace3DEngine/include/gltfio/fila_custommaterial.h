//
// Created by panda on 2020-02-21.
//

#ifndef TNT_FILA_CUSTOMMATERIAL_H
#define TNT_FILA_CUSTOMMATERIAL_H

//#define CGLTF_IMPLEMENTATION

#include <gltfio/fila_struct.h>

#ifdef CGLTF_IMPLEMENTATION
static void init_fila_custom_params(fila_custom_params* out_param) {
    out_param->sRGBA = true;
    out_param->valueRange[0] = 0;
    out_param->valueRange[1] = 1;
}
static int fila_parse_custom_materials(cgltf_options *options,
                                       jsmntok_t const *tokens,
                                       int i,
                                       const uint8_t *json_chunk,
                                       fila_glb *out_glb);
cgltf_result fila_parse_value_config_file(cgltf_options* options, const char* path, fila_custom_param_sampler** out_data);
static void fila_custom_free(fila_custom* custom_data);
static void fila_custom_param_sampler_free(fila_custom_param_sampler* custom_data);

void init_fila_custom_param_sampler_data(fila_custom_param_sampler* sampler) {
}

static int fila_parse_custom_param(cgltf_options *options, jsmntok_t const *tokens, int i, const uint8_t *json_chunk, fila_custom_params *out_param)
{
    CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_OBJECT);

    int size = tokens[i].size;
    ++i;

    for (int j = 0; j < size; ++j)
    {
        CGLTF_CHECK_KEY(tokens[i]);

        if (cgltf_json_strcmp(tokens+i, json_chunk, "type") == 0)
        {
            i = cgltf_parse_json_string(options, tokens, i + 1, json_chunk, &out_param->type);
        }
        else if (cgltf_json_strcmp(tokens + i, json_chunk, "value") == 0)
        {
            i = cgltf_parse_json_string_array(options, tokens, i + 1, json_chunk, &out_param->values, &out_param->valuesCount);
        }
        else if (cgltf_json_strcmp(tokens+i, json_chunk, "uniformName") == 0)
        {
            i = cgltf_parse_json_string(options, tokens, i + 1, json_chunk, &out_param->uniformName);
        }
        else if (cgltf_json_strcmp(tokens+i, json_chunk, "valueType") == 0)
        {
            i = cgltf_parse_json_string(options, tokens, i + 1, json_chunk, &out_param->valueType);
        }
        else if (cgltf_json_strcmp(tokens+i, json_chunk, "samplerWrapMode") == 0)
        {
            i = cgltf_parse_json_string(options, tokens, i + 1, json_chunk, &out_param->samplerWrapMode);
        }
        else if (cgltf_json_strcmp(tokens+i, json_chunk, "samplerMinFilter") == 0)
        {
            i = cgltf_parse_json_string(options, tokens, i + 1, json_chunk, &out_param->samplerMinFilter);
        }
        else if (cgltf_json_strcmp(tokens+i, json_chunk, "valueSamplerConfigFile") == 0)
        {
            i = cgltf_parse_json_string(options, tokens, i + 1, json_chunk, &out_param->valueSamplerConfigFile);
            fila_custom_param_sampler* sampler = nullptr;
            fila_options* filaOptions = (fila_options*)(options);
            fila_options newOptions{};
            newOptions.dataPath = filaOptions->dataPath;
            newOptions.type = cgltf_file_type_invalid;
            std::string copiedDataPath;
            copiedDataPath.assign(filaOptions->dataPath);
            cgltf_result result = fila_parse_value_config_file(&newOptions, copiedDataPath.append("/").append(out_param->valueSamplerConfigFile).c_str(), &sampler);
            out_param->valueSampler = sampler;
        }
        else if (cgltf_json_strcmp(tokens + i, json_chunk, "valueRange") == 0)
        {
            ++i;
            // note: we can't parse the precise number of elements since type may not have been computed yet
            int min_size = tokens[i].size > 2 ? 2 : tokens[i].size;
            i = cgltf_parse_json_float_array(tokens, i, json_chunk, out_param->valueRange,
                                             min_size);
        } else if (cgltf_json_strcmp(tokens + i, json_chunk, "sRGBA") == 0) {
            ++i;
            out_param->sRGBA = cgltf_json_to_bool(tokens + i, json_chunk);
            ++i;
        } else {
            i = cgltf_skip_json(tokens, i + 1);
        }

        if (i < 0)
        {
            return i;
        }
    }

    return i;
}

static int fila_parse_custom_params(cgltf_options *options, jsmntok_t const *tokens, int i, const uint8_t *json_chunk, fila_custom *out_custom)
{
    i = cgltf_parse_json_array(options, tokens, i, json_chunk, sizeof(cgltf_accessor), (void**)&out_custom->customParamsList, &out_custom->customParamsListCount);
    if (i < 0)
    {
        return i;
    }
    for (cgltf_size j = 0; j < out_custom->customParamsListCount; ++j)
    {
        init_fila_custom_params(&out_custom->customParamsList[j]);
        i = fila_parse_custom_param(options, tokens, i, json_chunk, &out_custom->customParamsList[j]);
        if (i < 0)
        {
            return i;
        }
    }
    return i;
}

static int fila_parse_custom_material(cgltf_options* options, jsmntok_t const* tokens, int i, const uint8_t* json_chunk, fila_custom* out_custom)
{
    CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_OBJECT);

    int size = tokens[i].size;
    ++i;

    for (int j = 0; j < size; ++j)
    {
        CGLTF_CHECK_KEY(tokens[i]);

        if (cgltf_json_strcmp(tokens+i, json_chunk, "matName") == 0)
        {
            i = cgltf_parse_json_string(options, tokens, i + 1, json_chunk, &out_custom->matName);
        }
        else if (cgltf_json_strcmp(tokens+i, json_chunk, "assetType") == 0)
        {
            i = cgltf_parse_json_string(options, tokens, i + 1, json_chunk, &out_custom->assetType);
        }
        else if (cgltf_json_strcmp(tokens + i, json_chunk, "nodeNameList") == 0)
        {
            i = cgltf_parse_json_string_array(options, tokens, i + 1, json_chunk, &out_custom->nodeNameList, &out_custom->nodeNameListCount);
        }
        else if (cgltf_json_strcmp(tokens + i, json_chunk, "params") == 0)
        {
            i = fila_parse_custom_params(options, tokens, i + 1, json_chunk, out_custom);
        }
        else
        {
            i = cgltf_skip_json(tokens, i+1);
        }

        if (i < 0)
        {
            return i;
        }
    }

    return i;
}

static int fila_parse_custom_materials(cgltf_options *options, jsmntok_t const *tokens, int i, const uint8_t *json_chunk, fila_glb *out_glb)
{
    i = cgltf_parse_json_array(options, tokens, i, json_chunk, sizeof(cgltf_accessor), (void**)&out_glb->customMaterials, &out_glb->customMaterialsCount);
    if (i < 0)
    {
        return i;
    }
    for (cgltf_size j = 0; j < out_glb->customMaterialsCount; ++j)
    {
        i = fila_parse_custom_material(options, tokens, i, json_chunk, &out_glb->customMaterials[j]);
        if (i < 0)
        {
            return i;
        }
    }
    return i;
}

static int fila_parse_custom_param_sampler_json_root(cgltf_options *options, jsmntok_t const *tokens, int i, const uint8_t *json_chunk, fila_custom_param_sampler *out_param)
{
    CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_OBJECT);

    int size = tokens[i].size;
    ++i;

    for (int j = 0; j < size; ++j)
    {
        CGLTF_CHECK_KEY(tokens[i]);

        if (cgltf_json_strcmp(tokens+i, json_chunk, "interpolation") == 0)
        {
            ++i;
            if (cgltf_json_strcmp(tokens + i, json_chunk, "LINEAR") == 0)
            {
                out_param->interpolation = cgltf_interpolation_type_linear;
            }
            else if (cgltf_json_strcmp(tokens + i, json_chunk, "STEP") == 0)
            {
                out_param->interpolation = cgltf_interpolation_type_step;
            }
            else if (cgltf_json_strcmp(tokens + i, json_chunk, "CUBICSPLINE") == 0)
            {
                out_param->interpolation = cgltf_interpolation_type_cubic_spline;
            }
            ++i;
        }
        else if (cgltf_json_strcmp(tokens+i, json_chunk, "timelines") == 0)
        {
            ++i;
            out_param->timelinesCount = tokens[i].size;
            out_param->timelines = new float[out_param->timelinesCount];
            i = cgltf_parse_json_float_array(tokens, i, json_chunk, out_param->timelines, out_param->timelinesCount);
        }
        else if (cgltf_json_strcmp(tokens+i, json_chunk, "values") == 0)
        {
            ++i;
            out_param->valuesCount = tokens[i].size;
            out_param->values = new float[out_param->valuesCount];
            i = cgltf_parse_json_float_array(tokens, i, json_chunk, out_param->values, out_param->valuesCount);
        }
        else if (cgltf_json_strcmp(tokens+i, json_chunk, "stringValues") == 0)
        {
            i = cgltf_parse_json_string_array(options, tokens, i + 1, json_chunk, &out_param->stringValues, &out_param->valuesCount);
        }
        else
        {
            i = cgltf_skip_json(tokens, i+1);
        }

        if (i < 0)
        {
            return i;
        }
    }

    return i;
}

cgltf_result fila_parse_value_sampler(cgltf_options* options, const void* json_chunk, cgltf_size size, fila_custom_param_sampler** out_data){
    jsmn_parser parser = { 0, 0, 0 };
    void* (*memory_alloc)(void*, cgltf_size) = options->memory.alloc ? options->memory.alloc : &cgltf_default_alloc;
    void (*memory_free)(void*, void*) = options->memory.free ? options->memory.free : &cgltf_default_free;

    if (options->json_token_count == 0)
    {
        int token_count = jsmn_parse(&parser, (const char*)json_chunk, size, NULL, 0);

        if (token_count <= 0)
        {
            return cgltf_result_invalid_json;
        }

        options->json_token_count = token_count;
    }

    jsmntok_t* tokens = (jsmntok_t*)memory_alloc(options->memory.user_data, sizeof(jsmntok_t) * (options->json_token_count + 1));

    if (!tokens)
    {
        return cgltf_result_out_of_memory;
    }

    jsmn_init(&parser);

    int token_count = jsmn_parse(&parser, (const char*)json_chunk, size, tokens, options->json_token_count);

    if (token_count <= 0)
    {
        memory_free(options->memory.user_data, tokens);
        return cgltf_result_invalid_json;
    }

    // this makes sure that we always have an UNDEFINED token at the end of the stream
    // for invalid JSON inputs this makes sure we don't perform out of bound reads of token data
    tokens[token_count].type = JSMN_UNDEFINED;

    fila_custom_param_sampler* data = (fila_custom_param_sampler*)memory_alloc(options->memory.user_data, sizeof(fila_custom_param_sampler));

    if (!data)
    {
        memory_free(options->memory.user_data, tokens);
        return cgltf_result_out_of_memory;
    }

    memset(data, 0, sizeof(fila_custom_param_sampler));
    init_fila_custom_param_sampler_data(data);
    data->memory_free = memory_free;
    data->memory_user_data = options->memory.user_data;

    int i = fila_parse_custom_param_sampler_json_root(options, tokens, 0, (const uint8_t*)json_chunk, data);

    memory_free(options->memory.user_data, tokens);

    if (i < 0)
    {
        fila_custom_param_sampler_free(data);
        return (i == CGLTF_ERROR_NOMEM) ? cgltf_result_out_of_memory : cgltf_result_invalid_json;
    }

    *out_data = data;

    return cgltf_result_success;
}

cgltf_result fila_parse_value_config_file(cgltf_options* options, const char* path, fila_custom_param_sampler** out_data)
{
    if (options == NULL)
    {
        return cgltf_result_invalid_options;
    }

    if(!options->memory.alloc) {
        options->memory.alloc = &cgltf_default_alloc;
    }
    if(!options->memory.free) {
        options->memory.free = &cgltf_default_free;
    }

    void* (*memory_alloc)(void*, cgltf_size) = options->memory.alloc;
    void (*memory_free)(void*, void*) = options->memory.free;

    FILE* file = fopen(path, "rb");
    if (!file)
    {
        return cgltf_result_file_not_found;
    }

    fseek(file, 0, SEEK_END);

    long length = ftell(file);
    if (length < 0)
    {
        fclose(file);
        return cgltf_result_io_error;
    }

    fseek(file, 0, SEEK_SET);

    char* file_data = (char*)memory_alloc(options->memory.user_data, length);
    if (!file_data)
    {
        fclose(file);
        return cgltf_result_out_of_memory;
    }

    cgltf_size file_size = (cgltf_size)length;
    cgltf_size read_size = fread(file_data, 1, file_size, file);

    fclose(file);

    if (read_size != file_size)
    {
        memory_free(options->memory.user_data, file_data);
        return cgltf_result_io_error;
    }

    cgltf_result result = fila_parse_value_sampler(options, file_data, file_size, out_data);

    if (result != cgltf_result_success)
    {
        memory_free(options->memory.user_data, file_data);
        return result;
    }

    (*out_data)->file_data = file_data;

    return cgltf_result_success;
}

static void fila_custom_free(fila_custom* custom_data){
    // TODO
}

static void fila_custom_param_sampler_free(fila_custom_param_sampler* custom_data)
{
    // TODO
}

#endif /* #ifdef CGLTF_IMPLEMENTATION */
#endif //TNT_FILA_CUSTOMMATERIAL_H
