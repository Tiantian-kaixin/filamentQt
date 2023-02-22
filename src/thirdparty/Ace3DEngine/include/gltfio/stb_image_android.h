//
// Created by ericwywang on 2020/1/8.
//

#ifndef TNT_STB_IMAGE_ANDROID_H
#define TNT_STB_IMAGE_ANDROID_H

#include <functional>

namespace gltfio {

    using image_load_from_file_handle =
        std::function<unsigned char *(char const *path, int inSampleSize)>;
    using image_info_from_file_handle =
        std::function<bool(char const *path, int *width, int *height, int *sampleSize)>;
    using image_load_from_memory_handle =
        std::function<unsigned char *(unsigned char const *data, int len, int inSampleSize)>;
    using image_info_from_memory_handle = std::function<bool(unsigned char const *data, int len,
                                                             int *width, int *height, int *sampleSize)>;

    struct stb_image_handle {
        image_load_from_file_handle image_load_from_file;
        image_info_from_file_handle image_info_from_file;
        image_load_from_memory_handle image_load_from_memory;
        image_info_from_memory_handle image_info_from_memory;
    };
}  // namespace gltfio

#endif  // TNT_STB_IMAGE_ANDROID_H
