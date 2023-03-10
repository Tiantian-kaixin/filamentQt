// Copyright (C) 2019 The Android Open Source Project
// Created by kevinxing on 2020-02-14.
//

#ifndef TNT_MODELDEF_H
#define TNT_MODELDEF_H

#include <set>
#include <map>
#include <string>
#include <unordered_map>

#include "fila_struct.h"
#include <utils/Path.h>
#include <utils/Entity.h>
#include <filament/Texture.h>
#include <filament/MaterialEnums.h>
#include <filament/VertexBuffer.h>
#include <filament/IndexBuffer.h>
#include <filament/MaterialInstance.h>
#include <gltfio/common_util.h>
#include "string_util.h"
#include <gltfio/gl_util.h>
#include <gltfio/MaterialManager.h>
#include <gltfio/TextureManager.h>
#include <gltfio/NormalCubemapComponent.h>

LIGHT3D_NAMESPACE_BEGIN

    class TrianglesEntityDatasource {
    public:
        ~TrianglesEntityDatasource() {
            if (vertices != nullptr) {
                delete vertices;
                vertices = nullptr;
            }

            if (indices != nullptr) {
                delete indices;
                indices = nullptr;
            }

            if (tangents != nullptr) {
                delete tangents;
                tangents = nullptr;
            }

            if (uv != nullptr) {
                delete uv;
                uv = nullptr;
            }
        }

        utils::Entity entity;
        filament::math::float3 *vertices = nullptr;
        uint16_t vertexCount;
        uint16_t *indices = nullptr;
        uint16_t indexCount;
        filament::MaterialInstance *matInstance;
        filament::math::quatf *tangents = nullptr;
        filament::math::float2 *uv = nullptr;
    };

    class TTHeadData {
    public:
        filament::VertexBuffer *vb;
        filament::IndexBuffer *ib;
        utils::Entity head;

        ~TTHeadData() {
            if (vb) {
                // TODO
                vb = nullptr;
            }
            if (ib) {
                // TODO
                ib = nullptr;
            }
        };
    };

    class TTDynamicBoneParam {
    public:
        std::vector<std::map<std::string, std::string>> boneParamsList;
        std::vector<std::map<std::string, std::string>> colliderList;
        std::string rootName;
        std::string debugBoneRootName;  // ?????? debug
        std::string debugModelName; //  ?????? debug
        filament::math::float3 gravity = 0;
        float boneRadius;
        int boneCount;
        bool noRotationUpdate = false;
        int updateMode = 0;
        int resetType = 0;
    };

    class TTReflectConfig {
    public:
        std::string nodeName;
        std::string baseColorImage;
        std::string reflectImage;
        float zNormalScale;
        float blendFactor;
        filament::math::float2 reflectOffset;
        float modulateAlpha;
        int blendMode;
    };

    class TTSkeleton {
    public:
        std::string nodeName;
        filament::math::float3 translate;
        filament::math::float3 scale;
    };

    class TTMaterialLightParam {
    public:
        filament::math::float3 direction;
        filament::math::float3 color;
        float intensity;
        bool castShadow;
    };

    class TTMaterialHeadFeatherParam {
    public:
        filament::math::float3 scale;
        float sigma;
        float offsetX;
        float offsetY;
        int useSpecificHeadModel;
        std::string smallHeadName;
        std::string bigHeadName;
    };

    class TTMaterialDynamicLightParam {
    public:
        filament::math::float3 position;
        filament::math::float3 color;
        float intensity;
        float efficiency; // see LightManager::setIntensity(Instance i, float watts, float efficiency)
    };

    class TTMaterialBloomParam {
    public:
        std::vector<std::string> bloomEntityNameList;    // ???????????? entity bloom
        float mixAlpha;             // blur view ??? primary view mix ??????, mixAlpha ??????, blur ??? mix ????????????????????? 0.0 ??? 1.0
        float blurWeight;           // blur weight ???????????????blur ???????????????  0.0 ??? 1.0
        float blurScale;            // scale ??????, blur ????????????, ????????????  1.0???5.0
        // 1: primary.alpha > 0 ???????????? blur mix???????????? primary ??????????????????
        // 0: primary ??????????????? blur mix
        int mixType;               // 0 or 1
    };

    class TTMaterialFurParam {
    public:
        filament::Shading shadingModel;
        std::string noiseTexturePath;
        std::string furTexturePath;
        filament::math::float4 gravity;
        filament::math::float4 wind;
        filament::math::float4 shadowColor;
        filament::math::float4 uvOffset;
        filament::math::float3 anisotropyDirection;
        float length;    //????????????
        float spacing;  //????????????
        float edgeSoftness; //tming ???????????????
        float topEndAlpha;  //dming ???????????????
        float roughness;  //?????????
        float reflectance;  //?????????
        float anisotropy;   //??????????????????

        //???????????????????????????????????????????????????
        bool enableLight = false;
        filament::math::float4 _BaseColor = filament::math::float4(1.0, 1.0, 1.0, 1.0);
        filament::math::float4 _SubTexUV = filament::math::float4(1.0, 1.0, 0.0, 0.0);
        filament::math::float4 shiftTexTilingOffset = filament::math::float4(1.0, 1.0, 0.0, 0.0);
};

    class TTEyeNode {
    public:
        std::string name;
        filament::math::float3 eulerAngles;
    };

    class TTShapeRange {
    public:
        float min;
        float max;

        TTShapeRange() : TTShapeRange(-1, -1) {};

        TTShapeRange(float min, float max) : min(min), max(max) {};
    };

    class TTFaceBindConfig {
    public:
        std::string entityName;
        int faceIndex;
    };

    class TTExpressionConfig {
    public:
        std::string shapeName;
        std::string controlledName;
        TTShapeRange shapeRange;
        TTShapeRange controlledRange;
    };

    class TTNodeModel {
    public:
        std::string name;
        int faceMesh = 0;
        int blendOrder = 0;
        bool enableExpressionConfigRemap;
        std::unordered_map<std::string, size_t> expressionOrderList;
        std::vector<SMART_PTR<TTExpressionConfig>> expressionConfigList;
        TTMaterialFurParam *furParam = nullptr; //?????????????????????
        bool castShadow;
        bool receiveShadow;

        ~TTNodeModel() {
            if (furParam != nullptr) {
                delete furParam;
                furParam = nullptr;
            }
        };
    };

    struct TriggerableAnimationItem {
        std::string name;

        int count;
        float blendDuration;
        float degreeXY;
        float duration;
        float startTime;
        float endTime;

        bool isDefault;
        bool isDegreeZ = false;
    };

    enum class TTCustomParamType {
        SAMPLER_2D,         //!< 2D or 2D array texture
        SAMPLER_CUBEMAP,    //!< Cube map texture
        BOOL,
        BOOL2,
        BOOL3,
        BOOL4,
        FLOAT,
        FLOAT2,
        FLOAT3,
        FLOAT4,
        INT,
        INT2,
        INT3,
        INT4,
        UINT,
        UINT2,
        UINT3,
        UINT4,
        MAT3,   //!< a 3x3 float matrix
        MAT4    //!< a 4x4 float matrix
    };

    enum class TTCustomParamValueType {
        Float,
        Color,
        Direction
    };

    enum class TTFilamentMode {
        LIGHT_STUDIO_MODE,
        PITU_DEMO_MODE
    };

    namespace parser {
        static const std::string TIME = "$time"; //NOLINT
        static const std::string CAMERA_TEXTURE = "$cameraTexture"; //NOLINT
        static const std::string PRESERVED_TEXTURE = "$preservedTexture_";
    }

    class ValueSampler {
        using TimeValues = std::vector<float>;
        using SourceValues = std::vector<float>;
        using TexturePaths = std::vector<std::string>;

    public:
        enum class Interpolation {
            LINEAR, STEP, CUBIC // CUBIC is not supported for now
        };
        enum class TextureType {
            SAMPLER2D, SAMPLERCUBEMAP
        };
        std::set<filament::MaterialInstance *> material_instance_listeners_;

        template<class T>
        static ValueSampler *
        Create(std::string param_name, TimeValues times, std::vector<T> data, Interpolation interpolation);

        static ValueSampler *
        Create(std::string param_name, TimeValues times, TexturePaths data, Interpolation interpolation,
               filament::backend::SamplerWrapMode sampler_wrap_mode, filament::backend::SamplerMinFilter sampler_min_filter,
               TextureType type = TextureType::SAMPLER2D, bool sRGBA = true);

        // Note that when render_time is smaller than 0, it's used to keep updating whatever in current_value. current_value can be modified by ImGui.
        virtual void Update(float render_time, SMART_PTR<TextureManager> texture_manager, const char *nodeName,
                            SMART_PTR<LIGHT3D::NormalCubemapComponent> normalCubemapComponent) = 0;

        virtual std::vector<std::string> GetCurrentValueInString() = 0;

        void AcceptMaterialInstance(filament::MaterialInstance *material_instance) {
            material_instance_listeners_.insert(material_instance);
        }

        virtual std::tuple<float *, size_t> GetFloatPointer() = 0;
    };

    template<class T>
    class ValueSpecificSampler : public ValueSampler {
        using TimeValues = std::vector<float>;
        using SourceValues = std::vector<T>;
        using TexturePaths = std::vector<std::string>;
    public:
        ValueSpecificSampler(std::string param_name, TimeValues times, SourceValues values, Interpolation interpolation)
                :
                param_name_(std::move(param_name)),
                times(std::move(times)),
                values(std::move(values)),
                interpolation(interpolation) {};

        ValueSpecificSampler(std::string param_name, TimeValues times, const TexturePaths &values,
                             Interpolation interpolation, filament::backend::SamplerWrapMode sampler_wrap_mode,
                             filament::backend::SamplerMinFilter sampler_min_filter, TextureType type, bool sRGBA) :
                param_name_(std::move(param_name)),
                times(std::move(times)),
                values(values),
                interpolation(interpolation),
                samplerWrapMode(sampler_wrap_mode),
                samplerMinFilter(sampler_min_filter),
                textureType(type),
                current_value(values[0]),
                sRGBA(sRGBA) {};

        void Update(float render_time, SMART_PTR<TextureManager> texture_manager, const char *name,
                    SMART_PTR<LIGHT3D::NormalCubemapComponent> normalCubemapComponent) override;

        std::vector<std::string> GetCurrentValueInString() override;

        std::tuple<float *, size_t> GetFloatPointer() override;

        void forceSetValues(const std::vector<T> &values) {
            this->values = values;
        }

        utils::Path modelDir; // ?????????texture????????????????????????
        filament::Texture *kapuTexture = nullptr; // ?????????texture????????????????????????
    private:
        bool is_first_initialize = true;
        T current_value;
        std::string param_name_;
        TimeValues times;
        std::vector<T> values;
        Interpolation interpolation = Interpolation::STEP;
        TextureType textureType = TextureType::SAMPLER2D;
        filament::backend::SamplerWrapMode samplerWrapMode = filament::backend::SamplerWrapMode::CLAMP_TO_EDGE;
        filament::backend::SamplerMinFilter samplerMinFilter = filament::backend::SamplerMinFilter::LINEAR;
        bool sRGBA;
    };

    class TTCustomUniformParam {
    public:
        static std::unordered_map<std::string, filament::backend::SamplerMinFilter> samplerMap;
        std::string param_name_;  //uniform?????????
        std::string uniform_type_;
        TTCustomParamValueType value_type_; // Used for dynamic configure in simpler viewer tool
        float value_range_[2]{}; // Used for dynamic configure in simpler viewer tool
        bool need_external_texture = false;
        ValueSampler *value_sampler_{nullptr};

        explicit TTCustomUniformParam(const fila_custom_params &fila_param);

        void Update(float render_time, SMART_PTR<TextureManager> texture_manager, const char *nodeName = nullptr,
                    SMART_PTR<LIGHT3D::NormalCubemapComponent> normalCubemapComponent = nullptr) {
            value_sampler_->Update(render_time, std::move(texture_manager), nodeName,
                                   std::move(normalCubemapComponent));
        }
        void SetUniformData(const std::vector<std::string> &param_value);

        std::vector<std::string> GetCurrentValueInString() {
            return value_sampler_->GetCurrentValueInString();
        }

        std::tuple<float *, size_t> GetFloatPointer() {
            return value_sampler_->GetFloatPointer();
        }

        static filament::backend::SamplerMinFilter getMinFilterFromString(char *name);

        void AcceptMaterialInstance(filament::MaterialInstance *material_instance) {
            value_sampler_->AcceptMaterialInstance(material_instance);
        }
    };

    class TTCustomShader {
    public:
        ~TTCustomShader() {
            for (auto *param : parameterVec) {
                delete param;
            }
        }

        enum class AssetType {
            NORMAL,
            FUR,
            SHADOW_PLANE,
            HEAD_3DMM,
            TRANSPARENT,
            TRANSPARENT_TAIL,
        };
        std::vector<std::string> nodeNameList;//?????????????????????????????????filamat
        std::string filamatPath;//?????????filamat??????????????????????????????????????????????????????????????????bundle??????filamat
        std::vector<LIGHT3D::TTCustomUniformParam *> parameterVec;//???????????????uniform
        std::map<std::string, int> param_name_to_vector_index;//?????????????????????
        AssetType assetType = AssetType::NORMAL;
        bool needNormal = false;

        void destroy(SMART_PTR<TextureManager> textureManager) {
            for (auto *param : parameterVec) {
//            param->destroy(textureManager);
                delete param;
            }
            parameterVec.clear();
        }
    };
