//
// Created by ericwywang on 2020/3/30.
//

#ifndef TNT_GEOMETRYCOMPONENT_H
#define TNT_GEOMETRYCOMPONENT_H

#include <gltfio/ModelDef.h>
#include <gltfio/common_util.h>
#include <utility>

LIGHT3D_NAMESPACE_BEGIN

class GeometryComponent {
    filament::Engine *mEngine;
    std::vector<std::map<std::pair<std::string, int>, filament::VertexBuffer*>> vertexMap;
    std::vector<gltfio::FilamentAsset* > assetList;

 public:
    explicit GeometryComponent(filament::Engine *engine, filament::Scene *scene)
        : mEngine(engine){};

    void initConfig(const utils::Path& materialDir, const utils::Path& resourcePath);

    void setup(gltfio::FilamentAsset *asset);

    void updateVertex(std::string& name, float *vert, int size);

    void updateMorph(std::string& name, int num, float *vert, int size);

    void updateMorphBase(std::string& name, float *vert, int size);
};

LIGHT3D_NAMESPACE_END

#endif  // TNT_GEOMETRYCOMPONENT_H
