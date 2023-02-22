//
//  MakeupLoader.h
//  brick
//
//  Created by louisluo on 2019/1/29.
//  Copyright © 2019 louisluo. All rights reserved.
//

#ifndef MakeupLoader_h
#define MakeupLoader_h

#include <string>
#include "FTLoader.h"
#include "AccessoryLoader.h"

namespace CM
{
    
    namespace Makeup
    {
        enum
        {
            kMakeup = 40,
            kMakeup_AttachmentTag = 41,
            kMakeup_TextureTag = 42,
        };
        struct Makeup
        {
            std::string attachMent;
            struct CM::Accessory::Texture::Texture texture;
        };
        
        class MakeupLoader: public CM::ILoader
        {
            Makeup* &_makeups;
            
            //makeup
            bool loadMakeup(CMTLVReader& tlvReader, CM::Makeup::Makeup &makeup);
            
        public:
            //        typedef FBXLoader this_class;
            MakeupLoader(Makeup* &makeups);
            virtual ~MakeupLoader();
            virtual bool load(CMTLVReader& tlvReader);
        };
    }
    
    
}

#endif /* MakeupLoader_h */
