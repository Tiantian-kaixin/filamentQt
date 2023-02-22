//
// Created by kevinxing on 2020-03-04.
//

#ifndef TNT_MATERIALMANAGER_H
#define TNT_MATERIALMANAGER_H

#include <filament/Material.h>
#include <gltfio/file_util.h>
#include <gltfio/log_util.h>
#include <filament/Engine.h>

#if ENABLE_LOG
#include <iostream>
#endif
#include <vector>
#include <map>

#ifdef WIN32
#undef TRANSPARENT
#endif

LIGHT3D_NAMESPACE_BEGIN

enum class MaterialType {
    BLOOM_BLUR,
    BLOOM_MIX,
    FACE_MESH,
    FACE_MESH_LIT,
    HEAD,
    HEAD_3DMM,
    FUR,
    FUR_LIGHT,
    FUR_LIGHT_ECS,
    LOCATION_INIT,
    REFLECTION,
    HEAD_FEATHER,
    BAKED_TEXTURE,
    NORMAL,
    BLIT,
    MIX,
    TRANSPARENT,
    TAIL,

    // 以下为kapu材质
    DISNEY_BASIC_OPAQUE,
    DISNEY_BASIC_MASKED,
    DISNEY_BASIC_FADE,
    DISNEY_BASIC_TRANSPARENT,

    DISNEY_HAIR_OPAQUE,
    DISNEY_HAIR_MASKED,
    DISNEY_HAIR_FADE,
    DISNEY_HAIR_TRANSPARENT,

    KAPU_CUSTOM_SP_OPAQUE,
    KAPU_CUSTOM_SP_MASKED,
    KAPU_CUSTOM_SP_FADE,
    KAPU_CUSTOM_SP_TRANSPARENT,

    KAPU_CUSTOM_MR_OPAQUE,
    KAPU_CUSTOM_MR_MASKED,
    KAPU_CUSTOM_MR_FADE,
    KAPU_CUSTOM_MR_TRANSPARENT,

    KAPU_STANDARD_OPAQUE,
    KAPU_STANDARD_MASKED,
    KAPU_STANDARD_FADE,
    KAPU_STANDARD_TRANSPARENT,

    // kapu Makeup 材质
    KAPU_MAKEUP,
};

// MaterialManager is responsible for fetching and releasing materials in local material directory and assets
// material directory. You must not destroy materials by your self.
class MaterialManager {
public:
    MaterialManager(filament::Engine *engine, utils::Path material_directory, utils::Path resource_directory);
    ~MaterialManager();

    filament::Material *GetLocalMaterial(const std::string &filename);
    filament::Material* GetBaseMaterial(MaterialType type);
    filament::MaterialInstance * CreateLocalMaterialInstance(const std::string &filename);
    void addKapuMaterialInstance(filament::MaterialInstance* mi);
    filament::Material *loadMaterial(const utils::Path &path);
    filament::Material *loadMaterial(const utils::Path &dir, const utils::Path &filename);

private:
    filament::Engine *engine_;
    utils::Path material_directory_;
    utils::Path resource_path_;
    std::map<std::string, filament::Material*> pathToMaterial; // used to cash materials
    std::vector<filament::MaterialInstance*> cashedMaterialInstance; // used to removed in destructor later
    std::map<MaterialType, std::string> typeToPath;
    std::map<MaterialType, std::string> optionMat;
    std::map<MaterialType, filament::Material*> typeToMaterial;
};

LIGHT3D_NAMESPACE_END

#endif //TNT_MATERIALMANAGER_H
