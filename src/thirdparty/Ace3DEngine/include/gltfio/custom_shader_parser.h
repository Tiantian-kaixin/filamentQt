//
// Created by panda on 2020-02-21.
//

#ifndef TNT_CUSTOM_SHADER_PARSER_H
#define TNT_CUSTOM_SHADER_PARSER_H

#include <vector>
#include <gltfio/ModelDef.h>
#ifndef FILAMENT_ENV_IOS
    #include "fila_struct.h"
#else
    #include <gltfio/fila_struct.h>
#endif
#include <backend/DriverEnums.h>
#include <gltfio/log_util.h>
#include <locale>

LIGHT3D_NAMESPACE_BEGIN

namespace parser {
    std::vector<std::string> array2Vector(char** array, const cgltf_size array_size)
    {
        std::vector<std::string> list;
        for(size_t i = 0; i < array_size; i++) {
            std::string str = array[i];
            list.push_back(str);
        }
        return list;
    }

    std::vector<TTCustomShader *> parseShader(const fila_custom* customMaterials, const cgltf_size customMaterialsCount, const utils::Path& materialDir)
    {
        std::vector<TTCustomShader *> shaders;
        for(size_t index = 0; index < customMaterialsCount; index++) {
            auto shader = new TTCustomShader;
            fila_custom custom = customMaterials[index];
            shader->filamatPath = custom.matName ? custom.matName : "";
            shader->assetType = TTCustomShader::AssetType::NORMAL;
            if(custom.assetType != nullptr) {
                if(!strcmp(custom.assetType, "fur")) {
                    shader->assetType = TTCustomShader::AssetType::FUR;
                } else if(!strcmp(custom.assetType, "shadowPlane")) {
                    shader->assetType = TTCustomShader::AssetType::SHADOW_PLANE;
                } else if(!strcmp(custom.assetType, "head")) {
                    shader->assetType = TTCustomShader::AssetType::HEAD_3DMM;
                } else if(!strcmp(custom.assetType, "transparent")) {
                    shader->assetType = TTCustomShader::AssetType::TRANSPARENT;
                } else if(!strcmp(custom.assetType, "transparentTail")) {
                    shader->assetType = TTCustomShader::AssetType::TRANSPARENT_TAIL;
                }
            }
            shader->nodeNameList = array2Vector(custom.nodeNameList, custom.nodeNameListCount);

            for(size_t ii = 0; ii < custom.customParamsListCount; ii++) {
                fila_custom_params params = custom.customParamsList[ii];
                auto param = new TTCustomUniformParam(params);
                shader->parameterVec.push_back(param);
                shader->param_name_to_vector_index.insert_or_assign(param->param_name_,shader->parameterVec.size()-1);
                if (strcmp("$normal", param->GetCurrentValueInString()[0].c_str()) == 0) {
                    shader->needNormal = true;
                }
            }
            shaders.push_back(shader);
        }
        return shaders;
    }
}

LIGHT3D_NAMESPACE_END

#endif //TNT_CUSTOM_SHADER_PARSER_H
