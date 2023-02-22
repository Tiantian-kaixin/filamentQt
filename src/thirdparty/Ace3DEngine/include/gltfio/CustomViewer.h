#ifndef GLTFIO_CUSTOMVIEWER_H
#define GLTFIO_CUSTOMVIEWER_H

#include <filament/Box.h>
#include <filament/DebugRegistry.h>
#include <filament/Engine.h>
#include <filament/IndirectLight.h>
#include <filament/Scene.h>
#include <filament/View.h>

#include <gltfio/Animator.h>
#include <gltfio/FilamentAsset.h>

#include <utils/Entity.h>

#include <math/vec3.h>
#include <math/vec2.h>
#include <gltfio/SimpleViewer.h>
#include <gltfio/FilamentFilter.h>
#include <filament/LightManager.h>

LIGHT3D_NAMESPACE_BEGIN
using namespace gltfio;

filament::math::mat4f fitIntoUnitCube(const filament::Box& bounds) {
    using namespace filament::math;
    auto minpt = bounds.getMin();
    auto maxpt = bounds.getMax();
    float maxExtent = 0;
    maxExtent = std::max(maxpt.x - minpt.x, maxpt.y - minpt.y);
    maxExtent = std::max(maxExtent, maxpt.z - minpt.z);
    float scaleFactor = 2.0f / maxExtent;
    float3 center = (minpt + maxpt) / 2.0f;
    center.z += 4.0f / scaleFactor;
    return mat4f::scaling(float3(scaleFactor)) * mat4f::translation(-center);
}
class CustomViewer : public SimpleViewer {
public:
    static constexpr int DEFAULT_SIDEBAR_WIDTH = 350;

    CustomViewer(filament::Engine* engine, filament::Scene* scene, filament::View* view,
        int sidebarWidth = DEFAULT_SIDEBAR_WIDTH);

    ~CustomViewer();

    void applyAnimation(double currentTime);
    void updateUserInterface();
    void addFur(std::vector<TTNodeModel*> nodeModel);
    void addAssets(std::vector<FilamentAsset*> assetList, bool scale);
    void addStickerAsset(std::vector<utils::Entity> entities, bool scale);
    void setSunlightColor(filament::math::float3 color);
    void setSunlightDirection(filament::math::float3 direction);
    void setSunlightIntensity(int intensity);
    void setReflectConfig(std::vector<std::vector<TTReflectConfig*>> config) {
        if (config.size() <= 0) {
            return;
        }
        reflectConfig = config;
    }

    void setIntensityKeynValue(float* key, float* value) {
        if (key != nullptr && value != nullptr) {
            intensityKey[0] = key[0];
            intensityKey[1] = key[1];
            intensityValue[0] = value[0];
            intensityValue[1] = value[1];
        }
    }

    void setIntensityMap(std::vector<float> key, std::vector<float> value) {
        if (key.size() > 1 && value.size() > 1) {
            intensityKey[0] = key[0];
            intensityKey[1] = key[1];
            intensityValue[0] = value[0];
            intensityValue[1] = value[1];
        }
    }
    void setEnableShadow(bool enable) { mEnableShadows = enable; }
    void setIBLRotation(float rotation) { mIblRotation = rotation; }
    bool getUnlit() { return mEnableUnlit; }
    int getIBLIntensity() { return mIblIntensity; }
    float getIblRotation() { return mIblRotation; }
    filament::math::float2 getIntensityKey() { return intensityKey; }
    filament::math::float2 getIntensityValue() { return intensityValue; }
    filament::math::float3 getGlbScale() { return glbScale; }
    filament::math::float3 getGlbRotation() { return glbRotate; }
    filament::math::float3 getGlbTranslate() { return glbTranslate; }
    filament::math::float3 getSunlightDirection() { return mSunlightDirection; }
    std::vector<std::vector<TTReflectConfig*>> getReflectConfigList() {
        return reflectConfig;
    }
    bool isHideUserHead() { return hideUserHead; }
    bool isDebugCustomShader() { return debug_custom_shader; }
    void setLightParam(TTMaterialLightParam* light) {
        if (light == nullptr) {
            lightParam = new TTMaterialLightParam();
        } else {
            lightParam = light;
        }
        mEnableShadows = lightParam->castShadow;
        mSunlightIntensity = lightParam->intensity;
        mSunlightColor = lightParam->color;
        mSunlightDirection = lightParam->direction;
    }

