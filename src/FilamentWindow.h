//
// Created by TIANTIAN on 2023/2/14.
//
#pragma once

#ifndef NODEGRAPH_FILAMENTWINDOW_H
#define NODEGRAPH_FILAMENTWINDOW_H
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
#include "QWindow"
#include <QDebug>

class FilamentWindow : public QWindow
{
    Q_OBJECT
public:
    FilamentWindow();
    ~FilamentWindow();
    Q_INVOKABLE void updateFrame();

protected:
    void doInitialize();
    void doFree();

private:
    filament::Engine* engine;
    filament::SwapChain* swapChain;
    filament::Renderer* render;
    filament::Camera* camera;
    filament::View* view;
    filament::Scene* scene;
};

#endif //NODEGRAPH_FILAMENTWINDOW_H
