//
//  SkeletonLoader.h
//  brick
//
//  Created by louisluo on 2019/1/29.
//  Copyright © 2019 louisluo. All rights reserved.
//

#ifndef SkeletonLoader_h
#define SkeletonLoader_h

#include <string>
#include <vector>
#include "FTLoader.h"

namespace CM
{
    namespace Skeleton
    {
        enum {
            kName = 1,
            kType = 2,
            kParent = 3,
            kMatrix = 4,
            kEulerOrder = 5,
            kPreRotation = 6,
            kPostRotation = 7,
            
            kSkeleton = 9
        };
        
        struct Skeleton
        {
            uint8_t Type = 0;
            uint8_t EulerOrder = 0;
            std::string Name;
            std::string Parent;
            struct {
                int count = 0;
                float* data = nullptr;
            } LocalTransform;
            struct {
                int count = 0;
                float* data = nullptr;
            } PreRotation;
            struct {
                int count = 0;
                float* data = nullptr;
            } PostRotation;

#ifdef BK_PLATFORM_IOS
            ~Skeleton() {
                delete[] LocalTransform.data;
                delete[] PreRotation.data;
                delete[] PostRotation.data;
            }
#endif
        };
        
        class FBXLoader: public CM::ILoader
        {
            std::vector<Skeleton*>& _skeletons;
            
            bool loadSkeleton(CMTLVReader& tlvReader);
        public:
            typedef FBXLoader this_class;
            
            FBXLoader(std::vector<Skeleton*>& skeletons);
            virtual ~FBXLoader();
            
            virtual bool load(CMTLVReader& tlvReader);
        };
    }
}


#endif /* SkeletonLoader_h */