//?????????shader end

    class TTThreeDMaterialModel {
    public:
        std::vector<TTDynamicBoneParam *> boneParams;
        std::vector<TTReflectConfig *> reflectConfigList;
        std::vector<TTSkeleton *> skeletonList;
        std::vector<TTCustomShader *> customMaterialList;
        std::vector<TTNodeModel *> nodes;
        std::vector<TTEyeNode *> eyeNodeList;
        std::vector<std::string> triggerNodeName;
        std::vector<uint8_t> glbData;
        std::vector<uint8_t> dualGlb;
        std::vector<float> intensityKey;
        std::vector<float> intensityValue;
        std::vector<std::string> nodeAssociateGender;
        std::vector<TriggerableAnimationItem> animationList;
        std::vector<TTFaceBindConfig> faceParams;

        enum class GlbLoadType {
            glb, gltf, bin
        };
        GlbLoadType glbLoadType;
        std::string modelName;    // glb?????????
        utils::Path modelDir;   // ??????????????????
        std::string dualPath = "";
        int faceMeshType = 0;
        std::string iblPath; // ???????????????????????????
        std::string skyboxPath; //  ?????????

        filament::math::float3 scale;
        filament::math::float3 translate;
        filament::math::float3 rotate;
        filament::math::float3 staticPosition;  // ???????????? ios ?????????
        filament::math::float3 eulerAngles;     // ???????????? ios ?????????, ???????????????
        filament::math::float3 autoScaleFactor;   // ??????x???y???z scale ????????????????????????????????????, 0 ?????????
        float scaleRange[2];

        filament::math::int3 rotateType;
        filament::math::int3 flip;

        float iblRotation;
        float iblDegree;
        float fixedTime;

        bool needNormal = false;

        int iblIntensity;
        int useClearIbl;// =1 ??????????????????ibl, =0??????????????????ibl
        int furLayerCount;
        int transparentLayerCount;
        int positionType;
        int needFaceMesh;
        std::string faceMeshTextureKey;
        size_t nodeAssociateGenderCount;
        TTMaterialHeadFeatherParam *headFeatherParam;
        TTMaterialLightParam *lightParam;// ???????????????optional
        std::vector<TTMaterialDynamicLightParam *> dynamicLightListParam;// ??????????????????optional
        TTMaterialBloomParam *bloomParam;  // bloom ??????

        ~TTThreeDMaterialModel() {
            for (TTDynamicBoneParam *param: boneParams) {
                delete param;
            }
            boneParams.clear();
            for (TTNodeModel *node: nodes) {
                delete node;
            }
            needNormal = false;
            nodes.clear();
            delete lightParam;
            lightParam = nullptr;
            delete bloomParam;
            bloomParam = nullptr;
            delete headFeatherParam;
        };
    };

    class HeadModelConfig {
    public:
        class HeadModelWrapper {
        public:
            MaterialType headType;
            std::string headIndexFile;
            std::string headVertexFile;
            std::string headUVFile;
            std::string headNormalFile;
        };

        utils::Path materialDir;
        utils::Path resourcePath;
        HeadModelWrapper headModel;
        static const HeadModelWrapper HEAD_MODEL_YOUTU;
        static const HeadModelWrapper HEAD_MODEL_3DMM;
    };

    class TTFilamentInitParam {
    public:
        utils::Path materialDir;
        const void *jsonStr = nullptr;
        size_t jsonLength = 0;
        bool isHighGPU = true;
        bool loadGlb = true;
        uint32_t width = 0;
        uint32_t height = 0;
        utils::Path resource_path;
        HeadModelConfig::HeadModelWrapper headModel = HeadModelConfig::HEAD_MODEL_YOUTU;
        bool kapuMaterial = false;
        int deviceClass = -1;//deviceClass???-1????????????0???????????????1????????????2????????????3????????????4???????????????
        bool loadResourceAsync = true;
        intptr_t metalTexture = -1;
        filament::Engine::Backend backend = filament::Engine::Backend::OPENGL;
    };

    enum class KapuMaterialType {
        INVALID,
        ROLE,
        TOPS,
        HAIR,
        PANTS,
        SHOES,
        STOCKING,
        EYELASH,
        EYEBROW,
        EYES,
        EYEGLASSES,
        WING,
        HEAD_DRESS,
        NOSE,
        MOUTH,

        // ???????????????
                freckle,
        eyeshadow,
        blusher,
        eyelid,
        eyeline,
        lipstick,
        tatoo,
    };

    enum class KapuAnimojiExpressionType {
        EXPRESSION_TYPE_BASIC,
        EXPRESSION_TYPE_ALL
    };

    enum class UpdateBoneMatrixType {
        TYPE_FOR_NORMAL,
        TYPE_FOR_KAPU
    };

    struct KapuMaterialModel {
        std::string modelDir;

        static KapuMaterialType getMaterialType(std::string name) {
            if (name == "role") {
                return KapuMaterialType::ROLE;
            } else if (name == "tops") {
                return KapuMaterialType::TOPS;
            } else if (name == "hair") {
                return KapuMaterialType::HAIR;
            } else if (name == "pants") {
                return KapuMaterialType::PANTS;
            } else if (name == "shoes") {
                return KapuMaterialType::SHOES;
            } else if (name == "stocking") {
                return KapuMaterialType::STOCKING;
            } else if (name == "eyelash") {
                return KapuMaterialType::EYELASH;
            } else if (name == "eyebrow") {
                return KapuMaterialType::EYEBROW;
            } else if (name == "eyes") {
                return KapuMaterialType::EYES;
            } else if (name == "eyeglasses") {
                return KapuMaterialType::EYEGLASSES;
            } else if (name == "wing") {
                return KapuMaterialType::WING;
            } else if (name == "headDress") {
                return KapuMaterialType::HEAD_DRESS;
            } else if (name == "nose") {
                return KapuMaterialType::NOSE;
            } else if (name == "mouth") {
                return KapuMaterialType::MOUTH;
            } else if (name == "freckle") {
                return KapuMaterialType::freckle;
            } else if (name == "eyeshadow") {
                return KapuMaterialType::eyeshadow;
            } else if (name == "blush") {
                return KapuMaterialType::blusher;
            } else if (name == "eyelid" || name == "eyelids") {
                return KapuMaterialType::eyelid;
            } else if (name == "eyeliner") {
                return KapuMaterialType::eyeline;
            } else if (name == "lipstick") {
                return KapuMaterialType::lipstick;
            } else if (name == "tatoo") {
                return KapuMaterialType::tatoo;
            } else {
                return KapuMaterialType::INVALID;
            }
        }

        static std::string getMaterialTypeName(KapuMaterialType type) {
            switch (type) {
                case KapuMaterialType::TOPS:
                    return "tops";
                case KapuMaterialType::ROLE:
                    return "role";
                case KapuMaterialType::HAIR:
                    return "hair";
                case KapuMaterialType::EYES:
                    return "eyes";
                case KapuMaterialType::EYELASH:
                    return "eyelash";
                case KapuMaterialType::EYEBROW:
                    return "eyebrow";
                case KapuMaterialType::PANTS:
                    return "pants";
                case KapuMaterialType::STOCKING:
                    return "stocking";
                case KapuMaterialType::SHOES:
                    return "shoes";
                case KapuMaterialType::EYEGLASSES:
                    return "eyeglasses";
                case KapuMaterialType::WING:
                    return "wing";
                case KapuMaterialType::MOUTH:
                    return "mouth";
                case KapuMaterialType::HEAD_DRESS:
                    return "headDress";
                case KapuMaterialType::NOSE:
                    return "nose";
                case KapuMaterialType::freckle:
                    return "freckle";
                case KapuMaterialType::eyeshadow:
                    return "eyeshadow";
                case KapuMaterialType::blusher:
                    return "blush";
                case KapuMaterialType::eyelid:
                    return "eyelids";
                case KapuMaterialType::eyeline:
                    return "eyeliner";
                case KapuMaterialType::lipstick:
                    return "lipstick";
                case KapuMaterialType::tatoo:
                    return "tatoo";
                default:
                    return "invalid";
            }
        }

        static std::string getHSVTypeName(KapuMaterialType type) {
            switch (type) {
                case KapuMaterialType::TOPS:
                case KapuMaterialType::PANTS:
                case KapuMaterialType::SHOES:
                    return "skin";
                case KapuMaterialType::freckle:
                    return "freckle";
                case KapuMaterialType::eyeshadow:
                    return "eyeshadow";
                case KapuMaterialType::blusher:
                    return "blusher";
                case KapuMaterialType::eyelid:
                    return "eyelid";
                case KapuMaterialType::eyeline:
                    return "eyeline";
                case KapuMaterialType::lipstick:
                    return "lipstick";
                case KapuMaterialType::tatoo:
                    return "tatoo";
                case KapuMaterialType::EYEBROW:
                    return "eyebrow";
                case KapuMaterialType::EYES:
                    return "eyes";
                case KapuMaterialType::EYELASH:
                    return "eyelash";
                case KapuMaterialType::HAIR:
                    return "hair";
                default:
                    return "invalid";
            }
        }
    };


    class AESticker2DModel {
    public:
        gl_util::Vertex vertex[4];
        uint16_t indices[6];
        filament::Texture *texture = nullptr;
        int blendMode = 1;
        float alpha = 1;
    };

    class AESticker2DConfig {
    public:
        filament::math::mat4f projectionMat;
        filament::math::mat4f cameraModelMat;
        std::vector<AESticker2DModel> models;
    };

    class TTStickerModel {
    public:
        std::string path;
        size_t width;
        size_t height;
        size_t frames;
        size_t frameDuration;
        filament::math::float3 scale;
        filament::math::float3 translate;
        filament::math::float3 rotate;
    };

    struct DeviceCapability {
        bool ace3d_capable = true;
        bool shadow_capable = true;
        bool fxaa_capable = true;
        bool msaa_capable = true;
        bool ssao_capable = true;
        bool bloom_capable = true;
        bool dither_capable = true;
        bool vignette_capable = true;
    };

    class TTFilamentSetupParam {
    public:
        std::vector<TTThreeDMaterialModel *> materialModels;
        std::vector<TTStickerModel *> stickers;
#if FILAMENT_PARTICLE_ENABL
        std::vector<TTThreeDMaterialModel*> particleMaterialModels;
#endif
        int fovAngle;
        int maxFaceCount;
        int toneMapping;
        int ssao;
        int viewDepthPrepassDisable;//default = 0, enable depthprepass
        int faceMeshShadingModel;
        filament::math::float3 headScale;
        float featureTransformAdjustAlpha; // ???3D???????????????????????????
        utils::Path materialDir;
        bool debug = false;
        bool hideUserHeadModel;
        bool headReceiveShadows;
        bool enableBloom;
        bool enableJobSystem; // ??????????????????????????????,????????????,????????????????????????
        bool enableHeadFeather;     // ????????????????????????
        float dualScale;
        int kapuMaterialType; // 0: ?????? BoneMatrices 1: ????????? BoneMatrices ???????????? animoji
        int aoType = 0;
        int fxaa = 1;
        int msaa = 0;
        filament::View::AmbientOcclusionOptions ssaoOptions;
        filament::View::BloomOptions bloomOptions;

        ~TTFilamentSetupParam() {
            for (auto model: materialModels) { //TODO(@dianxin) : it's said that this will trigger an error if the materials are not loaded successfully
                delete model;
            }
            materialModels.clear();

#if FILAMENT_PARTICLE_ENABL
            for (int i = 0; i < particleMaterialModels.size(); ++i) {
                delete particleMaterialModels[i];
                break;
            }
            particleMaterialModels.clear();
#endif
        };
    };
LIGHT3D_NAMESPACE_END
#endif //TNT_MODELDEF_H
