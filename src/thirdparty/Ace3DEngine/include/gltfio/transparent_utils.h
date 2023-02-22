//
// Created by ericwywang on 2020/10/22.
//

#ifndef TNT_TRANSPARENT_UTILS_H
#define TNT_TRANSPARENT_UTILS_H

#include <gltfio/common_util.h>
#include <gltfio/MaterialManager.h>

#include <filament/Engine.h>
#include <filament/Texture.h>
#include <filament/Box.h>

#include <math/mat4.h>

LIGHT3D_NAMESPACE_BEGIN

    class TransparentUtils {


        filament::VertexBuffer *vertexBuffer = nullptr;
        filament::IndexBuffer *indexBuffer = nullptr;
        filament::Engine *engine = nullptr;
    public:
        struct Framebuffer {
            filament::Camera *camera = nullptr;
            filament::View *view = nullptr;
            filament::Scene *scene = nullptr;
            filament::Texture *color = nullptr;
            filament::Texture *depth = nullptr;
            filament::RenderTarget *target = nullptr;
            utils::Entity cameraEntity;

            void destroy(filament::Engine *engine) {
                if (camera != nullptr) {
                    engine->destroyCameraComponent(cameraEntity);
                    engine->getEntityManager().destroy(cameraEntity);
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

        enum ImageOp {
            BLIT, MIX
        };
        
        TransparentUtils(filament::Engine *engine) : engine(engine) {};

        ~TransparentUtils();

        utils::Entity createQuad(const SMART_PTR<MaterialManager> &manager,
                                 filament::Texture *tex, ImageOp op, filament::Texture *secondary = nullptr);

        void initFrameBuffer(Framebuffer &framebuffer, int width, int height);

        void initRT(Framebuffer *framebuffer, int width, int height);

    };

LIGHT3D_NAMESPACE_END

#endif //TNT_TRANSPARENT_UTILS_H
