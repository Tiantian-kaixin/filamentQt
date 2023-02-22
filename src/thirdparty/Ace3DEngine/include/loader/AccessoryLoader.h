//
//  AccessoryLoader.h
//  brick
//
//  Created by louisluo on 2019/1/29.
//  Copyright © 2019 louisluo. All rights reserved.
//

#ifndef AccessoryLoader_h
#define AccessoryLoader_h

#include <string>
#include <vector>
#include "FTLoader.h"
#include "loader/DynamicBoneLoader.h"

namespace CM
{
    namespace Accessory
    {
        enum
        {
            kModel = 1,
            kGeometry = 7,
            kDeformer = 3,
            kMaterial = 4,
            kTexture = 5,
            
            // Model
            kName = 9,
            kInheritType = 10,
            kEulerOrder = 11,
            kTranslation = 12,
            kPreRotation = 13,
            kRotation = 14,
            kPostRotation = 15,
            kScale = 16,
            kScalingOffset = 17,
            kScalingPivot = 18,
            kRotationOffset = 19,
            kRotationPivot = 20,
            kTransform = 21,
            kMaterials = 30,
            
            kAccessoryList = 25,
            kDynamicBone = 7,
            kCollider = 8,


        };
        
        struct Model
        {
            std::string Name;
            uint8_t InheritType = 0;
            uint8_t EulerOrder = 0;
            struct { int count = 0; float* data = nullptr; } Translation;
            struct { int count = 0; float* data = nullptr; } PreRotation;
            struct { int count = 0; float* data = nullptr; } Rotation;
            struct { int count = 0; float* data = nullptr; } PostRotation;
            struct { int count = 0; float* data = nullptr; } Scale;
            struct { int count = 0; float* data = nullptr; } ScalingOffset;
            struct { int count = 0; float* data = nullptr; } ScalingPivot;
            struct { int count = 0; float* data = nullptr; } RotationOffset;
            struct { int count = 0; float* data = nullptr; } RotationPivot;
            struct { int count = 0; float* data = nullptr; } Transform;
            std::vector<std::string> Materials;

#ifdef BK_PLATFORM_IOS
            ~Model() {
                delete[] Translation.data;
                delete[] PreRotation.data;
                delete[] Rotation.data;
                delete[] PostRotation.data;
                delete[] Scale.data;
                delete[] ScalingOffset.data;
                delete[] ScalingPivot.data;
                delete[] RotationOffset.data;
                delete[] RotationPivot.data;
                delete[] Transform.data;
            }
#endif
        };
        

        
        namespace Geometry
        {
            enum
            {
                kVertices = 1,
                kUVs = 2,
                kNormals = 3,
                kWeight = 4,
                kBoneIndices = 5,
                kIndices = 6,
                kUVs2 = 7,
                kUVs3 = 8,
                kUVs4 = 9,
                kUVs5 = 10,
                kUVs6 = 11,
                kUVs7 = 12,
                kUVs8 = 13,

            };
            
            struct Geometry
            {
                struct { int count = 0; float* data = nullptr; } Vertices;
                struct { int count = 0; float* data = nullptr; } UVs;
                struct { int count = 0; float* data = nullptr; } UVs2;
                struct { int count = 0; float* data = nullptr; } UVs3;
                struct { int count = 0; float* data = nullptr; } UVs4;
                struct { int count = 0; float* data = nullptr; } UVs5;
                struct { int count = 0; float* data = nullptr; } UVs6;
                struct { int count = 0; float* data = nullptr; } UVs7;
                struct { int count = 0; float* data = nullptr; } UVs8;
                struct { int count = 0; float* data = nullptr; } Normals;
                struct { int count = 0; float* data = nullptr; } Weights;
                struct { int count = 0; int16_t* data = nullptr; } Indices;
                struct { int count = 0; int16_t* data = nullptr; } BoneIndices;

#ifdef BK_PLATFORM_IOS
                ~Geometry() {
                    delete[] Vertices.data;
                    delete[] UVs.data;
                    delete[] UVs2.data;
                    delete[] UVs3.data;
                    delete[] UVs4.data;
                    delete[] UVs5.data;
                    delete[] UVs6.data;
                    delete[] UVs7.data;
                    delete[] UVs8.data;
                    delete[] Normals.data;
                    delete[] Weights.data;
                    delete[] Indices.data;
                    delete[] BoneIndices.data;
                }
#endif
            };
            
        }
        
