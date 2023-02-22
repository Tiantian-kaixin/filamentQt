//
// Created by ericwywang on 2020/9/1.
//

#ifndef TNT_GLTFCONVERTER_H
#define TNT_GLTFCONVERTER_H

#include <gltfio/ModelDef.h>
#include <loader/AccessoryLoader.h>
#include <loader/AnimationLoader.h>
#include <loader/kapu_struct.h>
#include <list>
#include <set>

#if (__has_include(<cgltf/cgltf.h>))
#include <cgltf/cgltf.h>
#else

#include "cgltf.h"

#endif
#include <loader/SkeletonLoader.h>
#include <gltfio/fila_kapu_material.h>

struct VerticesInfo {
    float *verticesBase = nullptr;
    int baseCount = 0;
    int16_t *indexBase = nullptr;
    int indexBaseCount = 0;
    float *normalsBase = nullptr;
    float *result = nullptr;
    float *uvBase = nullptr;
    int uvCount = 0;
    void clear() {
        if (uvBase) {
          delete[] uvBase;
          uvBase = nullptr;
        }
        delete[] verticesBase;
        verticesBase = nullptr;
        delete[] indexBase;
        indexBase = nullptr;
        delete[] normalsBase;
        normalsBase = nullptr;
        delete[] result;
        result = nullptr;
    }

    ~VerticesInfo() {
      if (uvBase) {
        delete[] uvBase;
        uvBase = nullptr;
      }
        delete[] verticesBase;
        delete[] indexBase;
        delete[] normalsBase;
        delete[] result;
    }
};

struct MorphTarget {
    std::string name;
    int count = 0;
    float *vertices = nullptr;
    float *normals = nullptr;
    int32_t *index = nullptr;
    int indexCount = 0;
    int verticesCount = 0;
    int normalsCount = 0;

    ~MorphTarget() {
        if (vertices) {
            delete[] vertices;
            vertices = nullptr;
        }
        if (normals) {
            delete[] normals;
            normals = nullptr;
        }
        if (index) {
            delete[] index;
            index = nullptr;
        }
    }
};

struct MorphData {
    VerticesInfo info;
    std::vector<MorphTarget *> targets;

    ~MorphData() {
        clearTargets();
        info.clear();
    }

    void clearTargets() {
        for (auto target: targets) {
            if (target == nullptr || target->vertices == nullptr) continue;
            delete target;
            target = nullptr;
        }
        targets.clear();
    }
};

struct NodeData {
    cgltf_node *node = nullptr;
    std::string parent;
    std::vector<std::string> children;
};

class GltfConverter {
private:
    std::map<std::string, std::string> keyMap = {
            {"_MainTex", "baseColorMap"}
    };

    std::list<std::string> faceMorphPartList = {"face", "eyebrow", "eyelash", "mouth", "tongue", "tooth_up", "tooth_down", "eyes_l", "eyes_r"}; // 3DMM需要的9个morph 部位

    std::map<std::string, float> weightMap;

    std::map<std::string, NodeData> nodeMap;

    int accessory2node(CM::Accessory::Accessory *accessory,
                       cgltf_node *node_out, std::map<std::string, fila_kapu_material*> *kapu_materials, MorphData* morphData);

    int addSkin(CM::Accessory::Accessory *accessory, cgltf_skin *skin);

public:
    int skeleton2skin(std::vector<CM::Skeleton::Skeleton *> &skeletons, std::vector<std::string> &skeletonVec);

    void modifySkinNodes(std::set<std::string> &skeletons);

    cgltf_animation* clips2animation(std::vector<T3D::TAnimationClip *> &clips);

    int accessories2sourceAsset(std::vector<CM::Accessory::Accessory *> &accessories,
                                cgltf_data **data_out, std::map<std::string, fila_kapu_material*> *kapu_materials,
                                std::map<std::string, MorphData *> *blendShapeMap, std::string rootNode);

    std::map<std::string, std::string> getRelativeNameMap(const std::vector<CM::Accessory::Texture::TextureParameter *>& textureParams);

    std::list<std::string> getFaceMorphPartList();

    ~GltfConverter();
};


#endif //TNT_GLTFCONVERTER_H