    TTMaterialLightParam* getLightParam() {
        lightParam->castShadow = mEnableShadows;
        lightParam->intensity = mEnableSunlight ? mSunlightIntensity : 0;
        lightParam->color = mSunlightColor;
        lightParam->direction = mSunlightDirection;
        return lightParam;
    }

    void setCustomShaderList(std::vector<TTCustomShader*> customShaderList) {
        this->customShaderList = customShaderList;
    }

    void setGlbTransform(filament::math::float3 scale, filament::math::float3 rotation, filament::math::float3 translate)
    {
        glbScale[0] = scale[0];
        glbScale[1] = scale[1];
        glbScale[2] = scale[2];
        glbRotate[0] = rotation[0];
        glbRotate[1] = rotation[1];
        glbRotate[2] = rotation[2];
        glbTranslate[0] = translate[0];
        glbTranslate[1] = translate[1];
        glbTranslate[2] = translate[2];
    }
    TTMaterialLightParam* lightParam;
    float averageL = 60.f;
    filament::math::float2 intensityKey = { 30.f, 80.f };
    filament::math::float2 intensityValue = { 20000.f, 80000.f };
    filament::math::float2 mReflectOffset = filament::math::float2(0);
    bool saveParamsNow = false;

    int getSelectedAnimation();
    float getAnimationTestDegreeXY();
    float getAnimationTestDegreeZ();

    int getBinIndex() {
        return mCurrentBin;
    }

private:
    bool is_initialized = false;
    const utils::Entity mPointlight;
    std::vector<TTNodeModel*> mFurList;
    std::vector<FilamentAsset*> mAssetList;
    std::vector<Animator*> mAnimatorList;
    float mMorphWeights = 0.0f;
    float mPointlightIntensity = 10000.0f;
    float mPointlightEfficiency = 5.0f;
    filament::math::float3 mPointlightPosition = { 0.0f, -0.15f, -4.0f };
    filament::math::float3 mPointlightRGB = { 1.0f, 1.0f, 0.0f };
    std::vector<std::vector<TTReflectConfig*>> reflectConfig;
    std::vector<TTCustomShader*> customShaderList;
    filament::LightManager::ShadowOptions shadowOptions{};
    bool mEnablePointlight = false;
    bool mAces = true;
    bool mEnableUnlit = false;
    filament::math::float3 glbScale = { 1.f, 1.f, 1.f };
    filament::math::float3 glbRotate = { 0.f, 0.f, 0.f };
    filament::math::float3 glbTranslate = { 0.f, 0.f, 0.f };
    bool hideUserHead = true;
    bool debug_custom_shader = true;

    float mAnimationTestDegreeXY = 0;
    float mAnimationTestDegreeZ = 0;
    int mCurrentBin = 0;
};
LIGHT3D_NAMESPACE_END

#ifdef GLTFIO_SIMPLEVIEWER_IMPLEMENTATION

#include <filament/RenderableManager.h>
#include <filament/TransformManager.h>
#include <filament/LightManager.h>
#include <filament/Material.h>

#include <utils/EntityManager.h>

#include <math/mat4.h>
#include <math/vec3.h>

#include <imgui.h>
#include <filagui/ImGuiExtensions.h>

#include <vector>

LIGHT3D_NAMESPACE_BEGIN
using namespace gltfio;

CustomViewer::CustomViewer(filament::Engine* engine, filament::Scene* scene, filament::View* view,
    int sidebarWidth) :
    SimpleViewer(engine, scene, view, sidebarWidth),
    mPointlight(utils::EntityManager::get().create()) {
    filament::LightManager::Builder(filament::LightManager::Type::SPOT)
        .color(mPointlightRGB)
        .intensity(mPointlightIntensity, mPointlightEfficiency)
        .position(mPointlightIntensity)
        .falloff(4.0f)
        .build(*engine, mPointlight);
    if (mEnablePointlight) {
        mScene->addEntity(mPointlight);
    }
}

CustomViewer::~CustomViewer() {
    mEngine->destroy(mPointlight);
}

void CustomViewer::addFur(std::vector<TTNodeModel*> nodeModel) {
    mFurList.assign(nodeModel.begin(), nodeModel.end());
}

