//
// Created by kevinxing on 2020/8/20.
//

#ifndef TNT_DECODE_IMAGE_UTIL_H
#define TNT_DECODE_IMAGE_UTIL_H

#if 0 //(FILAMENT_ENV_IOS || (IOS))
    #define stbiForiOS 1
#else
    #define stbiForiOS 0
#endif

#if stbiForiOS
    #include <gltfio/stb_image_iOS.h>
#else
    #include <stb_image.h>
#endif

#endif //TNT_DECODE_IMAGE_UTIL_H
