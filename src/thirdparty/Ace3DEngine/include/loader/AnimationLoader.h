//
// Created by guangjinhuo on 2019-02-14.
//

#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "loader/RawBuffer.h"
#include "loader/HeaderInfo.h"
#include "loader/TlvReader.h"
#include "loader/TKeyframeTrack.h"
#include "loader/TAnimationClip.h"

namespace cm {

    namespace tlv {
        namespace tags {
            static const uint8_t kAnimation = 1;

            namespace animation {
                namespace animation_clip {
                    static const uint8_t kName = 1;
                    static const uint8_t kDuration = 2;
                    static const uint8_t kTracks = 3;
                }
                namespace animation_track {
                    static const uint8_t kName = 1;
                    static const uint8_t kTimes = 2;
                    static const uint8_t kValues = 3;
                    static const uint8_t kType = 4;
                }
            }
        }
    }

    namespace fbx {
        static void loadAnimationTrack(tlv::TLVReader<uint8_t> &reader, std::vector<T3D::TKeyframeTrack *> &tracks) {
            uint8_t type = 0;
            std::string name;
            int timesSize = 0;
            float *times = nullptr;
            int valuesSize = 0;
            float *values = nullptr;
            while (!reader.eof()) {
                auto tag = reader.getTag();
                switch (tag) {
                    case tlv::tags::animation::animation_track::kName:
                        reader.getString(name);
                        break;
                    case tlv::tags::animation::animation_track::kTimes:
                        reader.getFloat32Array(&timesSize, &times);
                        break;
                    case tlv::tags::animation::animation_track::kValues:
                        reader.getFloat32Array(&valuesSize, &values);
                        break;
                    case tlv::tags::animation::animation_track::kType:
                        reader.getUint8(type);
                        break;
                    default:
                        reader.jumpValue();
                }
            }

            if (type && timesSize && times && valuesSize && values) {
                auto *track = T3D::TKeyframeTrack::createKeyframeTrack(
                        static_cast<T3D::T_TRACK_TYPE>(type),
                        std::move(name),
                        timesSize,
                        times,
                        values
                );
                if (track) {
                    tracks.push_back(track);
                }
            }
        }

        static void loadAnimationClip(tlv::TLVReader<uint8_t> &reader, std::vector<T3D::TAnimationClip *> &clips) {
            std::string name;
            float duration = 0;
            std::vector<T3D::TKeyframeTrack *> tracks;
            while (!reader.eof()) {
                auto tag = reader.getTag();
                switch (tag) {
                    case tlv::tags::animation::animation_clip::kName:
                        reader.getString(name);
                        break;
                    case tlv::tags::animation::animation_clip::kDuration:
                        reader.getFloat32(duration);
                        break;
                    case tlv::tags::animation::animation_clip::kTracks: {
                        std::vector<CMTLVReader> subReaders;
                        if (reader.getTlvArray(subReaders)) {
                            for (auto subReader : subReaders) {
                                loadAnimationTrack(subReader, tracks);
                            }
                        }

                        break;
                    }
                    default:
                        reader.jumpValue();
                }
            }

            auto *clip = new T3D::TAnimationClip(name, duration, tracks);
            clips.push_back(clip);
        }

        static void loadAnimationClips(tlv::TLVReader<uint8_t> &reader, std::vector<T3D::TAnimationClip *> &clips) {
            while (!reader.eof()) {
                auto tag = reader.getTag();
                if (tag == tlv::tags::kAnimation) {
                    std::vector<CMTLVReader> subReaders;
                    if (reader.getTlvArray(subReaders)) {
                        for (auto subReader : subReaders) {
                            loadAnimationClip(subReader, clips);
                        }
                    }
                } else {
                    reader.jumpValue();
                }
            }
        }

        static std::vector<T3D::TAnimationClip *> loadAnimationClipsFromFile(const char *path) {
            std::vector<T3D::TAnimationClip *> clips;

            RawBuffer rawBuffer;
            bool result = rawBuffer.readFromFile(path);
            if (!result) {
                // TODO: 2019-02-14 report error
                return clips;
            }

            HeaderInfo headerInfo;
            result = headerInfo.load(rawBuffer);
            if (!result) {
                // TODO: 2019-02-14 report error
                return clips;
            }

//            BK_VAS_LOG(0, 0, "bin header", "path=%s version=%d.%d", path, headerInfo.major, headerInfo.minor);

            if (headerInfo.fileType != FileType::kThee) {
//                BK_VAS_LOG(0, 0, "bin header", "[AnimationLoader] unsupported file type: %d", headerInfo.fileType);
                return clips;
            }

            auto reader = CMTLVReader(
                    rawBuffer.data() + headerInfo.headerSize,
                    static_cast<const size_t>(headerInfo.bodySize)
            );

            loadAnimationClips(reader, clips);
            return clips;
        }

        static std::vector<T3D::TAnimationClip *> loadAnimationClipsFromBuffer(
                const char *buffer,
                size_t offset,
                size_t length
        ) {
            std::vector<T3D::TAnimationClip *> clips;

            auto reader = CMTLVReader(buffer + offset, length);

            loadAnimationClips(reader, clips);
            return clips;
        }
    }
}
