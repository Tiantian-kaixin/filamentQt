//
// Created by ericwywang on 2020/12/1.
//

#ifndef TNT_FACEBINDHELPER_H
#define TNT_FACEBINDHELPER_H

#include <gltfio/ModelDef.h>

LIGHT3D_NAMESPACE_BEGIN

    class FaceBindHelper {
    private:
        float *faceVertices = nullptr;
        filament::Camera *camera = nullptr;
        filament::Engine *engine = nullptr;
        int width;
        int height;
        std::map<const utils::Entity *, filament::math::mat4f> transformMap;
        filament::math::float3 posOffset;

    public:
        FaceBindHelper(filament::Engine *engine, filament::Camera *camera, int width, int height) :
                engine(engine), camera(camera), width(width), height(height), posOffset(0) {
        };

        ~FaceBindHelper() = default;

        void updateVertices(float *vertices) {
            faceVertices = vertices;
        }

        void updateSize(int width, int height) {
            this->width = width;
            this->height = height;
        }

        void updatePosOffset(const filament::math::float3 &offset) { posOffset = offset; }

        void bindFacePoint(const light3d::TTFaceBindConfig &config, gltfio::FilamentInstance *instance);

        void bindFacePoint(const light3d::TTFaceBindConfig &config, gltfio::FilamentAsset *asset);

        void bindFacePointEntity(const utils::Entity *entity, int faceIndex, const filament::math::mat4f &trans);
    };

LIGHT3D_NAMESPACE_END

#endif //TNT_FACEBINDHELPER_H
