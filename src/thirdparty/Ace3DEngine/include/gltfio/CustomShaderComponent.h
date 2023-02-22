//
// Created by panda on 2020-03-07.
//

#ifndef TNT_CUSTOMSHADERCOMPONENT_H
#define TNT_CUSTOMSHADERCOMPONENT_H

#include <string>
#include <set>
#include <gltfio/ModelDef.h>
#include <gltfio/TextureManager.h>
#include <gltfio/MaterialManager.h>
#include <filament/Material.h>
#include <gltfio/FilamentAsset.h>
#include <gltfio/NormalCubemapComponent.h>
#include <gltfio/IblManager.h>

LIGHT3D_NAMESPACE_BEGIN

class CustomShaderComponent {
private:
    filament::Engine *engine_;
    SMART_PTR<TTFilamentSetupParam> params_;
    SMART_PTR<TextureManager> texture_manager_;
    SMART_PTR<NormalCubemapComponent> normalCubemapComponent;
    std::map<std::string, filament::Texture *> preserved_texture_map_;

public:
    explicit CustomShaderComponent(filament::Engine *engine, filament::Scene *scene);

    void initConfig(SMART_PTR<TTFilamentSetupParam> param, SMART_PTR<TextureManager> textureManager, SMART_PTR<MaterialManager> materialManager);

    void updateRenderTime(float render_time);

    void updateKapuParams(const std::vector<TTCustomShader *>& customShaderParams);

    void setNormalComponent(SMART_PTR<NormalCubemapComponent> component);

    bool MIParamsShouldLoadTexture(size_t width, size_t height);

    void updateCustomShaderParameter(int materialIndex, int customShaderMaterialIndex,
                                     const std::string &parameterName,
                                     const std::vector<std::string> &value,
                                     const std::string &valueSamplerConfigFile);

    std::vector<TTCustomShader *> getCustomShaderList();

    const std::map<std::string, filament::Texture *> getExternalTextureMap();
};

LIGHT3D_NAMESPACE_END

#endif //TNT_CUSTOMSHADERCOMPONENT_H
