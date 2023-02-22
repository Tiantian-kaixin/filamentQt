//
// Created by kevinxing on 2019-11-06.
//

#ifndef TNT_GL_UTIL_H
#define TNT_GL_UTIL_H

#include "common_util.h"
#include <filament/Camera.h>
#include <filament/View.h>
#include <filament/Scene.h>
#include <filament/Texture.h>
#include <filament/RenderTarget.h>
#include <filament/IndexBuffer.h>
#include <gltfio/material_util.h>
#include <unordered_map>
// bloom
#define FILTER_SIZE 17
#define SAMPLE_COUNT (1 + FILTER_SIZE / 2)

LIGHT3D_NAMESPACE_BEGIN

namespace gl_util {

struct Framebuffer {
    filament::Camera* camera = nullptr;
    filament::View* view = nullptr;
    filament::Scene* scene = nullptr;
    filament::Texture* color = nullptr;
    filament::Texture* depth = nullptr;
    filament::RenderTarget* target = nullptr;
    void destroy(filament::Engine* engine) {
        if (camera) {
            engine->destroy(camera);
        }
        engine->destroy(view);
        engine->destroy(scene);
        engine->destroy(color);
        engine->destroy(depth);
        engine->destroy(target);
        camera = nullptr;
        view = nullptr;
        scene = nullptr;
        color = depth = nullptr;
        target = nullptr;
    }
};
struct Vertex {
    filament::math::float2 position;
    filament::math::float2 uv;
};

struct QuadConfig {
    Vertex verts[4] = {
            {{-1, -1}, {0, 0}},
            {{1,  -1}, {1, 0}},
            {{-1, 1},  {0, 1}},
            {{1,  1},  {1, 1}},
    };
    uint16_t indices[6] = {
            0, 1, 2,
            3, 2, 1,
    };
    filament::MaterialInstance* matInstance = nullptr;
    std::unordered_map<std::string, filament::Texture*> textureParams;
};

static utils::Entity createQuad(filament::Engine* engine, const QuadConfig& config) {

    auto freecb = [](void* buffer, size_t size, void* user) { free(buffer); };
    filament::VertexBuffer* vertexBuffer = [&](filament::Engine& engine) {

        auto vb = filament::VertexBuffer::Builder()
            .vertexCount(4)
            .bufferCount(1)
            .attribute(filament::VertexAttribute::POSITION, 0, filament::VertexBuffer::AttributeType::FLOAT2, 0, 16)
            .attribute(filament::VertexAttribute::UV0, 0, filament::VertexBuffer::AttributeType::FLOAT2, 8, 16)
            .build(engine);
        void* vertices = malloc(sizeof(config.verts));
        memcpy(vertices, config.verts, sizeof(config.verts));
        vb->setBufferAt(engine, 0, filament::VertexBuffer::BufferDescriptor(vertices, sizeof(config.verts), freecb));
        return vb;
    }(*engine);

    static filament::IndexBuffer* indexBuffer = [&](filament::Engine& engine) {
        auto ib = filament::IndexBuffer::Builder()
            .indexCount(6)
            .bufferType(filament::IndexBuffer::IndexType::USHORT)
            .build(engine);
        void* indices = malloc(sizeof(config.indices));
        memcpy(indices, config.indices, sizeof(config.indices));
        ib->setBuffer(engine, filament::IndexBuffer::BufferDescriptor(indices, sizeof(config.indices), freecb));
        return ib;
    }(*engine);

    filament::TextureSampler sampler(filament::TextureSampler::MinFilter::LINEAR, filament::TextureSampler::MagFilter::LINEAR);

    for (const auto& iter : config.textureParams) {
        config.matInstance->setParameter(iter.first.c_str(), iter.second, sampler);
    }

    utils::Entity entity = utils::EntityManager::get().create();
    filament::RenderableManager::Builder(1)
        .boundingBox({ { 0, 0, 0 }, { 1, 1, 1 } })
        .material(0, config.matInstance)
        .geometry(0, filament::RenderableManager::PrimitiveType::TRIANGLES, vertexBuffer, indexBuffer)
        .culling(false)
        .receiveShadows(false)
        .castShadows(false)
        .build(*engine, entity);

    return entity;
}
}

namespace BloomUtil {

enum ImageOp { HBLUR, VBLUR, MIX };
enum FrameBufferType { NORMAL, BLUR };

struct QuadConfig {
    filament::Material* blurMat = nullptr;
    filament::Material* mixMat = nullptr;
    filament::VertexBuffer* vertexBuffer = nullptr;
    filament::IndexBuffer* indexBuffer = nullptr;
    filament::Texture* tex = nullptr;
    ImageOp op;
    filament::Texture* secondary = nullptr;     // blur view
    filament::Texture* bloom = nullptr;         // bloom view
    float blurWeight = 0.2;
    float mixAlpha = 0.3;
    int mixType = 0;
    float offsetX = 1.0;           // 采样步长, 影响羽化模糊范围
    float offsetY = 1.0;
    float sigma = 7.0;            // 正太分布 sigma, 影响模糊权重, 最终影响模糊效果
};


utils::Entity createQuad(filament::Engine* engine, QuadConfig config,
    std::vector<filament::MaterialInstance*>& matList);
void initFramebuffer(filament::Engine* engine, gl_util::Framebuffer& frameBuffer,
                     int width, int height, FrameBufferType type, bool noRenderTarget = false);
}

LIGHT3D_NAMESPACE_END

#endif //TNT_GL_UTIL_H
