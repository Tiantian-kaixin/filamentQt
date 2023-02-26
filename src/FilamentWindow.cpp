//
// Created by TIANTIAN on 2023/2/14.
//

#include "FilamentWindow.h"

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
    manipulator = filament::camutils::Manipulator<float>::Builder()
            .viewport(600, 600)
            .targetPosition(0, 0, 0)
            .orbitHomePosition(0, 0, 6)
            .upVector(0, 1, 0)
            .build(filament::camutils::Mode::ORBIT);
    camera->setLensProjection(28, 1, 0.05, 1000);
    camera->lookAt(filament::math::float3{0, 0, 3}, filament::math::float3{0, 0, 0});
    view->setClearOptions({.clearColor = {0.1f, 1.0f, 0.1f, 1.0f}, .clear = true});
    view->setPostProcessingEnabled(false);
    view->setScene(scene);
    view->setCamera(camera);
    view->setViewport(filament::Viewport(0, 0, 1200, 1200));
    loadModelGlb("/Users/tiantian/Downloads/cube.glb");
    loadLight("/Users/tiantian/Downloads/default_env_ibl.ktx");
}

void FilamentWindow::loadModelGlb(std::string filename) {
    // readfile
    uint64_t size;
    std::vector<uint8_t> buffer = loadFileBuffer(filename, size);
    filament::Material* material = filament::Material::Builder()
            .build(*engine);
    materialInstance = material->createInstance();

    auto& entityManager = utils::EntityManager::get();
    auto componentManager = new utils::NameComponentManager(entityManager);
    auto assetLoader = gltfio::AssetLoader::create({
       engine,
       gltfio::createMaterialGenerator(engine),
       componentManager,
       &entityManager});
    gltfio::FilamentAsset* asset = assetLoader->createAssetFromBinary(buffer.data(), buffer.size());
    if (!asset) {
        qDebug() << "create asset failed" << endl;
    }
    buffer.clear();
    scene->addEntities(asset->getEntities(), asset->getEntityCount());
    auto resourceLoader = new gltfio::ResourceLoader({.engine = engine, .normalizeSkinningWeights = true});
    resourceLoader->loadResources(asset);
    asset->releaseSourceData();
}

void FilamentWindow::loadLight(std::string filename) {
    uint64_t size;
    std::vector<uint8_t> buffer = loadFileBuffer(filename, size);
    auto* _iblBundle = new image::KtxBundle(reinterpret_cast<uint8_t*>(buffer.data()), size);
    auto _iblTexture = image::ktx::createTexture(engine, _iblBundle, false);
    filament::math::float3 harmonics[9];
    _iblBundle->getSphericalHarmonics(harmonics);
    auto _indirectLight = filament::IndirectLight::Builder()
            .reflections(_iblTexture)
            .irradiance(3, harmonics)
            .intensity(30000.0f)
            .build(*engine);
    scene->setIndirectLight(_indirectLight);
}

void FilamentWindow::loadShader() {
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
    filament::Material* material = filament::Material::Builder()
            .build(*engine);
    materialInstance = material->createInstance();
    materialInstance->setParameter("texture", filament::math::float4(1, 1, 1, 1));
    filament::RenderableManager::Builder(1)
            .boundingBox({{ -1, -1, -1 }, { 1, 1, 1 }})
            .material(0, materialInstance)
            .geometry(0, filament::RenderableManager::PrimitiveType::TRIANGLES, vertexBuffer, indexBuffer, 0, 3)
            .receiveShadows(false)
            .castShadows(false)
            .build(*engine, renderable);
    scene->addEntity(renderable);
}

void FilamentWindow::updateFrame() {
    filament::math::float3 eye, center, up;
    manipulator->getLookAt(&eye, &center, &up);
    camera->lookAt(eye, center, up);
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

std::vector<uint8_t> FilamentWindow::loadFileBuffer(std::string filename, uint64_t& size) {
    std::ifstream in2(filename, std::ifstream::ate | std::ifstream::binary);
    size = static_cast<uint64_t>(in2.tellg());
    in2.close();
    std::vector<uint8_t> buffer(size);
    std::ifstream in(filename, std::ifstream::binary | std::ifstream::in);
    if (!in.read(reinterpret_cast<char*>(buffer.data()), size)) {
        qDebug() << "cannot read file" << endl;
        exit(1);
    }
    return buffer;
}

void FilamentWindow::mousePressEvent(QMouseEvent *mouseEvent) {
    manipulator->grabBegin(mouseEvent->x(), -mouseEvent->y(), false);
};
void FilamentWindow::mouseReleaseEvent(QMouseEvent *mouseEvent) {
    manipulator->grabEnd();
};
void FilamentWindow::mouseMoveEvent(QMouseEvent *mouseEvent) {
    manipulator->grabUpdate(mouseEvent->x(), -mouseEvent->y());
    updateFrame();
};