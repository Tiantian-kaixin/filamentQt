//
// Created by TIANTIAN on 2023/2/25.
//

#include "RenderThread.h"
static constexpr uint8_t BAKED_COLOR_PACKAGE[] = {
#include "../asset/material/bakedColor.inc"
};
RenderThread::RenderThread(): isInitialized(false), curFrame(0) {
}

RenderThread::~RenderThread() {

}

void RenderThread::initialize() {
    engine = filament::Engine::create();
    render = engine->createRenderer();
//    swapChain = engine->createSwapChain((void*)renderTargetID);
    camera = engine->createCamera(utils::EntityManager::get().create());
    view = engine->createView();
    scene = engine->createScene();
    manipulator = filament::camutils::Manipulator<float>::Builder()
            .viewport(texturesWidth, texturesHeight)
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
    view->setViewport(filament::Viewport(0, 0, texturesWidth, texturesHeight));
    updateRenderTarget(renderTargetID);
    loadModelGlb("/Users/tiantian/Downloads/LvBu.glb");
    loadLight("/Users/tiantian/Downloads/default_env_ibl.ktx");
    isInitialized = true;
}

void RenderThread::updateRenderTarget(intptr_t textureID) {
    swapChain = engine->createSwapChain(texturesWidth, texturesHeight, filament::SwapChain::CONFIG_TRANSPARENT);
    auto colorTexture = filament::Texture::Builder()
            .width(texturesWidth)
            .height(texturesHeight)
            .levels(1)
            .usage(filament::Texture::Usage::COLOR_ATTACHMENT | filament::Texture::Usage::SAMPLEABLE)
            .format(filament::Texture::InternalFormat::RGBA8)
            .import(textureID)
            .build(*engine);
    auto depthTexture = filament::Texture::Builder()
            .width(texturesWidth)
            .height(texturesHeight)
            .levels(1)
            .usage(filament::Texture::Usage::DEPTH_ATTACHMENT)
            .format(filament::Texture::InternalFormat::DEPTH24)
            .build(*engine);
    auto renderTarget = filament::RenderTarget::Builder()
            .texture(filament::RenderTarget::AttachmentPoint::COLOR, colorTexture)
            .texture(filament::RenderTarget::AttachmentPoint::DEPTH, depthTexture)
            .build(*engine);
    view->setRenderTarget(renderTarget);
}

void RenderThread::loadModelGlb(std::string filename) {
    // readfile
    uint64_t size;
    std::vector<uint8_t> buffer = loadFileBuffer(filename, size);
    filament::Material* material = filament::Material::Builder().package((void*) BAKED_COLOR_PACKAGE, sizeof(BAKED_COLOR_PACKAGE))
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

void RenderThread::loadLight(std::string filename) {
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

void RenderThread::updateFrame() {
    filament::math::float3 eye, center, up;
    manipulator->getLookAt(&eye, &center, &up);
    camera->lookAt(eye, center, up);
    if (render->beginFrame(swapChain)) {
        qDebug() << "updateFrame" << endl;
        render->render(view);
        render->endFrame();
        emit textureReady(renderTargetID, *new QSize(texturesWidth, texturesHeight));
    }
}

void RenderThread::renderNext() {
    updateFrame();
    sleep(0.5);
    emit textureReady(renderTargetID, *new QSize(texturesWidth, texturesHeight));
}

std::vector<uint8_t> RenderThread::loadFileBuffer(std::string filename, uint64_t& size) {
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

void RenderThread::mouseEvent(float x, float y, int state) {
    if (state == 0) {
        manipulator->grabBegin(x, -y, false);
    }else if (state == 1) {
        manipulator->grabUpdate(x, -y);
    } else if (state == 2) {
        manipulator->grabEnd();
    }
};

void RenderThread::run() {

}