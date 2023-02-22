//
// Created by kevinxing on 2020-03-04.
//

#ifndef LIBS_GLTFIO_INCLUDE_GLTFIO_TEXTUREMANAGER_H_
#define LIBS_GLTFIO_INCLUDE_GLTFIO_TEXTUREMANAGER_H_

#include <gltfio/common_util.h>
#include <string>
#include <map>
#include <filament/Texture.h>
#include <filament/Engine.h>
#include <filament/Texture.h>
#include <gltfio/common_util.h>
#include <gltfio/stb_image_android.h>
#include <gltfio/log_util.h>
#include <gltfio/file_util.h>

// TODO(@dianxin) : Texture manager is responsible for creating texture.
LIGHT3D_NAMESPACE_BEGIN

const std::string ETC_ALPHA_POSTFIX = "_Alpha";

// TODO(@dianxin) : Make TextureManager as a singleton
// TODO(@dianxin) : Texture manager should be responsible for managing texture (including creation and release).
// It will automatically release the corresponding texture if it's not used anymore.
class TextureManager {
 public:
    TextureManager(filament::Engine* engine, utils::Path texture_directory, bool is_mac_platform = false)
        : engine_(engine), texture_directory_(texture_directory), is_mac_platform_(is_mac_platform) {}
    ~TextureManager() {
        for(auto& iter : name_to_texture_) {
            engine_->destroy(iter.second);
        }
        for (auto& iter : mapTexIdToTexture) {
            engine_->destroy(iter.second);
        }
    }

    void setImageHandle(gltfio::stb_image_handle* imageHandle) {
        handle_ = imageHandle;
    }

    bool loadCubemapLevel(filament::Engine *engine, filament::Texture **texture, const utils::Path &path, size_t level = 0,
                          std::string const &levelPrefix = "") const;

    bool loadCubemapLevel(filament::Engine *engine,
                          filament::Texture **texture,
                          filament::Texture::PixelBufferDescriptor *outBuffer,
                          filament::Texture::FaceOffsets *outOffsets,
                          const utils::Path &path, size_t level, std::string const &levelPrefix) const;

    filament::Texture* Create2DTexture(uint32_t width, uint32_t height, bool sRGBA);
    void InsertExternalTexture(const std::string& texture_name, filament::Texture* texture);
    filament::Texture* createFromOriginTexture(uint32_t texId, uint32_t width, uint32_t height,
                                               bool sRGBA = false);
    filament::Texture* loadTexture(const std::string& texture_name, bool sRGBA = true, filament::Texture** etcAlphaTexture = nullptr);
    filament::Texture* loadCubeMapTexture(const std::string& filePath);
    filament::Texture* loadCubeMapTextureFromPng(const std::string& filePath);
    void clearTexture(const std::string& filename);
    void clearAllMap() {
        for (auto& iter : name_to_texture_) {
            engine_->destroy(iter.second);
        }
        for (auto& iter : mapTexIdToTexture) {
            engine_->destroy(iter.second);
        }
        name_to_texture_ = {};
        mapTexIdToTexture = {};
    };
    gltfio::stb_image_handle* getImageHandle() {
        return handle_;
    }

    bool is_mac_platform_;

private:
    filament::Engine* engine_;
    gltfio::stb_image_handle* handle_ = nullptr;
    utils::Path texture_directory_;
    std::map<std::string, filament::Texture*> name_to_texture_;
    std::map<uint32_t, filament::Texture*> mapTexIdToTexture;
};

LIGHT3D_NAMESPACE_END

#endif  // LIBS_GLTFIO_INCLUDE_GLTFIO_TEXTUREMANAGER_H_
