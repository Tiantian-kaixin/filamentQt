//
// Created by guangjinhuo on 2019-02-14.
//

#ifndef RAWBUFFER_H
#define RAWBUFFER_H

#include <cstddef>

namespace cm {

    class RawBuffer {
    private:
        size_t _size;
        char *_data;

    public:
        static bool readFile(const char *path, char **data, size_t *size);

        RawBuffer() : _size(0), _data(nullptr) {}

        ~RawBuffer();

        bool readFromFile(const char *path);

        size_t size() const { return _size; };

        const char *data() const { return _data; };
    };
}

#endif
