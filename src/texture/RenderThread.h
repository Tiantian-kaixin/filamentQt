//
// Created by TIANTIAN on 2023/2/25.
//

#ifndef FILAMENTQT_RENDERTHREAD_H
#define FILAMENTQT_RENDERTHREAD_H
#include "filament/Engine.h"
#include "filament/Camera.h"
#include "filament/Scene.h"
#include "filament/View.h"
#include "filament/Renderer.h"
#include "filament/SwapChain.h"
#include "utils/EntityManager.h"
#include "filament/RenderableManager.h"
#include "filament/Material.h"
#include "filament/VertexBuffer.h"
#include "filament/IndexBuffer.h"
#include "filament/RenderTarget.h"
#include "filament/Viewport.h"
#include "filament/TransformManager.h"
#include "filament/IndirectLight.h"
#include "camutils/Manipulator.h"
#include "utils/NameComponentManager.h"
#include "gltfio/AssetLoader.h"
#include "gltfio/ResourceLoader.h"
#include "gltfio/MaterialProvider.h"
#include "image/KtxBundle.h"
#include "image/KtxUtility.h"
#include <fstream>
#include <QDebug>
#include <QQuickWindow>
#include <QJsonArray>
#include "QThread"

class RenderThread: public QThread {
Q_OBJECT
public:
    RenderThread();
    ~RenderThread();
    void initialize();
    void doFree();
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    Q_SIGNAL void textureReady(int id, const QSize& size);
    Q_SLOT void renderNext();
    intptr_t renderTargetID;
    bool isInitialized;
protected:
    void run() override;
private:
    filament::Engine* engine;
    filament::SwapChain* swapChain;
    filament::Renderer* render;
    filament::Camera* camera;
    filament::View* view;
    filament::Scene* scene;
    utils::Entity renderable;
    filament::MaterialInstance* materialInstance;
    filament::camutils::Manipulator<float>* manipulator;
    int curFrame;

    void updateFrame();
    void updateRotation();
    void loadShader();
    void loadModelGlb(std::string filename);
    void loadLight(std::string filename);
    std::vector<uint8_t> loadFileBuffer(std::string filename, uint64_t& size);
    void updateRenderTarget(intptr_t textureID);
};

#endif //FILAMENTQT_RENDERTHREAD_H
