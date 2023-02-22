/*
 * Copyright (C) 2019 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define METAL_AVAILABLE __has_include(<QuartzCore/CAMetalLayer.h>)
#if METAL_AVAILABLE

#include <TargetConditionals.h>

#else
//#error The iOS simulator does not support Metal.
#endif

#ifndef FilamentFilter_h
#define FilamentFilter_h

// 宏定义放在头文件最前面，确保对以下头文件都生效
#define FILAMENT_PARTICLE_ENABL 0

#include <filament/Engine.h>
#include <filament/IndirectLight.h>
#include <filament/RenderableManager.h>
#include <filament/RenderTarget.h>
#include <filament/Scene.h>
#include <filament/Skybox.h>
#include <filament/TransformManager.h>
#include <filament/MaterialInstance.h>
#include <gltfio/FilamentDynamicBone.h>
#include <filament/View.h>
#include <gltfio/Animator.h>
#include <gltfio/TextureManager.h>
#include <gltfio/MaterialManager.h>
#include <gltfio/IblManager.h>

#include <utils/Path.h>
#include <gltfio/gl_util.h>
#include <gltfio/ModelDef.h>

#include <gltfio/AssetHelper.h>
#include <gltfio/AssetHelperKapu.h>
#include <gltfio/HeadHelper.h>
#include <gltfio/DynamicBoneHelper.h>
#include <gltfio/CustomShaderComponent.h>
#include <gltfio/AnimojiManager.h>
#include <gltfio/StickerComponent.h>
#include <gltfio/AESticker2DComponent.h>
#include <utils/EasingFunction.h>
#include <loader/Loader.h>

#if (FILAMENT_ENV_IOS || (IOS))
#define IS_IOS 1
#define ENABLE_3D_TOOL_IOS 1
#include <gltfio/stb_image_android.h>
#else

#include <utils/Log.h>
#include <gltfio/stb_image_android.h>

#  define IS_IOS 0
#endif

#include "NormalCubemapComponent.h"
#include "string_util.h"

#include <string>
#include <map>

LIGHT3D_NAMESPACE_BEGIN

    class FilamentConfig {
    public:
        filament::Engine *mEngine;
        filament::Scene *mScene;
        filament::View *mView;

        FilamentConfig(filament::Engine *engine, filament::View *view, filament::Scene *scene) {
            mEngine = engine;
            mView = view;
            mScene = scene;
        }
    };

#define GY3D_FILAMENT_IOS_BACKEND_METAL     0  // 是否默认Metal渲染 (必须在iOS、mac环境下判断该宏)

    class FilamentFilter {
        using TransformFunc = std::function<filament::math::mat4f(filament::math::float3 &translation,
                                                                  filament::math::quatf &rotation,
                                                                  filament::math::float3 &scale)>;

    public:
        // Constructor for MAC. As `engine`, `view`, `scene` is created by clients. This only has a weak
        // connection.
        FilamentFilter(FilamentConfig &config, TTFilamentInitParam &initParam);

        // Constructor for IOS and Android. `engine`, `view`, `scene` is created by us. We are
        // responsible for release these resources.
        //
        FilamentFilter(void *native_layer, void *sharedGLContext, void *platform,
                       TTFilamentInitParam &initParam, TTFilamentSetupParam *setupParam = nullptr,
                       TTFilamentMode mode = TTFilamentMode::LIGHT_STUDIO_MODE,
                       filament::Engine* sharedEngine = nullptr);

        ~FilamentFilter();

        FilamentFilter(const FilamentFilter &) = delete;

        FilamentFilter &operator=(const FilamentFilter &) = delete;

        // 初始化方法
        // iOS会准备好TTFilamentSetupParam，传进来，Android需要在里面自己解析出TTFilamentSetupParam
        void init(TTFilamentInitParam &initParam);

        // must be called before init to be effective
        void setDeviceCapability(DeviceCapability capability);

        void initNormal(TTFilamentInitParam &initParam);

        void initKapu(TTFilamentInitParam &initParam);

        //适配不使用FilmentFilter的场景
        static void setResourcePath(const std::string& path);

        // 加载所有素材指定的数据
        void loadAllData();

        // C++层没有解密GLB的能力，所以遇到加密的GLB需要在上层做好解密，然后通过glbPath名字来把解好密的glbData传到C++层
        bool setGlbData(const char *glbPath, const void *glbData, size_t length);

        // faceMesh用到到接口，可以传相机纹理给到3D引擎
        void setFaceTexture(filament::Texture *texture);

        filament::Engine *getEngine();

        filament::View *getView();

        filament::Camera *getCamera();

        void setFilamentAssetScale(float scaleFactor);

        void setFilamentAssetScale(int index, float scaleFactor, filament::math::float2 scaleRange);

        void setFilamentAssetTranslation(filament::math::float3 trans);  // Not Used
        void setFilamentAssetRotate(filament::math::float3 euler_zxy); //for kapu
        void setFilamentAssetPosition(int index, filament::math::float3 position);

        void setFilamentAssetRotate(int index, filament::math::float3 euler_zxy);

        void rotateARModelToFront(int deviceOrientation); // Used for AR model to face camera

        filament::math::float3 getFilamentAssetPosition();

        // 绘制AR阴影平面
        void showArShadowPlane(filament::math::float3 arbitraryPoint, filament::math::float3 normal);

        // 检测当前AR平面与相机到目标位置向量的夹角
        float checkAngleBetweenPlaneAndCameraVector(filament::math::float3 targetPosition);

        bool filamentAssetHitTest(filament::math::float3 position);

        void addTouchPoint(filament::math::float2 touchPoint);

        // 根据随机出来的值（charmRange），确定显示素材里哪些节点
        void changeAssetDynamic(int *charmRange, int rangeCount, int *genders, int genderCount);

        void updateFaceVertices(void *vertices, void *vertices2,
                                filament::math::mat4f &orthMatrix,
                                filament::math::mat4f &perspectiveMatrix,
                                filament::math::mat4f &modelMatrix,
                                filament::math::mat4f &modelConvertMatrix,
                                float scale, uint32_t userIndex,
                                float staticAlpha = -1.0);

        float *getDualFaceMeshAssetVertex();

        void getTessellationVerticesMapData(
            std::vector<int> &dynamicVerticesMap,
            std::vector<int> &staticVerticesMap,
            std::vector<int> &tessellationVerticesMap);

        void setHeadTransform(uint32_t userIndex, filament::math::mat4f transform);

        void initARTransform(filament::math::float3 midPoint);

        // 每一帧更新人脸3D点的信息
        void
        setMaterialTransform(uint32_t userIndex, filament::math::mat4f transform, void *vertices, void *vertices3DMM,
                             int vertices3DMMCount, float posScale = 1.0F);

        void setMvpMatrix(filament::math::mat4f &orthMatrix,
                          filament::math::mat4f &perspectiveMatrix,
                          filament::math::mat4f &modelMatrix, float scale);

        void setProjFaceVertice(void *vertices3DMM);

        // 用于测试批量3D粒子的接口
        void setMaterialTransformTest(filament::math::mat4f transform, uint32_t userIndex,
                                      void *vertices);

        // 设置animoji节点的权重值
        void setMorphNameAndWeight(const std::string &nodeName, const std::vector<gltfio::MorphWeight> &weights,
                                   uint32_t userIndex, uint32_t materialIndex);

        void updateMorphWeights(int userIndex, float *expressionWeights,
                                filament::math::float3 faceAngles);

        // 更新眼珠旋转
        void updateEyeNodeEuler(uint32_t userIndex, uint32_t materialIndex, const std::string &nodeName,
                                const filament::math::float3 euler);

        void updateEyeEulerAngles(int userIndex, filament::math::float3 eulerAngles);

        // 动作触发后更新贴图的接口
        void setMaterialImage(std::string &path, std::string &itemName, std::string &parameterName, int userIndex,
                              int materialIndex,
                              bool sRGBA, bool hideEntity);

        void setMaterialImage(int userIndex, int materialIndex, const std::string &itemName,
                              const std::string &parameterName, const std::string &path, bool sRGBA);

        void updateNodeTransform(uint32_t userIndex, uint32_t materialIndex, const std::string &nodeName,
                                 filament::math::float3 *scale, filament::math::float3 *translate,
                                 filament::math::float3 *rotate);


        // 注册动画
        void registerAnimation(std::vector<std::string> &nameList);

        //动画触发修改版
        void updateAnimationTrigger(std::vector<std::string> &nameList);

        // 播放动画：count == 0, 代表循环播放
        void playAnimation(const std::string &name, int count);

        // 动画是否在运行
        bool isAnimationRunning(std::string &name);

        // 停止动画
        void stopAnimation(const std::string &name);

        filament::math::float2 getAnimationTime(const std::string &name);

        // 暂停动画(pause)
        void pauseGlobalAnimation();

        // 恢复动画(resume)
        void resumeGlobalAnimation();

        // 动态shader实时动态数据, Android端调用, 传入纹理指针 TODO(@dianxin) : Find a way to merge the below two functions
        void updateShaderDynamicValues(const std::map<std::string, filament::Texture *> &nameToTextureMap);

        // 动态shader实时动态数据, Ios端调用，传入image指针
        void setPreservedTextureMap(std::map<std::string, void *> &textureDataMap);

        // 重置相关
        void reset();

        void showOrHideAsset(int userIndex, int materialIndex, bool show);

        void showOrHideAssetEntity(int userIndex, int materialIndex, const std::string &entityName,
                                   bool show);

        filament::math::float3 getPosition();

        filament::math::float3 getRotation();

        filament::math::float3 getScale();

#if !IS_IOS

        //特效达人，手动设置参数接口，暂时只支持单个glb
        void setPosition(filament::math::float3 translate);

        void setRotation(filament::math::float3 rotate);

        void setScale(filament::math::float3 scale);

        void
        setGlbTransform(filament::math::float3 scale, filament::math::float3 rotate, filament::math::float3 translate);

        void setIblIntensity(int intensity);

        void setIblRotation(float rotation);

        void setIblDegree(float rotation);

        void setDirectionIntensity(int intensity);

        void setLightDirection(float x, float y, float z);

        void setDirectionColor(float r, float g, float b);

        int getIblIntensity();

        float getIblRotation();

        int getDirectionIntensity();

        filament::IndirectLight *getibl();

        filament::math::float3 getLightDirection();

        filament::math::float3 getDirectionColor();

        TTMaterialLightParam *getLightParam();

        int getDynamicLightCount();

        std::vector<TTMaterialDynamicLightParam *> getDynamicLightsParams();

        std::vector<utils::Entity> getDynamicLights();

        void setLightParam(TTMaterialLightParam *light);

        // 在PC端，把头像glb直接添加在param->materialModels的末尾
        void addUserHeadGlb(const utils::Path &asset_path);

        void setModelsTransform();

#endif

        void fitIntoUnitCube();

#if !IS_IOS

        void saveParam2Json(std::string path);

#endif

        // 隐藏掉不渲染的人头
        void hiddenExtraHead(int currentHeadCount);

        // 动态毛发层数
        void updateAnimation(float deltaTime);

        void updateRenderTime(float deltaTime, bool debug_custom_shader_parameter = false);

        void updateAnimationKapu(float deltaTime);

        bool needRenderUpdate = true;     //标志该帧是否需要绘制，暂时用于kapu素材

        void updateCustomShaderParameter(int userIndex, int materialIndex,
                                         int customShaderMaterialIndex,
                                         const std::string &paramName,
                                         const std::vector<std::string> &paramValue,
                                         const std::string &valueSamplerConfigFile);

        bool isLoadAllData = false;

        bool startLoad = false;

        bool isFirstRender = true;

        // 渲染3D，deltaTime会用于动画，返回值表示是否更新绘制
        bool render(float deltaTime);

        std::vector<gltfio::FilamentAsset *> getAssetList();

        std::vector<gltfio::FilamentInstance *> getFilamentInstances();

        std::vector<gltfio::FilamentAsset *> getHitAssetList(float *hitPoint);

        std::vector<utils::Entity> getHeadList();

        filament::math::float3 const *getSphericalHarmonics();

        // 切换比例时需要更新引擎的画布大小，bloom等功能也需要更新大小
        void updateFilamentSize(uint32_t w, uint32_t h);  //切比例后续要看看
        // 切换比例时需要更新引擎的画布大小，bloom等功能也需要更新大小
        void resizeBloomView(uint32_t w, uint32_t h);

        // Bloom功能用到的，更新与 bloome primary、hblur、vblur viewport、camera Projection
        void updateBloomViewSize();  // 更新与 bloome primary、hblur、vblur viewport、camera Projection

        void setUpMaterial(const void *furpayload, size_t fursize);

        // 设置faceMesh相关的信息
        void setUpFaceMaterial();

        // 根据相机流的亮度情况，更新3D引擎的全局光照强度
        void updateIBL(float averageL);

        // 毛发功能每一帧渲染时，根据时间更新参数，用来模拟毛发根据风力、重力甩动的效果
        void updateParams(double now);

        // 眼镜反射更新所涉及到的参数。为了眼镜镜片上的反射画面能够更加清晰可控，所以就不用PBR来计算镜片的反射，而使用自定义的shader来计算
        void updateReflectionParams(float zNormalScale, float blendFactor,
                                    filament::math::float2 reflectOffset, float modulateAlpha);

        // 更新相机流光亮的map信息
        void updateIntensityMap(filament::math::float2 intensityKey, filament::math::float2 intensityValue);

        //更新mesh节点
        void updateMesh(std::string name, float *vertex, int size);

        void updateMeshMorph(std::string name, int morphIndex, float *vertex, int size);

        void updateMeshMorphBase(std::string name, float *vertex, int size);

        // 可以实时调整头模羽化参数
        void updateHeadFeatherParams(float offsetX, float offsetY, float sigma);

        // 支持的最大人脸数量
        int getMaxFaceCount();

        void updateReflectionParams(std::vector<std::vector<TTReflectConfig *>> reflect);

        bool getEnableShadow();

        std::vector<float> getIntensityKey();

        std::vector<float> getIntensityValue();

        int getColorTexture();

        bool canUseShareContext() {
            return useShareContext;
        }

        // 用于素材工具的接口
        void addBone(std::string &rootName);

        void deleteBone(std::string &rootName);

        bool updateBoneParams(std::string &rootName, int boneIndex, std::string &attributeName,
                              float value);

        bool updateBoneNoRotation(std::string &rootName, int value);

        bool updateBoneGravity(std::string &rootName, float values[]);

#if ENABLE_3D_TOOL_IOS
        // iOS 工具接口
        void updateModelScale(float x,float y,float z);//更新模型的大小（暂时默认只有一个模型）
        void updateModelPosition(float x,float y,float z);//更新模型的位置（暂时默认只有一个模型）
        void updateModelRotate(float x,float y,float z);//更新模型的旋转角（暂时默认只有一个模型）
        void updateIbl(int intensity, float degree); //更新模型的ibl（暂时默认只有一个模型）
        void updateDirectionLight(TTMaterialLightParam* lightParam); //更新模型的方向光（暂时默认只有一个模型）
        void updateModelFov(int fov);
        SMART_PTR<DynamicBoneHelper> getDynamicBoneHelper();
#endif

        void setDebugMode(bool mode);

        std::vector<std::vector<TTReflectConfig *>> getReflectConfig();

        //Used for MacOs only
        std::vector<TTCustomShader *> getCustomShaderList();

        // filament渲染回调，在FilamentView中实现并在Filter初始化时传入
        filament::backend::FrameFinishedCallback callback = nullptr;

        // 用于素材工具调试毛发用的map
        std::vector<TTNodeModel *> getFurMap() {
            if (furComponent == nullptr) {
                return std::vector<TTNodeModel *>();
            } else {
                return furComponent->furMap;
            }
        }

        static filament::Viewport viewport;

        static void freeCallback(void *buffer, size_t size, void *user);

        void saveOutTexture();

        void setOutputPath(const utils::Path &path);

        const std::map<std::string, filament::Texture *> getExternalTextureMap();

        std::pair<std::string, filament::Texture *> getFaceTexture();

        std::vector<utils::Entity> getStickerEntities();

        SMART_PTR<TextureManager> getTextureManager();

        void updateSticker2DModels(SMART_PTR<AESticker2DConfig> config);

        void updateFurLayerCount(size_t layerCount);

        void setupLoaders(const std::vector<CM::Loader *> &loaders);

        void loadData();

        void updateKapuMorphData();

        void
        setExpressionMorphTargetDataForKapu(int index, std::string nodeName, float **dataList, std::string *expList,
                                            int expCount);

        void updateKapuBlendShapeWeights(int index, std::map<std::string, float> &weightMap);

        void updateKapuBlendShapeWeights(int index, float *expressionList);

        std::vector<std::map<std::string, MorphData *>> getKapuMorphData(int index);

        void setKapuAnimation();

        void playKapuAnimation();

        bool getAnimationOver() {
            return isAnimationOver;
        };

        bool getNeedFaceBind() {
            return needFaceBind;
        }

        void setupKapuModelTransform();

        void
        setDisplayMaterialTypeList(int userIndex, std::vector<std::string> typeStringList);     //控制需要显示的materialType
        void setKapuHeadTransform(uint32_t userIndex, filament::math::mat4f transform);

        //camera animation
        void setCameraAnimation(filament::math::float3 eye, filament::math::float3 rotation, float animationDuration,
                                utils::EasingFunctionType type = utils::EasingFunctionTypeLinear);

        std::vector<filament::View *> getOffscreenViews();

        /// 设置kapu模型显示位置
        /// @param userIndex userIndex
        /// @param fullViewSize 整个renderView的size
        /// @param modelViewFrame renderView中用于显示模型的区域
        /// @param bottomMargin 模型的底部到modelView的下边距
        void
        adjustKapuModelView(int userIndex, filament::math::float2 fullViewSize, filament::math::float4 modelViewFrame,
                            float bottomMargin);

        void changToAnimation(int animIndex);

        void changAnimationToDegree(int materialIndex, float degreeXY, float degreeZ);

        /**
         * 更新自定义的Uniforms
         */
        void updateCustomUniforms();

        int getFov();

    private:
        void setupMesh();

        void setupMeshKapu();

        void setupIbl();

        void setupSkybox();

        void setupSkeletons();

        void setupDirectionLight();

        void setupDynamicLights();

        void setupView(uint32_t width, uint32_t height);

        void setupViewParam(filament::View *finalView);

        filament::View *getFinalView();

        void setAssetTransform(TransformFunc func);

        void setTransformByAsset(int index, TransformFunc func);

        void createRenderFrameBuffer(uint32_t width, uint32_t height);

        void createSwapChain(uint32_t width, uint32_t height);

        void updateProjAndViewport(bool shouldUpdateView, uint32_t width, uint32_t height);

        void *native_layer_ = nullptr;  // It's only a reference from outside. Don't release it.
        uint32_t width = 0;
        uint32_t height = 0;
        int fov = 0;
        utils::Path resource_path_;
        fila_data *sourceAsset;
        TTFilamentInitParam initParam;
        DeviceCapability capability_;

        intptr_t metalTexture = -1;
        bool needRenderTarget = false;
        bool needFaceBind = true;
        bool enableFaceMesh = false;    // 增加一个 face mesh 是否开启标志位
        bool shouldLoadTexture = false;  //自定义shader是否需要加载纹理
        bool isEngineCreatedByUs =
                false;  // TODO(@dianxin) : This is temporarily used. Try to split constructor instead.
        bool is_high_level_GPU_ = true;
        bool isKapuMaterial = false;
        bool useShareContext = true;
        filament::Engine *engine = nullptr;
        filament::Renderer *renderer = nullptr;
        filament::Scene *scene = nullptr;
        filament::View *view = nullptr;
        filament::Camera *camera = nullptr;
        SMART_PTR<filament::SwapChain> swapChain;
        SMART_PTR<TTFilamentSetupParam> param;
        std::vector<SMART_PTR<TTThreeDMaterialModel>> kapuParams;
        SMART_PTR<gl_util::Framebuffer> framebuffer;

        SMART_PTR<FurComponent> furComponent;
        SMART_PTR<BloomComponent> bloomComponent;
        SMART_PTR<FaceMeshComponent> faceMeshComponent;
        SMART_PTR<LIGHT3D::DualFaceMeshComponent> dualFaceMeshComponent;
        SMART_PTR<LIGHT3D::CustomShaderComponent> customShaderComponent;
        SMART_PTR<LIGHT3D::TransparentComponent> transparentComponent;
        SMART_PTR<NormalCubemapComponent> normalCubemapComponent;
        SMART_PTR<GeometryComponent> geometryComponent;
        SMART_PTR<DynamicGlbComponent> dynamicGlbComponent;
        SMART_PTR<ReflectionComponent> reflectionComponent;
        SMART_PTR<HeadFeatherComponent> headFeatherComponent;
        SMART_PTR<StickerComponent> stickerComponent;
        SMART_PTR<LIGHT3D::AESticker2DComponent> aeSticker2DComponent;

        SMART_PTR<TextureManager> textureManager;
        SMART_PTR<MaterialManager> materialManager;
        SMART_PTR<IblManager> iblManager;
        SMART_PTR<LIGHT3D::AnimojiManager> animojiManager;

        SMART_PTR<LIGHT3D::AssetHelper> assetHelper;
        SMART_PTR<AssetHelperKapu> assetHelperKapu;
        SMART_PTR<HeadHelper> headHelper;
        SMART_PTR<TransformHelper> transformHelper;
        SMART_PTR<DynamicBoneHelper> dynamicBoneHelper;
        SMART_PTR<LIGHT3D::FaceBindHelper> faceHelper;
        SMART_PTR<LIGHT3D::AnimatorHelper> animatorHelper;

        float renderTime = 0.0; //将上层传入的间隔时间累加起来，用于动画速率随帧率变化
        float animationTime = 0.0; //动画的播放时间，主要是为了支持动画的暂停(pause)和继续(resume)
        double currentAnimTime = 0.0;
        bool isAnimationOver = true;
        bool animationPaused = false; //动画是否被用户暂停

        filament::math::float3 lastArbitraryPoint =
                filament::math::float3{0, 0, 0};  //保存上一帧平面的基准点；
        filament::math::float3 lastNormal = filament::math::float3{0, 0, 0};  //保存上一帧平面的法向量；
        filament::math::float3 iblSphericalHarmonics[9];

        SMART_PTR<TrianglesEntityDatasource> arShadowPlaneSource;   //用于绘制AR模型的阴影
        SMART_PTR<filament::IndirectLight> indirectLight;
        SMART_PTR<filament::Skybox> skybox;
        SMART_PTR<filament::Skybox> debug_skybox;
        filament::Texture *iblTexture = nullptr;
        filament::Texture *faceTexture = nullptr;
        filament::Texture *skyboxTexture = nullptr;
        filament::Texture *cameraTexture = nullptr;
        std::map<std::string, filament::Texture *> preservedTextureMap;
        std::vector<utils::Entity> directionLights;
        std::vector<utils::Entity> dynamicLights;
#if FILAMENT_PARTICLE_ENABL
        std::vector<std::vector<gltfio::FilamentAsset*>> particleAssetList;
#endif
        std::vector<float> intensity_key_;
        std::vector<float> intensity_value_;

        utils::Path savePath;
        bool hasUserHead = false;

        // mode:0 for light studio
        // mode:1 for pituDemo
        TTFilamentMode filamentMode = TTFilamentMode::LIGHT_STUDIO_MODE;

        void updateFaceTexture(uint32_t w, uint32_t h);

        //camera animation
        float pastCameraAnimationTime = 0.0f;
        float totalCameraAnimationTime = -1.0f;
        utils::EasingFunctionType cameraAnimationType = utils::EasingFunctionTypeLinear;
        filament::math::float3 cameraAnimationStartEyePosition;
        filament::math::float3 cameraAnimationEyeDelta;
        filament::math::float3 cameraAnimationStartEyeRotation; //角度
        filament::math::float3 cameraAnimationEyeRotationDelta; //角度
        void updateCameraAnimation(float deltaTime);
    };
LIGHT3D_NAMESPACE_END
#endif /* FilamentFilter_h */