void CustomViewer::addAssets(std::vector<FilamentAsset*> assetList, bool scale) {
    using namespace filament::math;
    mAssetList = assetList;
    transform(begin(assetList), end(assetList), back_inserter(mAnimatorList), [](auto asset) {return asset->getAnimator(); });
    mat4f transform = mat4f(gltfio::fitIntoUnitCube(mAssetList[0]->getBoundingBox()));
    if (!scale) return;
    auto& tcm = mEngine->getTransformManager();
    for (auto asset : assetList) {
        auto root = tcm.getInstance(asset->getRoot());
        tcm.setTransform(root, transform);
    }
}

void CustomViewer::addStickerAsset(std::vector<utils::Entity> entities, bool scale) {
    if (!scale || entities.empty()) {
        return;
    }
    auto& tcm = mEngine->getTransformManager();
    auto& rm = mEngine->getRenderableManager();
    const filament::Box& boundingBox = rm.getAxisAlignedBoundingBox(rm.getInstance(entities[0]));
    auto transform = filament::math::mat4f(fitIntoUnitCube(boundingBox));
    for (auto entity : entities) {
        auto root = tcm.getInstance(entity);
        tcm.setTransform(root, transform);
    }
}

void CustomViewer::setSunlightColor(filament::math::float3 color) {
    mSunlightColor = color;
}
void CustomViewer::setSunlightDirection(filament::math::float3 direction) {
    mSunlightDirection = direction;
}
void CustomViewer::setSunlightIntensity(int intensity) {
    mSunlightIntensity = intensity;
}

void CustomViewer::applyAnimation(double currentTime) {
    static double startTime = 0;
    for (size_t i = 0; i < mAnimatorList.size(); i++) {
        auto mAnimator = mAnimatorList[i];
        if (mAnimator == nullptr) {
            continue;
        }

        const size_t numAnimations = mAnimator->getAnimationCount();
        if (mResetAnimation) {
            startTime = currentTime;
            for (size_t i = 0; i < numAnimations; i++) {
                mAnimator->applyAnimation(i, 0);
            }
            mResetAnimation = false;
        }
        if (numAnimations > 0 && mCurrentAnimation > 0) {
            mAnimator->applyAnimation(mCurrentAnimation - 1, currentTime - startTime);
        }
        mAnimator->updateBoneMatrices();
    }
}

