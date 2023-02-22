//
// Created by kevinxing on 2019-10-29.
//

#ifndef TNT_FILE_UTIL_H
#define TNT_FILE_UTIL_H

#include <fstream>
#include <string>
#include <utils/Path.h>
#include <filament/Texture.h>
#include <filament/Material.h>
#include <filament/MaterialInstance.h>
#include <gltfio/log_util.h>
#include <gltfio/common_util.h>
#include <sstream>

#if ENABLE_LOG
#include <iostream>
#endif

LIGHT3D_NAMESPACE_BEGIN

namespace file {

static std::string loadStringFile(const utils::Path& path) {
    std::ifstream t(path);
    std::stringstream buffer;
    buffer << t.rdbuf();
    return buffer.str();
}

static std::vector<uint8_t> loadFile(const utils::Path &);

static std::ifstream::pos_type getFileSize(const char *filename) {
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    return in.tellg();
}

static std::vector<uint8_t> loadAsset(const utils::Path &resourcePath, const utils::Path &filename) {
    std::vector<uint8_t> buffer;
    utils::Path path = resourcePath.concat(filename);
    if (!path.exists()) {
        path = resourcePath.concat("head").concat(filename);
    }
    if (!path.exists()) {
#if ENABLE_LOG
        LOGE << "Unable to find file " << filename << LOGEND;
#endif
        return buffer;
    }
    return loadFile(path);
}

static bool hasExtension(const utils::Path filename, std::string extension) {
    return filename.getExtension() == extension;
}

static std::vector<uint8_t> loadFile(const utils::Path &filename) {
  long contentSize = static_cast<long>(getFileSize(filename.c_str()));
  std::vector<uint8_t> buffer;
  if (contentSize <= 0 || contentSize >= buffer.max_size()) {
    LOGE << "Unable to open " << filename.getName()
         << ", fileLength: " << contentSize << LOGEND;
    assert(0);
  }

  // Consume the glTF file.
  std::ifstream in(filename.c_str(), std::ifstream::in | std::ifstream::binary);
  buffer.resize(contentSize);
  if (!in.read((char *) buffer.data(), contentSize)) {
    LOGE << "Unable to read " << filename.getName()
         << ", fileLength: " << contentSize << LOGEND;
    //return std::vector<uint8_t>(0);
    assert(0);
  }
  return buffer;
}

static bool saveFile(const utils::Path &filePath, void *data, size_t length) {
    std::ofstream out(filePath);
    out.write(static_cast<char *>(data), length);
    out.close();
    return true;
}

static bool fileExist(const std::string &name) {
    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }
}

template<typename T>
static std::vector<T> loadFile(const utils::Path &filePath, char delimiter) {
    std::vector<T> result;
    std::ifstream in(filePath, std::ifstream::in);
    if (!in.is_open()) {
        printf("[filament] Error opening file %s\n", filePath.c_str());
        return result;
    }

    in.seekg(0);

    int i = 0;
    std::string buff;
    char *end;

    while (!in.eof()) {
        getline(in, buff, delimiter);
        T t = strtof(buff.c_str(), &end);
        result.push_back(t);
    }

    return std::move(result);
}

}

LIGHT3D_NAMESPACE_END

#endif //TNT_FILE_UTIL_H
