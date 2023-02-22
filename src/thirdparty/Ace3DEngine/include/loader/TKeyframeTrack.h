#pragma once
#include <string>
#include <functional>
#include <vector>
#include "loader/TDefine.h"

namespace T3D {
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class TKeyframeTrack {
        friend class TInterpolant;
        friend class TPropertyMixer;

    public:
        explicit TKeyframeTrack(const std::string& name);
        virtual ~TKeyframeTrack();

        static TKeyframeTrack *createKeyframeTrack(
                T_TRACK_TYPE type,
                const std::string &name,
                const int keyframeCount,
                const float *times,
                const float *values
        );

        virtual int getValueSize() = 0;
        virtual T_INTERPOLANT_TYPE getInterpolantType() = 0;

        //virtual void evaluate(float time, bool resetKeyframeIndex, int accuIndex, float weight, bool print);

    public:
        void shift(float timeOffset);
        void scale(float timeScale);

    public:
        inline const std::string& getTrackName() const { return _name; }

        float getLastKeyframeTime() const { return _times[_keyframeCount-1]; }

//    private:
        std::string             _name;
        int                     _keyframeCount = 0;
        float*                  _times = nullptr;
        float*                  _values = nullptr;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // vector: float 3      position, scale
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class TVectorKeyframeTrack : public TKeyframeTrack {
    public:
        explicit TVectorKeyframeTrack(const std::string& name) : TKeyframeTrack(name) { }
        virtual ~TVectorKeyframeTrack() { }

        virtual int getValueSize() { return 3; }
        virtual T_INTERPOLANT_TYPE getInterpolantType() { return T_INTERPOLANT_TYPE::INTERPOLANT_LINEAR; }
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // float                morph
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class TNumberKeyframeTrack : public TKeyframeTrack {
    public:
        explicit TNumberKeyframeTrack(const std::string& name) : TKeyframeTrack(name) { }
        virtual ~TNumberKeyframeTrack() { }

        virtual int getValueSize() { return 1; }
        virtual T_INTERPOLANT_TYPE getInterpolantType() { return T_INTERPOLANT_TYPE::INTERPOLANT_LINEAR; }
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // quaternion: float 4      rotation
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class TQuaternionKeyframeTrack : public TKeyframeTrack {
    public:
        explicit TQuaternionKeyframeTrack(const std::string& name) : TKeyframeTrack(name) { }
        virtual ~TQuaternionKeyframeTrack() { }

        virtual int getValueSize() { return 4; }
        virtual T_INTERPOLANT_TYPE getInterpolantType() { return T_INTERPOLANT_TYPE::INTERPOLANT_QUATERNION_LINEAR; }
    };
}
