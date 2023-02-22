//
// Created by dianxinwang on 2020/6/1.
//

#ifndef TNT_RESOURCEHELPER_H
#define TNT_RESOURCEHELPER_H

#include <gltfio/FilamentAsset.h>
#include <gltfio/ResourceLoader.h>
#include "stb_image_android.h"
#include <set>
#include <vector>
#include <string>
#include <utils/Path.h>
#include <gltfio/common_util.h>
#include <unordered_map>

LIGHT3D_NAMESPACE_BEGIN

class ResourceHelper {
public:
    ResourceHelper(const gltfio::ResourceConfiguration &config);
    ~ResourceHelper() {
        for (auto &ar : resourceLoaderMap) {
            delete ar.second;
        }
    }
    decltype(auto) loadResources(gltfio::FilamentAsset* asset) {
        addResources(asset); // This should be fast if resources have been loaded
        return getResourceLoader(asset)->loadResources(asset);
    }

    void loadResourcesAsync(gltfio::FilamentAsset* asset);

    bool isResourceLoadCompleted() const;

    void asyncUpdateLoad();

    gltfio::ResourceLoader* getResourceLoader(gltfio::FilamentAsset* asset);

private:
    void addResources(gltfio::FilamentAsset* asset);

    std::unordered_map<gltfio::FilamentAsset*, gltfio::ResourceLoader*> resourceLoaderMap;
    const utils::Path resourcePath;
    utils::Path gltfPath;
    bool loadResourceAsync;
    gltfio::ResourceConfiguration resourceConfig;
    std::set<std::string> gltfResourcesAdded = {};
};


LIGHT3D_NAMESPACE_END

#endif //TNT_RESOURCEHELPER_H
