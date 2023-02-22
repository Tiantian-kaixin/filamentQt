//
//  FTLoader.h
//  brick
//
//  Created by louisluo on 2019/2/2.
//  Copyright © 2019 louisluo. All rights reserved.
//

#ifndef FTLoader_h
#define FTLoader_h

#include <string>
#include "TlvReader.h"

namespace CM
{
    class ILoader
    {
    public:
        virtual ~ILoader() {}
        
        virtual bool load(CMTLVReader& tlvReader) = 0;
    };
    
    class FTLoader
    {
        FILE* _fp;
        size_t _size;
        char* _data;
    public:
        FTLoader();
        
        ~FTLoader();
        
        size_t size() const { return _size; }
        const char* data() const { return _data; }
        
        bool load(const std::string& path);
    };
}

#endif /* FTLoader_h */
