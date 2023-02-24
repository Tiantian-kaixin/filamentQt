//
// Created by TIANTIAN on 2023/2/14.
//

#include "FilamentWindow.h"

static constexpr uint8_t BAKED_COLOR_PACKAGE[] = {
#include "./asset/material/bakedColor.inc"
};

struct Vertex {
    filament::math::float2 position;
    uint32_t color;
};

static const Vertex TRIANGLE_VERTICES[3] = {
        {{1, 0}, 0xffff0000u},
        {{cos(M_PI * 2 / 3), sin(M_PI * 2 / 3)}, 0xff00ff00u},
        {{cos(M_PI * 4 / 3), sin(M_PI * 4 / 3)}, 0xff0000ffu},
};

static constexpr uint16_t TRIANGLE_INDICES[3] = { 0, 1, 2 };

FilamentWindow::FilamentWindow(): curFrame(0) {
    doInitialize();
}

FilamentWindow::~FilamentWindow() {
    doFree();
}

void FilamentWindow::doFree() {

}

void FilamentWindow::doInitialize() {
    engine = filament::Engine::create();
    swapChain = engine->createSwapChain((void*)this->winId());
    render = engine->createRenderer();
    camera = engine->createCamera(utils::EntityManager::get().create());
    view = engine->createView();
    scene = engine->createScene();

    view->setClearOptions({.clearColor = {0.1f, 1.0f, 0.1f, 1.0f}, .clear = true});
    view->setPostProcessingEnabled(false);
    renderable = utils::EntityManager::get().create();

    auto vertexBuffer = filament::VertexBuffer::Builder()
            .vertexCount(3)
            .bufferCount(1)
            .attribute(filament::VertexAttribute::POSITION, 0, filament::VertexBuffer::AttributeType::FLOAT2, 0, 12)
            .attribute(filament::VertexAttribute::COLOR, 0, filament::VertexBuffer::AttributeType::UBYTE4, 8, 12)
            .normalized(filament::VertexAttribute::COLOR)
            .build(*engine);
    vertexBuffer->setBufferAt(*engine, 0, filament::VertexBuffer::BufferDescriptor(TRIANGLE_VERTICES, 36, nullptr));

    auto indexBuffer = filament::IndexBuffer::Builder()
            .indexCount(3)
            .bufferType(filament::IndexBuffer::IndexType::USHORT)
            .build(*engine);
    indexBuffer->setBuffer(*engine, filament::IndexBuffer::BufferDescriptor(TRIANGLE_INDICES, 6, nullptr));
// build a quad
    filament::Material* material = filament::Material::Builder()
            .package((void*) BAKED_COLOR_PACKAGE, sizeof(BAKED_COLOR_PACKAGE))
            .build(*engine);
    materialInstance = material->createInstance();
    filament::RenderableManager::Builder(1)
            .boundingBox({{ -1, -1, -1 }, { 1, 1, 1 }})
            .material(0, materialInstance)
            .geometry(0, filament::RenderableManager::PrimitiveType::TRIANGLES, vertexBuffer, indexBuffer, 0, 3)
            .receiveShadows(false)
            .castShadows(false)
            .build(*engine, renderable);
    scene->addEntity(renderable);
    view->setScene(scene);
    view->setCamera(camera);
    view->setViewport(filament::Viewport(0, 0, 600, 600));
}

void FilamentWindow::updateFrame() {
    if (render->beginFrame(swapChain)) {
        qDebug() << "updateFrame" << endl;
        // for each View
        render->render(view);
        render->endFrame();
    }
}

void FilamentWindow::updateParameter(QJsonArray json) {
    materialInstance->setParameter("texture",
       filament::math::float4(json[0].toDouble(), json[1].toDouble(), json[2].toDouble(), json[3].toDouble()));
    updateRotation();
    updateFrame();
}

void FilamentWindow::updateRotation() {
    curFrame += 1;
    filament::TransformManager& tr = engine->getTransformManager();
    filament::math::mat4f curMat = filament::math::mat4f::rotation(curFrame * 0.1, filament::math::float3{0, 0, 1});
    tr.setTransform(tr.getInstance(renderable), curMat);
}