        namespace Deformer
        {
            namespace BlendShapeTargetShape
            {
                enum
                {
                    kName = 110,
                    kIndices = 111,
                    kVertices = 112,
                    kNormals = 113
                };
                
                struct BlendShapeTargetShape
                {
                    std::string Name;
                    struct { int count = 0; float* data = nullptr; } Vertices;
                    struct { int count = 0; float* data = nullptr; } Normals;
                    struct { int count = 0; int32_t* data = nullptr; } Indices;

#ifdef BK_PLATFORM_IOS
                    ~BlendShapeTargetShape() {
                        delete[] Vertices.data;
                        delete[] Normals.data;
                        delete[] Indices.data;
                    }
#endif
                };
            }
            
            namespace BlendShapeChannel
            {
                enum
                {
                    kName = 106,
                    kTargetShapes = 109
                };
                
                struct BlendShapeChannel
                {
                    std::string Name;
                    std::vector<BlendShapeTargetShape::BlendShapeTargetShape*> TargetShapes;
                    
                    ~BlendShapeChannel() {
                        for (auto iter: TargetShapes) {
                            delete iter;
                        }
                    }
                };
            }
            
            namespace BlendShapeDeformer
            {
                enum
                {
                    kName = 102,
                    kBlendShapeChannels = 104
                };
                
                struct BlendShapeDeformer
                {
                    std::string Name;
                    std::vector<BlendShapeChannel::BlendShapeChannel*> BlendShapeChannels;
                    
                    ~BlendShapeDeformer() {
                        for (auto iter: this->BlendShapeChannels) {
                            delete iter;
                        }
                    }
                };
            }
            
            namespace SkinDeformerCluster
            {
                enum
                {
                    kIndices = 31,
                    kWeights = 32,
                    kTransformLink = 34,
                    kSrcNodes = 40,
                    kTransform = 33,
                };
                
                struct SkinDeformerCluster
                {
                    struct { int count = 0; int32_t* data = nullptr; } Indices;
                    struct { int count = 0; float* data = nullptr; } Weights;
                    struct { int count = 0; float* data = nullptr; } Transform;
                    struct { int count = 0; float* data = nullptr; } TransformLink;
                    std::vector<std::string> SrcNodes;

#ifdef BK_PLATFORM_IOS
                    ~SkinDeformerCluster() {
                        delete[] Indices.data;
                        delete[] Weights.data;
                        delete[] Transform.data;
                        delete[] TransformLink.data;
                    }
#endif
                };
            }
            
            namespace SkinDeformer
            {
                enum
                {
                    kSkinDeformerClusters = 24
                };
                
                struct SkinDeformer
                {
                    std::vector<SkinDeformerCluster::SkinDeformerCluster*> SkinDeformerClusters;
                    
                    ~SkinDeformer() {
                        for( auto iter: this->SkinDeformerClusters) {
                            delete iter;
                        }
                    }
                };
                
            }
            
            enum
            {
                kSkin = 2,
                kBlendShape = 99
            };
            
            struct Deformer
            {
                std::vector<SkinDeformer::SkinDeformer*> Skin;
                std::vector<BlendShapeDeformer::BlendShapeDeformer*> BlendShape;
                
                ~Deformer() {
                    for (auto iter: this->Skin) {
                        delete iter;
                    }
                    for (auto iter: this->BlendShape) {
                        delete iter;
                    }
                }
            };
        }
        
