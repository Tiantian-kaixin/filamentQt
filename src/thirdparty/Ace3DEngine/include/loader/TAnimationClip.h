#pragma once
#include <string>
#include <vector>
#include "loader/TDefine.h"

namespace T3D {
    class TObject3D;
    class TKeyframeTrack;

    class TAnimationClip {
    public:
        TAnimationClip(const std::string &name, float duration, const std::vector<TKeyframeTrack *> &tracks);

        ~TAnimationClip();

    protected:
        void resetDuration();

    public:
        inline const std::string& getName() const { return _name; }
        inline float getDuration() const { return _duration; }

        inline size_t getTrackCount() const { return _tracks.size(); }
        TKeyframeTrack* getTrack(int index) { return _tracks[index]; }

    protected:
        std::string                     _name;
        float                           _duration = 0.0f;

        std::vector<TKeyframeTrack*>    _tracks;
    };
}
