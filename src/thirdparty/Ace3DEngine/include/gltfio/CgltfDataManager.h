//
// Created by Xueyuan Xing on 2020-03-09.
//

#ifndef TNT_CGLTFDATAMANAGER_H
#define TNT_CGLTFDATAMANAGER_H

#include <string>
#include <map>
#include <gltfio/AssetLoader.h>
#include <filament/Scene.h>
#include <gltfio/common_util.h>

LIGHT3D_NAMESPACE_BEGIN

class CgltfDataManager {
 public:
    void* loadCgltfData(gltfio::AssetLoader* assetLoader, std::string& glbPath,
                        std::vector<uint8_t>& glbData) {
        void* gltfData = nullptr;

        auto iter = mapAssetGLTFData.find(glbPath);
        if (iter != mapAssetGLTFData.end()) {
            gltfData = iter->second;
        } else {
            gltfData = assetLoader->createCGLTFFromBinary((const uint8_t *) glbData.data(),
                                                          static_cast<uint32_t>(glbData.size()));
            mapAssetGLTFData.insert(std::pair<std::string, void *>(glbPath, gltfData));
        }
        return gltfData;
    }

 private:
    std::map<std::string, void*> mapAssetGLTFData;
};

LIGHT3D_NAMESPACE_END

#endif //TNT_CGLTFDATAMANAGER_H