        namespace Material
        {
            enum
            {
                kName = 2,
                kShadingModel = 20,
                kMultiLayer = 21,
                kEmissive = 22,
                kEmissiveFactor = 23,
                kAmbient = 24,
                kAmbientFactor = 25,
                kDiffuse = 26,
                kDiffuseFactor = 27,
                kNormalMap = 28,
                kBump = 29,
                kBumpFactor = 30,
                kTransparentColor = 31,
                kTransparentFactor = 32,
                kDisplacementColor = 33,
                kDisplacementFactor = 34,
                kSpecular = 37,
                kSpecularFactor = 38,
                kShininess = 39,
                kReflection = 40,
                kReflectionFactor = 41,
                
                kTextures = 50,
                kPropName = 51,
                kTextureNames = 52,
                ktexColor=54,
                kcolBase=55,
                ktexNormal=56,
                ktexMetallic=57,
                kmetallic=58 ,
                ktexRoughness=59,
                kroughness=60,
                ktexEmissive=61,
                kcolEmissive=62,
                kemissiveIntensity=63,
                ktexAmbientOcclusion=64,
                
                kfbxTexture = 70,
                
                kCustomMaterials = 0xEE,
                kStandardMaterials = 0xFE
            };
            
            struct TextureRelationship
            {
                std::string Prop;
                std::vector<std::string> Names;
                
                ~TextureRelationship() {

                }
            };
            
            struct MaterialParameter
            {
                uint8_t MultiLayer = 0;
                std::string Name;
                std::string ShadingModel;
                
                std::vector<double> Emissive;
                double EmissiveFactor = 0.0;
                
                std::vector<double> Ambient;
                double AmbientFactor = 0.0;
                
                std::vector<double> Diffuse;
                double DiffuseFactor = 0.0;
                
                std::vector<double> NormalMap;
                
                std::vector<double> Bump;
                double BumpFactor = 0.0;
                
                std::vector<double>  TransparentColor;
                double TransparencyFactor = 0.0;
                
                std::vector<double> DisplacementColor;
                double DisplacementFactor = 0.0;
                
                std::vector<double> Specular;
                double SpecularFactor = 0.0;
                double Shininess = 0.0;
                
                std::vector<double> Reflection;
                double ReflectionFactor = 0.0;
                
                std::vector<TextureRelationship*> Textures;
                
                std::vector<double> colBase;
                double metallic = 0.0;
                double roughness = 0.0;
                std::vector<double>  colEmissive;
                double emissiveIntensity = 0.0;
                
                ~MaterialParameter() {
                    for (auto iter: this->Textures) {
                        delete iter;
                    }
                }
            };
            
            struct Material
            {
                std::string JSONString;
                std::vector<MaterialParameter*> Parameters;
                
                ~Material() {
                    for (auto iter: Parameters) {
                        delete iter;
                    }
                }
            };
        }
        
        namespace Texture
        {
            enum
            {
                kTextures = 1,
                
                kName = 3,
                kRelativeFilename = 4,
                kModelUVScaling = 5,
                kModelUVTranslation = 6,
                kTextureAlphaSource = 7,
                kCropping = 8,
                kBlendMode = 9,
                kWrapModeUV = 11,
                kUvRotation = 12,
                kUvScalingPivot = 13,
                kUvRotationPivot = 14,
                kPremultiply = 15,
                kOriginSize = 16,
                kAtlasTextureSize = 17
            };
            
