//
// Created by guangjinhuo on 2019-02-14.
//

#ifndef HEADERINFO_H
#define HEADERINFO_H

#include <cstdint>
#include <cstddef>
#include "RawBuffer.h"

namespace cm {

    namespace FileType {
        constexpr uint8_t kUnknown = 0;
        constexpr uint8_t kFBX = 1;
        constexpr uint8_t kGLTF = 2;
        constexpr uint8_t kThee = 3;
    }

    namespace BodyType {
        constexpr uint8_t kUnknown = 0;
        constexpr uint8_t kSkeleton = 1;
//        constexpr uint8_t kBody = 2;
        constexpr uint8_t kAccessory = 3;
        constexpr uint8_t kAnimation = 4;
//        constexpr uint8_t kFace = 5;
//        constexpr uint8_t kPose = 6;
    }

    /**
     *  claymore bin format
     *
     * ┌────────────────┐
     * │     header     │
     * │┌──────────────┐│
     * ││  magic 2B    ││
     * │├──────────────┤│
     * ││ version 4B   ││
     * │├──────────────┤│
     * ││header size 2B││
     * │├──────────────┤│
     * ││ body size 4B ││
     * │├──────────────┤│
     * ││ body type 1B ││
     * │├──────────────┤│
     * ││ file type 1B ││
     * │├──────────────┤│
     * ││ raw type 1B  ││
     * │├──────────────┤│
     * ││ timestamp 4B ││
     * │├──────────────┤│
     * ││  preserve nB ││
     * │└──────────────┘│
     * │┌──────────────┐│ <- headerSize
     * ││    body      ││
     * │└──────────────┘│
     * └────────────────┘
     */
    class HeaderInfo {
    public:
        struct {
            uint16_t major;
            uint16_t minor;
        };
        int16_t headerSize;
        int32_t bodySize;
        int8_t bodyType;
        int8_t fileType;
        int8_t rawType;
        char formatTime[32]; // xxxx-xx-xx xx:xx:xx

        bool load(RawBuffer &rawBuffer);
        bool load(char* data, size_t size);
    };
}

#endif