void CustomViewer::updateUserInterface() {
    using namespace filament;

    auto& tm = mEngine->getTransformManager();
    auto& rm = mEngine->getRenderableManager();
    auto& lm = mEngine->getLightManager();

    // Show a common set of UI widgets for all renderables.
    if (!is_initialized) {
        is_initialized = true;
        ImGui::SetColorEditOptions(ImGuiColorEditFlags_Float);
    }
    auto renderableTreeItem = [this, &rm](utils::Entity entity) {
        bool rvis = mScene->hasEntity(entity);
        ImGui::Checkbox("visible", &rvis);
        if (rvis) {
            mScene->addEntity(entity);
        } else {
            mScene->remove(entity);
        }
        auto instance = rm.getInstance(entity);
        bool scaster = rm.isShadowCaster(instance);
        ImGui::Checkbox("casts shadows", &scaster);
        rm.setCastShadows(instance, scaster);
        size_t numPrims = rm.getPrimitiveCount(instance);
        for (size_t prim = 0; prim < numPrims; ++prim) {
            const char* mname = rm.getMaterialInstanceAt(instance, prim)->getName();
            if (mname) {
                ImGui::Text("prim %zu: material %s", prim, mname);
            } else {
                ImGui::Text("prim %zu: (unnamed material)", prim);
            }
        }
    };

    auto lightTreeItem = [this, &lm](utils::Entity entity) {
        bool lvis = mScene->hasEntity(entity);
        ImGui::Checkbox("visible", &lvis);

        if (lvis) {
            mScene->addEntity(entity);
        } else {
            mScene->remove(entity);
        }

        auto instance = lm.getInstance(entity);
        bool lcaster = lm.isShadowCaster(instance);
        ImGui::Checkbox("shadow caster", &lcaster);
        lm.setShadowCaster(instance, lcaster);
    };

    // Declare a std::function for tree nodes, it's an easy way to make a recursive lambda.
    std::function<void(utils::Entity)> treeNode;

    treeNode = [&](utils::Entity entity) {
        auto tinstance = tm.getInstance(entity);
        auto rinstance = rm.getInstance(entity);
        auto linstance = lm.getInstance(entity);
        intptr_t treeNodeId = 1 + entity.getId();

        //const char* name = mAsset->getName(entity); // AceEngine modified
        const char* name = mAssetList[0]->getName(entity); // AceEngine added
        auto getLabel = [&name, &rinstance, &linstance]() {
            if (name) {
                return name;
            }
            if (rinstance) {
                return "Mesh";
            }
            if (linstance) {
                return "Light";
            }
            return "Node";
        };
        const char* label = getLabel();

        ImGuiTreeNodeFlags flags = 0; // rinstance ? 0 : ImGuiTreeNodeFlags_DefaultOpen;
        std::vector<utils::Entity> children(tm.getChildCount(tinstance));
        if (ImGui::TreeNodeEx((const void*)treeNodeId, flags, "%s", label)) {
            if (rinstance) {
                renderableTreeItem(entity);
            }
            if (linstance) {
                lightTreeItem(entity);
            }
            tm.getChildren(tinstance, children.data(), children.size());
            for (auto ce : children) {
                treeNode(ce);
            }
            ImGui::TreePop();
        }
    };

    // AceEngine added
    auto animationsTreeItem = [&]() {
        for (size_t i = 0; i < mAnimatorList.size(); i++) {
            auto mAnimator = mAnimatorList[i];
            size_t count = mAnimator->getAnimationCount();
            int selectedAnimation = mCurrentAnimation;
            ImGui::RadioButton("Disable", &selectedAnimation, 0);
            for (size_t i = 0; i < count; ++i) {
                std::string label = mAnimator->getAnimationName(i);
                if (label.empty()) {
                    label = "Unnamed " + std::to_string(i);
                }
                ImGui::RadioButton(label.c_str(), &selectedAnimation, i + 1);
            }
            if (selectedAnimation != mCurrentAnimation) {
                mCurrentAnimation = selectedAnimation;
                mResetAnimation = true;
            }
        }
    };

    // Disable rounding and draw a fixed-height ImGui window that looks like a sidebar.
    ImGui::GetStyle().WindowRounding = 0;
    ImGui::SetNextWindowPos(ImVec2(0, 0));

    const float width = ImGui::GetIO().DisplaySize.x;
    const float height = ImGui::GetIO().DisplaySize.y;
    ImGui::SetNextWindowSize(ImVec2(mSidebarWidth, height), ImGuiCond_Once);
    ImGui::SetNextWindowSizeConstraints(ImVec2(20, height), ImVec2(width, height));

    ImGui::Begin("Filament", nullptr, ImGuiWindowFlags_NoTitleBar);
    if (mCustomUI) {
        mCustomUI();
    }

    DebugRegistry& debug = mEngine->getDebugRegistry();

    if (ImGui::CollapsingHeader("View")) {
        ImGui::Indent();
        ImGui::Checkbox("Dithering", &mEnableDithering);
        ImGui::Checkbox("FXAA", &mEnableFxaa);
        ImGui::Checkbox("MSAA 4x", &mEnableMsaa);
        ImGui::Checkbox("SSAO", &mEnableSsao);
        ImGui::Checkbox("ACES", &mAces); // AceEngine added
        ImGui::Checkbox("Unlit( Please reload asset )", &mEnableUnlit); // AceEngine added
        ImGui::Checkbox("Bloom", &mBloomOptions.enabled);
        ImGui::Unindent();
    }
    // AceEngine added begin
    if (ImGui::Button("Prev Bin")) {
        mCurrentBin--;
    }
    if (ImGui::Button("Next Bin")) {
        mCurrentBin++;
    }
    mView->setDithering(mEnableDithering ? View::Dithering::TEMPORAL : View::Dithering::NONE);
    mView->setAntiAliasing(mEnableFxaa ? View::AntiAliasing::FXAA : View::AntiAliasing::NONE);
    mView->setSampleCount(mEnableMsaa ? 4 : 1);
    mView->setAmbientOcclusion(
        mEnableSsao ? View::AmbientOcclusion::SSAO : View::AmbientOcclusion::NONE);
    mView->setToneMapping(mAces ? View::ToneMapping::ACES : View::ToneMapping::LINEAR);

    if (ImGui::CollapsingHeader("Mock head")) {
        ImGui::Checkbox("Hide User Head", &hideUserHead);
    }
    // AceEngine added end

    if (ImGui::CollapsingHeader("Light")) {
        ImGui::Indent();
        ImGui::SliderFloat("IBL intensity", &mIblIntensity, 0.0f, 100000.0f);
        ImGui::SliderAngle("IBL rotation", &mIblRotation);
        // ImGui::SliderFloat("Sun intensity", &mSunlightIntensity, 50000.0, 150000.0f); // Filament own codes
        ImGuiExt::DirectionWidget("Sun direction", mSunlightDirection.v);
        ImGui::Checkbox("Enable sunlight", &mEnableSunlight);
        ImGui::Checkbox("Enable shadows", &mEnableShadows);
        // AceEngine added begin
        ImGui::SliderFloat("Morph", &mMorphWeights, 0.0f, 1.0f);
        ImGui::ColorPicker3("Sun color", &mSunlightColor.x);
        ImGui::SliderFloat("Sun intensity", &mSunlightIntensity, 00000.0, 150000.0f);
        if (mEnableShadows) {
            ImGui::SliderInt("Shadow Map Size", reinterpret_cast<int*>(&shadowOptions.mapSize), 1, 2048);
        }
        ImGui::SliderFloat("Point intensity", &mPointlightIntensity, 0.0f, 10000.0f);
        ImGui::SliderFloat("Point efficiency", &mPointlightEfficiency, 0.0f, 20.0f);
        ImGui::SliderFloat("Point positionX", &mPointlightPosition.x, -10.0f, 10.0f);
        ImGui::SliderFloat("Point positionY", &mPointlightPosition.y, -10.0f, 10.0f);
        ImGui::SliderFloat("Point positionZ", &mPointlightPosition.z, -10.0f, 10.0f);
        ImGui::ColorPicker3("Point color", &mPointlightRGB.x);
        ImGui::Checkbox("Enable pointlight", &mEnablePointlight);
        // AceEngine added end
        ImGui::Unindent();
    }

    //AceEngine added begin
    if (ImGui::CollapsingHeader("Glb Transform")) {
        // ImGui::Checkbox("Filled Into Unit Cube", &isFilledInToCube);
        ImGui::SliderFloat3("Position", glbTranslate.v, -10.f, 10.f, "%.2f");
        ImGui::InputFloat3("Input position", glbTranslate.v, 3);
        ImGui::SliderAngle("Rotate X", &glbRotate[0]);
        ImGui::SliderAngle("Rotate Y", &glbRotate[1]);
        ImGui::SliderAngle("Rotate Z", &glbRotate[2]);
        float* rotate = new float[3]{ (float)(glbRotate[0] * 360.0f / (2 * M_PI)), (float)(glbRotate[1] * 360.0f / (2 * M_PI)),
                                      (float)(glbRotate[2] * 360.0f / (2 * M_PI)) };
        ImGui::InputFloat3("Input rotate", rotate, 2);
        glbRotate[0] = rotate[0] * (2 * M_PI) / 360.0f;
        glbRotate[1] = rotate[1] * (2 * M_PI) / 360.0f;
        glbRotate[2] = rotate[2] * (2 * M_PI) / 360.0f;
        delete[] rotate;
        ImGui::SliderFloat3("Scale", glbScale.v, 0.0f, 100.0f, "%.2f");
        ImGui::InputFloat3("Input scale", glbScale.v, 3);
    }

    int refCount = 0;
    for (size_t i = 0; i < reflectConfig.size(); i++) {
        for (size_t j = 0; j < reflectConfig[i].size(); j++) {
            TTReflectConfig* config = reflectConfig[i][j];
            if (ImGui::CollapsingHeader((std::string("Reflection ") + config->nodeName).c_str())) {
                ImGui::SliderFloat((std::string("Z normal scale ") + std::to_string(refCount)).c_str(),
                    &config->zNormalScale, 0.0f, 10.0f);
                ImGui::SliderFloat((std::string("Blend Factor ") + std::to_string(refCount)).c_str(),
                    &config->blendFactor, 0.0f, 1.0f);
                ImGui::SliderFloat2((std::string("Reflect Offset ") + std::to_string(refCount)).c_str(),
                    &config->reflectOffset.r, -1, 1);
                ImGui::SliderFloat((std::string("Modulate Alpha ") + std::to_string(refCount)).c_str(),
                    &config->modulateAlpha, 0.0f, 1.0f);
                refCount++;
            }
        }
    }

    for (auto config : customShaderList) {
        auto nodeName = config->nodeNameList.empty() ? "default" : config->nodeNameList[0];
        if (ImGui::CollapsingHeader((std::string("CustomShader ") + nodeName).c_str())) {
            ImGui::Checkbox("debug custom shader", &debug_custom_shader);
            for (TTCustomUniformParam* param : config->parameterVec) {
                float min = param->value_range_[0];
                float max = param->value_range_[1];
                auto [data_ptr, float_size] = param->GetFloatPointer();
                if (float_size == 0) {
                    continue;
                } else if (float_size == 1) {
                    ImGui::SliderFloat(param->param_name_.c_str(), data_ptr, min, max);
                } else if (float_size == 3) {
                    if (param->value_type_ == TTCustomParamValueType::Color) {
                        ImGui::ColorPicker3(param->param_name_.c_str(), data_ptr);
                    } else if (param->value_type_ == TTCustomParamValueType::Direction) {
                        ImGuiExt::DirectionWidget(param->param_name_.c_str(), data_ptr);
                    } else {
                        ImGui::SliderFloat3(param->param_name_.c_str(), data_ptr, min, max);
                    }
                } else if (float_size == 4) {
                    if (param->value_type_ == TTCustomParamValueType::Color) {
                        ImGui::ColorPicker4(param->param_name_.c_str(), data_ptr);
                    } else {
                        ImGui::SliderFloat4(param->param_name_.c_str(), data_ptr, min, max);
                    }
                }
            }
        }
    }
    //AceEngine added ended

    mView->setDithering(mEnableDithering ? View::Dithering::TEMPORAL : View::Dithering::NONE);
    mView->setAntiAliasing(mEnableFxaa ? View::AntiAliasing::FXAA : View::AntiAliasing::NONE);
    mView->setSampleCount(mEnableMsaa ? 4 : 1);
    mView->setAmbientOcclusion(
        mEnableSsao ? View::AmbientOcclusion::SSAO : View::AmbientOcclusion::NONE);
//    mView->setBloomOptions(mBloomOptions);

    if (mEnableSunlight) {
        mScene->addEntity(mSunlight);
        auto sun = lm.getInstance(mSunlight);
        lm.setIntensity(sun, mSunlightIntensity);
        lm.setDirection(sun, normalize(mSunlightDirection));
        lm.setColor(sun, mSunlightColor);
        lm.setShadowCaster(sun, mEnableShadows);
        lm.setShadowOptions(sun, shadowOptions); // AceEngine added
    } else {
        mScene->remove(mSunlight);
    }

    // AceEngine added begin
    if (mEnablePointlight) {
        mScene->addEntity(mPointlight);
        auto point = lm.getInstance(mPointlight);
        lm.setPosition(point, mPointlightPosition);
        lm.setIntensity(point, mPointlightIntensity, mPointlightEfficiency);
        lm.setColor(point, mPointlightRGB);
    } else {
        mScene->remove(mPointlight);
    }
    // AceEngine added end

    lm.forEachComponent([this, &lm](utils::Entity e, LightManager::Instance ci) {
        auto options = lm.getShadowOptions(ci);
        options.screenSpaceContactShadows = mEnableContactShadows;
        options.shadowCascades = mShadowCascades;
        lm.setShadowOptions(ci, options);
        lm.setShadowCaster(ci, mEnableShadows);
        });

    // AceEngine added begin
    for (size_t i = 0; i < mFurList.size(); i++) {
        auto pNodeModel = mFurList[i];
        if (pNodeModel != nullptr && pNodeModel->furParam != nullptr) {
            if (ImGui::CollapsingHeader((std::string("Fur ") + pNodeModel->name).c_str())) {
                ImGui::SliderFloat((std::string("length") + std::to_string(i)).c_str(), &pNodeModel->furParam->length, 0.0f, 10.0f);
                ImGui::SliderFloat((std::string("tming") + std::to_string(i)).c_str(), &pNodeModel->furParam->edgeSoftness, 0.0f, 3.0f);
                ImGui::SliderFloat((std::string("dming") + std::to_string(i)).c_str(), &pNodeModel->furParam->topEndAlpha, 0.0f, 1.0f);

                if (pNodeModel->furParam->shadingModel == Shading::LIT) {
                    ImGui::SliderFloat((std::string("roughness") + std::to_string(i)).c_str(), &pNodeModel->furParam->roughness, 0.0f, 1.0f);
                    ImGui::SliderFloat((std::string("reflectance") + std::to_string(i)).c_str(), &pNodeModel->furParam->reflectance, 0.0f, 1.0f);
                    ImGui::SliderFloat((std::string("anisotropy") + std::to_string(i)).c_str(), &pNodeModel->furParam->anisotropy, -1.0f, 1.0f);
                    ImGuiExt::DirectionWidget((std::string("AnisoDir") + std::to_string(i)).c_str(), pNodeModel->furParam->anisotropyDirection.v);

                    ImGui::InputFloat4((std::string("gravity") + std::to_string(i)).c_str(), pNodeModel->furParam->gravity.v);
                    ImGui::InputFloat4((std::string("wind") + std::to_string(i)).c_str(), pNodeModel->furParam->wind.v);
                    ImGui::ColorPicker3("shadow color", pNodeModel->furParam->shadowColor.v);
                    ImGui::SliderFloat4((std::string("uvOffset") + std::to_string(i)).c_str(), pNodeModel->furParam->uvOffset.v, -1.0f, 1.0f);

                } else {
                    ImGui::SliderFloat("spacing", &pNodeModel->furParam->spacing, 0.0f, 1.0f);
                    ImGui::InputFloat4((std::string("gravity") + std::to_string(i)).c_str(), pNodeModel->furParam->gravity.v);
                    ImGui::InputFloat4((std::string("wind") + std::to_string(i)).c_str(), pNodeModel->furParam->wind.v);
                }

            }
        }
    }

    if (ImGui::CollapsingHeader("Model")) {
        ImGui::Indent();
        ImGui::SliderFloat("Animation degree XY", &mAnimationTestDegreeXY, 0.0f, 360);
        ImGui::SliderFloat("Animation degree Z", &mAnimationTestDegreeZ, 0.0f, 90);
        ImGui::Checkbox("Show bounds", &mEnableWireframe);
        for (size_t i = 0; i < mAssetList.size(); i++) {
            auto mAsset = mAssetList[i];
            if (mAsset == nullptr) {
                continue;
            }
            if (mAnimatorList[i] != nullptr && mAnimatorList[i]->getAnimationCount() > 0) {
                intptr_t animationsNodeId = -1;
                if (ImGui::TreeNodeEx((const void*)animationsNodeId, ImGuiTreeNodeFlags_DefaultOpen, "Animations")) {
                    animationsTreeItem();
                    ImGui::TreePop();
                }
            }
            treeNode(mAsset->getRoot());
            if (mEnableWireframe) {
                mScene->addEntity(mAsset->getWireframe());
            } else {
                mScene->remove(mAsset->getWireframe());
            }
        }
        ImGui::Unindent();
    }

    if (ImGui::CollapsingHeader("Save params", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Indent();
        const ImVec2& size = ImVec2(180, 40);
        if (ImGui::Button("Save Params To JSON", size)) {
            saveParamsNow = true;
        }
        ImGui::Unindent();
    }
    // AceEngine added end

    mSidebarWidth = ImGui::GetWindowWidth();
    ImGui::End();

    updateIndirectLight();
}

int CustomViewer::getSelectedAnimation() {
    return mCurrentAnimation - 1;
}

float CustomViewer::getAnimationTestDegreeXY() {
    return mAnimationTestDegreeXY;
}

float CustomViewer::getAnimationTestDegreeZ() {
    return mAnimationTestDegreeZ;
}

LIGHT3D_NAMESPACE_END

#endif // GLTFIO_SIMPLEVIEWER_IMPLEMENTATION

#endif // GLTFIO_CUSTOMVIEWER_H