            struct TextureParameter
            {
                std::string Name;
                std::string RelativeFileName;
                std::string TextureAlphaSource;
                int64_t BlendMode = 0;
                struct { int count = 0; float* data = nullptr; } ModelUVScaling;
                struct { int count = 0; float* data = nullptr; } ModelUVTranslation;
                struct { int count = 0; float* data = nullptr; } Cropping;
                struct { int count = 0; uint8_t* data = nullptr; } WrapModeUV;
                
                
                struct { int count = 0; float* data = nullptr; } ModelUVRotation;
                struct { int count = 0; float* data = nullptr; } ModelUVScalingPivot;
                struct { int count = 0; float* data = nullptr; } ModelUVRotationPivot;
                uint8_t premultiplyAlpha;
                struct { int count = 0; int32_t* data = nullptr; } OriginSize;
                struct { int count = 0; int32_t* data = nullptr; } AtlasTextureSize;

#ifdef BK_PLATFORM_IOS
                ~TextureParameter() {
                    delete[] ModelUVScaling.data;
                    delete[] ModelUVTranslation.data;
                    delete[] Cropping.data;
                    delete[] WrapModeUV.data;

                    delete[] ModelUVRotation.data;
                    delete[] ModelUVScalingPivot.data;
                    delete[] ModelUVRotationPivot.data;

                    delete[] OriginSize.data;
                    delete[] AtlasTextureSize.data;
                }
#endif
            };
            
            struct Texture
            {
                std::vector<TextureParameter*> Parameters;
                ~Texture() {
                    for (auto iter: Parameters) {
                        delete iter;
                    }
                }
            };
        }
        

        struct Accessory
        {
            struct Model Model;
            struct Geometry::Geometry Geometry;
            struct Deformer::Deformer Deformer;
            struct Texture::Texture Texture;
            struct Material::Material Material;
        };
        
        class FBXLoader: public CM::ILoader
        {
            std::vector<Accessory*>& _accessorys;
            std::vector<T3D::TDynamicBone *> &_dynamicBones;
            std::vector<T3D::TDynamicBoneColliderBase *> &_colliders;

            // Model
            bool loadModel(CMTLVReader& tlvReader, Model& model);
            
            // Geometry
            bool loadGeometry(CMTLVReader& tlvReader, Geometry::Geometry& geometry);
            
            // Deformer
            bool loadBlendShapeTargetShape(std::vector<CMTLVReader>& tlvReaders, CM::Accessory::Deformer::BlendShapeChannel::BlendShapeChannel& blendShapeChannel);
            bool loadBlendShapeChannel(std::vector<CMTLVReader>& tlvReaders, Deformer::BlendShapeDeformer::BlendShapeDeformer& blendShapeDeformer);
            bool loadBlendShapeDeformer(std::vector<CMTLVReader>& tlvReaders, Deformer::Deformer& deformer);
            bool loadSkinDeformerCluster(std::vector<CMTLVReader>& tlvReaders, Deformer::SkinDeformer::SkinDeformer& skinDeformer);
            bool loadSkinDeformer(std::vector<CMTLVReader>& tlvReaders, Deformer::Deformer& deformer);
            bool loadDeformer(CMTLVReader& tlvReader, Deformer::Deformer& deformer);
            
            // Material
            bool loadMaterialTextures(std::vector<CMTLVReader>& tlvReaders, Material::MaterialParameter& parameter);
            bool loadMaterialParameters(std::vector<CMTLVReader>& tlvReaders, Material::Material& Material);
            bool loadMaterial(CMTLVReader& tlvReader, Material::Material& Material);
            

            
            bool loadAccessory(CMTLVReader& tlvReader);
        public:
            // Texture
            static bool loadTextureParameters(std::vector<CMTLVReader>& tlvReaders, Texture::Texture& Texture);
            static bool loadTexture(CMTLVReader& tlvReader, Texture::Texture& Texture);
            
            typedef FBXLoader this_class;
            
            FBXLoader(std::vector<Accessory *> &accessorys, std::vector<T3D::TDynamicBone *> &dynamicBones, std::vector<T3D::TDynamicBoneColliderBase *> &colliders);
            virtual ~FBXLoader();
            
            virtual bool load(CMTLVReader& tlvReader);
        };
    }
}

#endif /* AccessoryLoader_h */
